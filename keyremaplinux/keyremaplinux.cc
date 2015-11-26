#include <iostream>
#include <vector>
#include <cstdio>
#include <string>
#include <dirent.h> 

#include "keyremaplinux/remapper/remapper.h"
#include "device_remapping_daemon.h"

namespace keyremaplinux {

  using namespace std;

  static const string deviceRoot = "/dev/input/by-id";

  static const string keyboardSubstr = "event-kbd";

  vector<string> FindKeyboardDevices() {
    vector<string> result;
    DIR *d;
    struct dirent *dir;
    d = opendir(deviceRoot.c_str());
    if (d) {
      while ((dir = readdir(d)) != NULL) {
        if (((string)dir->d_name).find(keyboardSubstr) != string::npos) {
          result.push_back(dir->d_name);
        }
      }
      closedir(d);
    }
    return result;
  }

}  // end namespace keyremaplinux

int main(int argc, char* argv[]) {
  std::vector<std::string> devices = keyremaplinux::FindKeyboardDevices();
  if (devices.empty()) {
    LOG(WARNING) << "Did not find any input devices";
  }
  for (int i=0; i<devices.size(); i++) {
    std::cout << devices[i] << std::endl;
  }
}
