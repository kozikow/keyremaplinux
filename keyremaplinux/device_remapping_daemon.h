#ifndef KEYREMAPLINUX_DEVICE_REMAPPING_DAEMON
#define KEYREMAPLINUX_DEVICE_REMAPPING_DAEMON

#include <string>

#include "remapper/remapper.h"

namespace keyremaplinux {

class DeviceRemappingDaemon {
public:
  DeviceRemappingDaemon(const string& path, Remapper* remapper);

  virtual ~DeviceRemappingDaemon();

  pthread_t Run();

private:

  DeviceRemappingDaemon(const DeviceRemappingDaemon& daemon) = delete;

  void operator=(const DeviceRemappingDaemon&) = delete;

  static string FindUInputDevice();
  
  void GrabInputDevice();

  void EnableUInputEvents();

  void CreateDeviceFromUInput();

  pthread_t StartRemappingThread();

  static void* RemappingThreadMainStub(void* data);

  void RemappingThreadMain();
  
  // Outputs event that signals end of output chunk.
  void OutputSyncEvent();

  string inputPath_ = "";

  string outputPath_ = "";

  // Descriptor of device we are reading from
  int inputDescriptor_ = 0;

  // Descriptor of device we are writing to
  int outputDescriptor_ = 0;

  Remapper* remapper_ = nullptr;
};

}  // end namespace keyremaplinux

#endif  // KEYREMAPLINUX_DEVICE_REMAPPING_DAEMON
