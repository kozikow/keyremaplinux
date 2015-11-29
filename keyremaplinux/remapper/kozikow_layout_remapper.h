#ifndef KEYREMAPLINUX_REMAPPER_KOZIKOW_LAYOUT_REMAPPER
#define KEYREMAPLINUX_REMAPPER_KOZIKOW_LAYOUT_REMAPPER

#include <vector>
#include <linux/input.h>
#include <chrono>
#include <map>
#include <string.h>

#include "remapper.h"

namespace keyremaplinux {

using namespace std;
using namespace std::chrono;

class KozikowLayoutRemapper : public Remapper {
public:
  using Remapper::Remapper;

  virtual ~KozikowLayoutRemapper() {};

  virtual std::vector<input_event> Remap(input_event event) override;
  
private:
  input_event LayerOnRemap(input_event event);

  vector<input_event> ModifierOrKeyPress(input_event event, int pressEventCode);

  static input_event KeyPressEvent(int eventCode);

  bool layerOn_ = false;

  bool keyPressedSinceModifier_ = false;
  
  map<int, high_resolution_clock::time_point> modifierPressTime_;
};

}  // end namespace keyremaplinux

#endif  // KEYREMAPLINUX_REMAPPER_KOZIKOW_LAYOUT_REMAPPER
