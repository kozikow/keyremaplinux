#ifndef KEYREMAPLINUX_REMAPPER_REMAPPER
#define KEYREMAPLINUX_REMAPPER_REMAPPER

#include <vector>
#include <linux/input.h>

namespace keyremaplinux {

using namespace std;

class Remapper {
public:
  Remapper();

  virtual ~Remapper();

  virtual vector<input_event> remap(input_event event) = 0;

private:

  Remapper(const Remapper&) = delete;

  void operator=(const Remapper&) = delete;
};

}  // end namespace keyremaplinux

#endif  // KEYREMAPLINUX_REMAPPER_REMAPPER
