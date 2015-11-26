#include <string>

#include <linux/input.h>
#include <linux/uinput.h>
#include <sys/fcntl.h>
#include <unistd.h>

#include "device_remapping_daemon.h"
#include "util/logging.h"

namespace keyremaplinux {

using namespace std;

static const string uinputDevicePaths[] = {"/dev/input/uinput", "/dev/input/uinput"};

DeviceRemappingDaemon::~DeviceRemappingDaemon() {
}

DeviceRemappingDaemon::DeviceRemappingDaemon(const string& path,
    Remapper* remapper) : inputPath_(path), remapper_(remapper) {
  string uInput = FindUInputDevice();
  CHECK(uInput != "");
  outputDescriptor_ = open(uInput.c_str(), O_WRONLY | O_NONBLOCK);
  CHECK(outputDescriptor_ < 0);
  int ret = ioctl(outputDescriptor_, UI_SET_EVBIT, EV_KEY);
  ret = ioctl(outputDescriptor_, UI_SET_EVBIT, EV_SYN);
}

string DeviceRemappingDaemon::FindUInputDevice() {
  for (string path : uinputDevicePaths) {
    if (access(path.c_str(), F_OK) != -1) {
      return path;
    }
  }
  return "";
}

}  // end namespace keyremaplinux
