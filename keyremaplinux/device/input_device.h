#ifndef KEYREMAPLINUX_DEVICE_INPUT_DEVICE
#define KEYREMAPLINUX_DEVICE_INPUT_DEVICE

#include <linux/input.h>
#include <string>

namespace keyremaplinux {
  
class InputDevice {
public:
  InputDevice(const std::string& path);

  virtual ~InputDevice();

  input_event ReadInputEvent();

private:
  // Grabing gives exclusive access to the device to this process.
  void GrabInputDevice();

  void UnGrabInputDevice();

std::string inputPath_ = "";

  // Descriptor of the device we are reading from.
  int inputDescriptor_ = 0;

  InputDevice(const InputDevice& daemon) = delete;

  void operator=(const InputDevice&) = delete;

};

}  // end namespace keyremaplinux

#endif  // KEYREMAPLINUX_DEVICE_INPUT_DEVICE
