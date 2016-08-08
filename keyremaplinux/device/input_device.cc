#include "input_device.h"

#include <linux/input.h>
#include <linux/uinput.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <string>
#include <cstring>

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

  // Preallocate struct that will be used for reading input events.
  struct input_event ev;
  inputEvent_ = (input_event*) malloc(sizeof(ev));
  memset(inputEvent_, 0, sizeof(ev));
  inputEvent_->type = EV_SYN;
  inputEvent_->code = 0;
  inputEvent_->value = 0;
}

void InputDevice::GrabInputDevice() {
  CHECK(!ioctl(inputDescriptor_, EVIOCGRAB, (void*)1));
}

void InputDevice::UnGrabInputDevice() {
  CHECK(!ioctl(inputDescriptor_, EVIOCGRAB, (void*)0));
}
  
input_event* InputDevice::ReadInputEvent() {
  CHECK(read(inputDescriptor_, inputEvent_, sizeof(*inputEvent_)) > 0);
  return inputEvent_;
}

}  // end namespace keyremaplinux
