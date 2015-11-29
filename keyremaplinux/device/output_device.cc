#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

#include <linux/input.h>
#include <linux/uinput.h>

#include "../util/logging.h"
#include "output_device.h"

namespace keyremaplinux {

using namespace std;

// Dependent on the OS.
static const string uinputDevicePaths[] = {"/dev/input/uinput", "/dev/uinput"};

OutputDevice::~OutputDevice() {
  if (outputDescriptor_ > 0) {
    ioctl(outputDescriptor_, UI_DEV_DESTROY);
    close(outputDescriptor_);
  }
}

OutputDevice::OutputDevice() {
  // uInput device is used to create actual device we will be writing to.
  // See http://thiemonge.org/getting-started-with-uinput
  string uInputPath = FindUInputDevice();
  CHECK(uInputPath != "");
  outputDescriptor_ = open(uInputPath.c_str(), O_WRONLY | O_NONBLOCK);
  CHECK(outputDescriptor_ > 0);

  EnableUInputEvents();
  CreateDeviceFromUInput();
  CHECK(outputDescriptor_ > 0);
}

string OutputDevice::FindUInputDevice() {
  for (string path : uinputDevicePaths) {
    if (access(path.c_str(), F_OK) != -1) {
      return path;
    }
  }
  return "";
}
  
void OutputDevice::EnableUInputEvents() {
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
  
void OutputDevice::CreateDeviceFromUInput() {
  struct uinput_user_dev uidev;
  memset(&uidev, 0, sizeof(uidev));

  snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "uinput-device");
  uidev.id.bustype = BUS_USB;
  uidev.id.vendor  = 0x1;
  uidev.id.product = 0x1;
  uidev.id.version = 1;
  CHECK(write(outputDescriptor_, &uidev, sizeof(uidev)) > 0);
  CHECK(!ioctl(outputDescriptor_, UI_DEV_CREATE));
}

void OutputDevice::WriteInputEvent(input_event event) {
  CHECK(write(outputDescriptor_, &event, sizeof(event)) > 0);
}

void OutputDevice::WriteSyncEvent() {
  struct input_event ev;
  memset(&ev, 0, sizeof(ev));
  ev.type = EV_SYN;
  ev.code = 0;
  ev.value = 0;
  CHECK(write(outputDescriptor_, &ev, sizeof(ev)) >= 0);
}
  
}  // end namespace keyremaplinux
