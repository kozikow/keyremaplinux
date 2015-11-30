#ifndef KEYREMAPLINUX_REMAPPER_KOZIKOW_LAYOUT_REMAPPER
#define KEYREMAPLINUX_REMAPPER_KOZIKOW_LAYOUT_REMAPPER

#include <vector>
#include <linux/input.h>
#include <chrono>
#include <map>
#include <string.h>
#include <vector>

#include "remapper.h"

namespace keyremaplinux {

using namespace std;
using namespace std::chrono;

class KozikowLayoutRemapper : public Remapper {
public:
  KozikowLayoutRemapper(int modifierTimeoutMillis);

  virtual ~KozikowLayoutRemapper() {};

  virtual std::vector<input_event> Remap(input_event event) override;
  
private:
  input_event LayerOnRemap(input_event event);

  vector<input_event> ModifierOrKeyPress(input_event event, int pressEventCode);

  static input_event KeyPressEvent(int eventCode);

  static void WrapInShift(vector<input_event>&);

  int modifierTimeoutMillis_;

  bool layerOn_ = false;

  bool keyPressedSinceModifier_ = false;
  
  high_resolution_clock::time_point modifierPressTime_[KEY_MAX];

  int layerRemap_[KEY_MAX+1];
};

}  // end namespace keyremaplinux

#endif  // KEYREMAPLINUX_REMAPPER_KOZIKOW_LAYOUT_REMAPPER
