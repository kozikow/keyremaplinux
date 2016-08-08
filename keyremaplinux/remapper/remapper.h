#ifndef KEYREMAPLINUX_REMAPPER_REMAPPER
#define KEYREMAPLINUX_REMAPPER_REMAPPER

#include <linux/input.h>
#include <vector>

namespace keyremaplinux {

class Remapper {
public:
  Remapper() {};

  virtual ~Remapper() {};

  virtual std::vector<input_event*> Remap(input_event* event) = 0;

private:

  Remapper(const Remapper&) = delete;

  void operator=(const Remapper&) = delete;
};

}  // end namespace keyremaplinux

#endif  // KEYREMAPLINUX_REMAPPER_REMAPPER
