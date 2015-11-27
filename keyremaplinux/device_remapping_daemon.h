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

  void EnableUInputEvents();

  pthread_t StartRemappingThread();

  static void* RemappingThreadMainStub(void* data);

  void RemappingThreadMain();

  string inputPath_ = "";

  int inputDescriptor_ = 0;

  int outputDescriptor_ = 0;

  Remapper* remapper_ = nullptr;
};

}  // end namespace keyremaplinux

#endif  // KEYREMAPLINUX_DEVICE_REMAPPING_DAEMON
