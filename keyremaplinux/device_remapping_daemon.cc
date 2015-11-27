#include <string>

#include <linux/input.h>
#include <linux/uinput.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <pthread.h>

#include "device_remapping_daemon.h"
#include "util/logging.h"

namespace keyremaplinux {

using namespace std;

static const string uinputDevicePaths[] = {"/dev/input/uinput", "/dev/uinput"};

DeviceRemappingDaemon::~DeviceRemappingDaemon() {
}

DeviceRemappingDaemon::DeviceRemappingDaemon(const string& inputPath,
    Remapper* remapper) : inputPath_(inputPath), remapper_(remapper) {
  inputDescriptor_ = open(inputPath_.c_str(), O_RDONLY);
  CHECK(inputDescriptor_ > 0);

  string uInput = FindUInputDevice();
  CHECK(uInput != "");
  outputDescriptor_ = open(uInput.c_str(), O_WRONLY);
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

pthread_t DeviceRemappingDaemon::Run() {
  // See http://thiemonge.org/getting-started-with-uinput
  EnableUInputEvents();
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
  struct input_event ev;
  while(true) {
    CHECK(!read(inputDescriptor_, &ev, sizeof(ev)));
    auto remapped = remapper_->Remap(ev);
    for (auto ev : remapped) {
      CHECK(!write(outputDescriptor_, &ev, sizeof(ev)));
    }
  }
}

}  // end namespace keyremaplinux
