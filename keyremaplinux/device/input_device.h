#ifndef KEYREMAPLINUX_DEVICE_INPUT_DEVICE
#define KEYREMAPLINUX_DEVICE_INPUT_DEVICE

#include <string>

namespace keyremaplinux {
  
using namespace std;

class InputDevice {
public:
  InputDevice(const string& path);

  virtual ~InputDevice();

  input_event ReadInputEvent();

private:
  // Grabing gives exclusive access to the device to this process.
  void GrabInputDevice();

  void UnGrabInputDevice();

  string inputPath_ = "";

  // Descriptor of the device we are reading from.
  int inputDescriptor_ = 0;

  InputDevice(const InputDevice& daemon) = delete;

  void operator=(const InputDevice&) = delete;

};

}  // end namespace keyremaplinux

#endif  // KEYREMAPLINUX_DEVICE_INPUT_DEVICE
