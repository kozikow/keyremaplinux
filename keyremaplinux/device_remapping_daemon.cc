#include <string>

#include <linux/input.h>
#include <linux/uinput.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

#include "device_remapping_daemon.h"
#include "util/logging.h"

namespace keyremaplinux {

using namespace std;

static const string uinputDevicePaths[] = {"/dev/input/uinput", "/dev/uinput"};

DeviceRemappingDaemon::~DeviceRemappingDaemon() {
  if (outputDescriptor_ > 0) {
    ioctl(outputDescriptor_, UI_DEV_DESTROY);
  }
  ioctl(inputDescriptor_, EVIOCGRAB, (void*)0); // ungrab
  close(outputDescriptor_);
  close(inputDescriptor_);
}

DeviceRemappingDaemon::DeviceRemappingDaemon(const string& inputPath,
    Remapper* remapper) : inputPath_(inputPath), remapper_(remapper) {
  inputDescriptor_ = open(inputPath_.c_str(), O_RDONLY);
  CHECK(inputDescriptor_ > 0);
  GrabInputDevice();

  string uInputPath = FindUInputDevice();
  CHECK(uInputPath != "");
  outputDescriptor_ = open(uInputPath.c_str(), O_WRONLY | O_NONBLOCK);
  CHECK(outputDescriptor_ > 0);

  EnableUInputEvents();
  CreateDeviceFromUInput();
  CHECK(outputDescriptor_ > 0);
}

string DeviceRemappingDaemon::FindUInputDevice() {
  for (string path : uinputDevicePaths) {
    if (access(path.c_str(), F_OK) != -1) {
      return path;
    }
  }
  return "";
}
  
void DeviceRemappingDaemon::GrabInputDevice() {
  ioctl(inputDescriptor_, EVIOCGRAB, (void*)1);
}

void DeviceRemappingDaemon::EnableUInputEvents() {
  // Enable keys being sent.
  CHECK(!ioctl(outputDescriptor_, UI_SET_EVBIT, EV_KEY));

  // Enable synchronization events being sent.
  CHECK(!ioctl(outputDescriptor_, UI_SET_EVBIT, EV_SYN));

  // Enable all keybord characters.
  for (int key = 0; key < KEY_MAX; key++) {
    int res = ioctl(outputDescriptor_, UI_SET_KEYBIT, key);
    if (res != 0) {
      LOG(WARNING) << "ioctl failed for character " << key << " with code " << res;
    }
  }
}

void DeviceRemappingDaemon::CreateDeviceFromUInput() {
  struct uinput_user_dev uidev;
  memset(&uidev, 0, sizeof(uidev));

  outputPath_ = "uinput-device";
  snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, outputPath_.c_str());
  uidev.id.bustype = BUS_USB;
  uidev.id.vendor  = 0x1;
  uidev.id.product = 0x1;
  uidev.id.version = 1;
  CHECK(write(outputDescriptor_, &uidev, sizeof(uidev)) > 0);
  CHECK(!ioctl(outputDescriptor_, UI_DEV_CREATE));
}

pthread_t DeviceRemappingDaemon::Run() {
  // See http://thiemonge.org/getting-started-with-uinput
  return StartRemappingThread();
}

pthread_t DeviceRemappingDaemon::StartRemappingThread() {
  pthread_t thread;
  CHECK(!pthread_create(&thread, NULL,
      &DeviceRemappingDaemon::RemappingThreadMainStub, this));
  return thread;
}

void* DeviceRemappingDaemon::RemappingThreadMainStub(void* deviceRemappingDaemon) {
  reinterpret_cast<DeviceRemappingDaemon*>(deviceRemappingDaemon)->
      RemappingThreadMain();
  return nullptr;
}

void DeviceRemappingDaemon::RemappingThreadMain() {
  input_event ev;
  while(true) {
    LOG(INFO) << "Waiting for event for device " + inputPath_;
    CHECK(read(inputDescriptor_, &ev, sizeof(ev)) > 0);
    vector<input_event> remapped = remapper_->Remap(ev);
    for (input_event event : remapped) {
      CHECK(write(outputDescriptor_, &event, sizeof(event)) > 0);
    }
    if (remapped.size() > 0) {
      OutputSyncEvent();
    }
  }
}

void DeviceRemappingDaemon::OutputSyncEvent() {
  LOG(INFO) << "Sending syn event";
  struct input_event ev;
  memset(&ev, 0, sizeof(ev));
  ev.type = EV_SYN;
  ev.code = 0;
  ev.value = 0;
  CHECK(write(outputDescriptor_, &ev, sizeof(ev)) >= 0);
}

}  // end namespace keyremaplinux
