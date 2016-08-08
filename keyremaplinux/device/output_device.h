#ifndef KEYREMAPLINUX_DEVICE_OUTPUT_DEVICE
#define KEYREMAPLINUX_DEVICE_OUTPUT_DEVICE

#include <linux/input.h>
#include <string>

namespace keyremaplinux {
  
// Actual device we will be writing to is created using the uInput system.
// See http://thiemonge.org/getting-started-with-uinput
class OutputDevice {
public:
  OutputDevice();

  virtual ~OutputDevice();

  void WriteInputEvent(input_event event);

  // Outputs event that signals end of output chunk.
  void WriteSyncEvent();

private:
  static std::string FindUInputDevice();
  
  void EnableUInputEvents();

  void CreateDeviceFromUInput();

  // Descriptor of device we are writing to
  int outputDescriptor_ = 0;

  struct input_event *syncEvent_;

  OutputDevice(const OutputDevice& daemon) = delete;

  void operator=(const OutputDevice&) = delete;


};

}  // end namespace keyremaplinux

#endif  // KEYREMAPLINUX_DEVICE_OUTPUT_DEVICE
