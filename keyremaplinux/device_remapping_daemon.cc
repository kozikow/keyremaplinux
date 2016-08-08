#include "device_remapping_daemon.h"

#include <linux/input.h>
#include <pthread.h>
#include <string>

#include "keyremaplinux/device/input_device.h"
#include "keyremaplinux/device/output_device.h"
#include "keyremaplinux/util/logging.h"


namespace keyremaplinux {

DeviceRemappingDaemon::~DeviceRemappingDaemon() {
  delete inputDevice_;
  delete outputDevice_;
}

DeviceRemappingDaemon::DeviceRemappingDaemon(const std::string& inputPath,
    Remapper* remapper) : remapper_(remapper) {
  inputDevice_ = new InputDevice(inputPath);
  outputDevice_ = new OutputDevice();
}

pthread_t DeviceRemappingDaemon::Run() {
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
  while(true) {
    input_event* ev = inputDevice_->ReadInputEvent();
    std::vector<input_event*> remapped = remapper_->Remap(ev);
    for (input_event* event : remapped) {
      outputDevice_->WriteInputEvent(event);
    }
    if (remapped.size() > 0) {
      outputDevice_->WriteSyncEvent();
    }
  }
}

}  // end namespace keyremaplinux
