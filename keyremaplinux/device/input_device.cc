#include "input_device.h"

#include <linux/input.h>
#include <linux/uinput.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <string>

#include "keyremaplinux/util/logging.h"

namespace keyremaplinux {

InputDevice::~InputDevice() {
  if (inputDescriptor_ > 0) {
    UnGrabInputDevice();
    close(inputDescriptor_);
  }
}

InputDevice::InputDevice(const std::string& inputPath) : inputPath_(inputPath) {
  inputDescriptor_ = open(inputPath_.c_str(), O_RDONLY);
  CHECK(inputDescriptor_ > 0);
  GrabInputDevice();
}

void InputDevice::GrabInputDevice() {
  CHECK(!ioctl(inputDescriptor_, EVIOCGRAB, (void*)1));
}

void InputDevice::UnGrabInputDevice() {
  CHECK(!ioctl(inputDescriptor_, EVIOCGRAB, (void*)0));
}
  
input_event InputDevice::ReadInputEvent() {
  input_event ev;
  CHECK(read(inputDescriptor_, &ev, sizeof(ev)) > 0);
  return ev;
}

}  // end namespace keyremaplinux
