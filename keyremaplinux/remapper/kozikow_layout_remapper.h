#ifndef KEYREMAPLINUX_REMAPPER_KOZIKOW_LAYOUT_REMAPPER
#define KEYREMAPLINUX_REMAPPER_KOZIKOW_LAYOUT_REMAPPER

#include <chrono>
#include <linux/input.h>
#include <string.h>
#include <map>
#include <vector>

#include "remapper.h"
#include "keyboard_type.h"

namespace keyremaplinux {

class KozikowLayoutRemapper : public Remapper {
public:
  KozikowLayoutRemapper(int modifierTimeoutMillis, KeyboardType keyboardType);

  virtual ~KozikowLayoutRemapper() {};

  virtual std::vector<input_event*> Remap(input_event* event) override;
  
private:
  input_event* LayerOnRemap(input_event* event);

  std::vector<input_event*> ModifierOrKeyPress(input_event* event, int pressEventCode);

  bool ModifierRecentlyPressed(int keyCode);

  static input_event* KeyPressEvent(int eventCode);

  void WrapInShift(std::vector<input_event*>&);


  int modifierTimeoutMillis_;

  KeyboardType keyboardType_;

  bool layerOn_ = false;

  bool keyPressedSinceModifier_ = false;

  input_event* shiftDownEvent_;
  input_event* shiftUpEvent_;

  // It is reusable event struct to avoid allocations.
  input_event* reusableEvent1_;
  input_event* reusableEvent2_;
  
  std::chrono::high_resolution_clock::time_point modifierPressTime_[KEY_MAX];

  int layerRemap_[KEY_MAX+1];
};

}  // end namespace keyremaplinux
#endif  // KEYREMAPLINUX_REMAPPER_KOZIKOW_LAYOUT_REMAPPER
