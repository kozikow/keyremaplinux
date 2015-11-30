#include <iostream>
#include <vector>
#include <cstdio>
#include <string>
#include <dirent.h> 
#include <limits.h>

#include "keyremaplinux/remapper/remapper.h"
#include "keyremaplinux/remapper/kozikow_layout_remapper.h"
#include "device_remapping_daemon.h"
#include "util/logging.h"

namespace keyremaplinux {

  using namespace std;

  static const string dirName = "/dev/input";

  static const string devicesCmd = (string) "cat /proc/bus/input/devices | " +
      " grep Handlers | grep kbd | grep -o \"event[0-9]*\"";

  vector<string> FindKeyboardDevices() {
    // Finds list of keyboards based on contests of the /proc/bus/input/devices.
    // It's a bit hacky, as it relies on parsing text format.
    // Sadly, a few other ways didn't work. Those include:
    // 1. Finding keyboards from /dev/input/by-id
    // 2. Finding keyboards from /dev/input/by-path
    // Just reading all devices from /dev/input would make the mouse unusable.

    FILE *fp = popen(devicesCmd.c_str(), "r");
    CHECK(fp != NULL);

    char fileName[NAME_MAX+1];
    vector<string> fileNames;
    while(fscanf(fp, "%s", fileName) > 0) {
      fileNames.push_back(dirName + "/" + fileName);
    }
    return fileNames;
  }

}  // end namespace keyremaplinux

int main(int argc, char* argv[]) {
  using namespace std;
  vector<string> devices = keyremaplinux::FindKeyboardDevices();
  if (devices.empty()) {
    LOG(WARNING) << "Did not find any input devices";
  }

  keyremaplinux::Remapper* remapper = new keyremaplinux::KozikowLayoutRemapper(500);
  vector<pthread_t> threads;
  for (string device : devices) {
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
