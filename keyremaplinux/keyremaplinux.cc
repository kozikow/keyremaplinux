#include <iostream>
#include <vector>
#include <cstdio>
#include <string>
#include <dirent.h> 
#include <limits.h>
#include <set>

#include "keyremaplinux/remapper/remapper.h"
#include "keyremaplinux/remapper/kozikow_layout_remapper.h"
#include "device_remapping_daemon.h"
#include "util/logging.h"

namespace keyremaplinux {

  using namespace std;

  static const string dirNames[] = {"/dev/input/by-id", "/dev/input/by-path"};

  static const string keyboardSubstrings[] = {"kbd", "Keyboard"};

  string ResolveSymlink(string path) {
    char buf[PATH_MAX + 1];
    char *res = realpath(path.c_str(), buf);
    CHECK(res > 0);
    return res;
  }

  bool LooksLikeKeyboardPath(string path) {
    for (string substr : keyboardSubstrings) {
      if (path.find(substr) != string::npos) {
        return true;
      }
    }
    return false;
  }

  set<string> FindKeyboardDevices() {
    set<string> paths;
    DIR *d;
    struct dirent *path;
    for (string dirName : dirNames) {
      d = opendir(dirName.c_str());
      if (d) {
        while ((path = readdir(d)) != NULL) {
          if (LooksLikeKeyboardPath(path->d_name)) {
            paths.insert(ResolveSymlink(dirName + "/" + path->d_name));
          }
        }
        closedir(d);
      }
    }
    return paths;
  }

}  // end namespace keyremaplinux

int main(int argc, char* argv[]) {
  using namespace std;
  set<string> devices = keyremaplinux::FindKeyboardDevices();
  if (devices.empty()) {
    LOG(WARNING) << "Did not find any input devices";
  }

  keyremaplinux::Remapper* remapper = new keyremaplinux::KozikowLayoutRemapper();
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
