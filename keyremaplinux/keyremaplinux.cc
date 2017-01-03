#include <cstdio>
#include <dirent.h> 
#include <limits.h>
#include <iostream>
#include <string>
#include <vector>

#include "keyremaplinux/remapper/remapper.h"
#include "keyremaplinux/remapper/kozikow_layout_remapper.h"
#include "keyremaplinux/remapper/keyboard_type.h"
#include "device_remapping_daemon.h"
#include "util/logging.h"

namespace keyremaplinux {

  static const std::string dirName = "/dev/input";

  static const std::string devicesCmd = (std::string) "cat /proc/bus/input/" +
      "devices | grep Handlers | grep kbd | grep -v mouse | grep -o \"event[0-9]*\"";

  std::vector<std::string> FindKeyboardDevices() {
    // Finds list of keyboards based on contests of the /proc/bus/input/devices.
    // It's a bit hacky, as it relies on parsing text format.
    // Sadly, a few other ways didn't work. Those include:
    // 1. Finding keyboards from /dev/input/by-id
    // 2. Finding keyboards from /dev/input/by-path
    // Just reading all devices from /dev/input would make the mouse unusable.

    FILE *fp = popen(devicesCmd.c_str(), "r");
    CHECK(fp != NULL);

    char fileName[NAME_MAX+1];
    std::vector<std::string> fileNames;
    while(fscanf(fp, "%s", fileName) > 0) {
      fileNames.push_back(dirName + "/" + fileName);
    }
    return fileNames;
  }

}  // end namespace keyremaplinux

int main(int argc, char* argv[]) {
  std::vector<std::string> devices = keyremaplinux::FindKeyboardDevices();
  if (devices.empty()) {
    LOG(WARNING) << "Did not find any input devices";
  }

  std::string layoutName(argv[1]);
  keyremaplinux::Remapper* remapper;
  if (layoutName == "kozikow_standard") {
    remapper = new keyremaplinux::KozikowLayoutRemapper(500, keyremaplinux::standard);
  } else if (layoutName == "kozikow_mac") {
    remapper = new keyremaplinux::KozikowLayoutRemapper(500, keyremaplinux::mac);
  } else {
    throw "Unrecognized layout " + layoutName;
  }

  std::vector<pthread_t> threads;
  for (std::string device : devices) {
    LOG(INFO) << "Opening device " << device;
    keyremaplinux::DeviceRemappingDaemon* daemon =
        new keyremaplinux::DeviceRemappingDaemon(device, remapper);
    pthread_t thread = daemon->Run();
    threads.push_back(thread);
  }

  LOG(INFO) << "Waiting for threads";
  for (pthread_t thread : threads) {
    CHECK(!pthread_join(thread, nullptr));
  }
  return 0;
}
