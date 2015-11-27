#ifndef KEYREMAPLINUX_REMAPPER_KOZIKOW_LAYOUT_REMAPPER
#define KEYREMAPLINUX_REMAPPER_KOZIKOW_LAYOUT_REMAPPER

#include <vector>
#include <linux/input.h>

#include "remapper.h"

namespace keyremaplinux {

class KozikowLayoutRemapper : public Remapper {
public:
  using Remapper::Remapper;

  virtual ~KozikowLayoutRemapper() {};

  virtual std::vector<input_event> Remap(input_event event) override;
};

}  // end namespace keyremaplinux

#endif  // KEYREMAPLINUX_REMAPPER_KOZIKOW_LAYOUT_REMAPPER
