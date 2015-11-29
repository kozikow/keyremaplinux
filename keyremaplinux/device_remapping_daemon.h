#ifndef KEYREMAPLINUX_DEVICE_REMAPPING_DAEMON
#define KEYREMAPLINUX_DEVICE_REMAPPING_DAEMON

#include <string>

#include "device/input_device.h"
#include "device/output_device.h"
#include "remapper/remapper.h"

namespace keyremaplinux {

class DeviceRemappingDaemon {
public:
  DeviceRemappingDaemon(const string& inputPath, Remapper* remapper);

  virtual ~DeviceRemappingDaemon();

  pthread_t Run();
  
private:
  static string FindUInputDevice();
  
  void EnableUInputEvents();

  void CreateDeviceFromUInput();

  pthread_t StartRemappingThread();

  static void* RemappingThreadMainStub(void* data);

  void RemappingThreadMain();
  
  // Outputs event that signals end of output chunk.
  void OutputSyncEvent();

  DeviceRemappingDaemon(const DeviceRemappingDaemon& daemon) = delete;

  void operator=(const DeviceRemappingDaemon&) = delete;


  string outputPath_ = "";

  // Descriptor of device we are writing to
  int outputDescriptor_ = 0;

  Remapper* remapper_ = nullptr;
  
  InputDevice* inputDevice_ = nullptr;
  
  OutputDevice* outputDevice_ = nullptr;
};

}  // end namespace keyremaplinux

#endif  // KEYREMAPLINUX_DEVICE_REMAPPING_DAEMON
