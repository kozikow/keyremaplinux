#include "kozikow_layout_remapper.h"

#include <chrono>
#include <ctime>
#include <ratio>
#include <vector>

#include "keyremaplinux/util/logging.h"

namespace keyremaplinux {

KozikowLayoutRemapper::KozikowLayoutRemapper(int modifierTimeoutMillis, KeyboardType keyboardType) :
  modifierTimeoutMillis_(modifierTimeoutMillis), keyboardType_(keyboardType) {
  for (int i=0; i<=KEY_MAX; i++) {
    layerRemap_[i] = i;
  }

  // First row
  layerRemap_[KEY_Q] = KEY_1;
  layerRemap_[KEY_W] = KEY_2;
  layerRemap_[KEY_E] = KEY_3;
  layerRemap_[KEY_R] = KEY_4;
  layerRemap_[KEY_T] = KEY_5;
  layerRemap_[KEY_Y] = KEY_6;
  layerRemap_[KEY_U] = KEY_7;
  layerRemap_[KEY_I] = KEY_8;
  layerRemap_[KEY_O] = KEY_9;
  layerRemap_[KEY_P] = KEY_0;

  // second row
  layerRemap_[KEY_A] = KEY_KPLEFTPAREN;
  layerRemap_[KEY_S] = KEY_KPRIGHTPAREN;
  layerRemap_[KEY_D] = KEY_LEFTBRACE;
  layerRemap_[KEY_F] = KEY_RIGHTBRACE;
  layerRemap_[KEY_G] = KEY_GRAVE;
  layerRemap_[KEY_H] = KEY_LEFT;
  layerRemap_[KEY_J] = KEY_DOWN;
  layerRemap_[KEY_K] = KEY_UP;
  layerRemap_[KEY_L] = KEY_RIGHT;
  layerRemap_[KEY_SEMICOLON] = KEY_EQUAL;
  layerRemap_[KEY_APOSTROPHE] = KEY_BACKSLASH;

  // Third row
  layerRemap_[KEY_Z] = KEY_LEFTBRACE;
  layerRemap_[KEY_X] = KEY_RIGHTBRACE;
  layerRemap_[KEY_C] = KEY_MINUS;
  layerRemap_[KEY_V] = KEY_KPPLUS;
  layerRemap_[KEY_M] = KEY_MINUS;
}

std::vector<input_event> KozikowLayoutRemapper::Remap(input_event event) {
  std::vector<input_event> result;
  if (event.type == EV_KEY) {
    switch (event.code) {
      case KEY_RIGHTMETA:
        if (keyboardType_ == mac) {
            layerOn_ = event.value;
            return result;
        }
      case KEY_RIGHTALT:
        if (keyboardType_ == standard) {
            layerOn_ = event.value;
            return result;
        }
      case KEY_LEFTALT:
        if (keyboardType_ == standard) {
          event.code = KEY_LEFTMETA;
        }
        result.push_back(event);
        return result;
      case KEY_LEFTMETA:
        if (keyboardType_ == standard) {
          event.code = KEY_LEFTALT;
        }
        result.push_back(event);
        return result;
      case KEY_SYSRQ:
        if (keyboardType_ == standard) {
          event.code = KEY_RIGHTALT;
        }
        result.push_back(event);
        return result;
      case KEY_CAPSLOCK:
        event.code = KEY_LEFTCTRL;
        return ModifierOrKeyPress(event, KEY_ESC);
      case KEY_ENTER: 
        event.code = KEY_RIGHTCTRL;
        return ModifierOrKeyPress(event, KEY_ENTER);
      default:
        keyPressedSinceModifier_ = true;
        if (layerOn_) {
          result.push_back(LayerOnRemap(event));
          if (event.code == KEY_M || event.code == KEY_D || event.code == KEY_F) {
            WrapInShift(result);
          }
        } else {
          result.push_back(event);
        }
    }
  } else {
    // Only EV_KEY events are relevant. Forward other events.
    result.push_back(event);
  }
  return result;
}
  
std::vector<input_event> KozikowLayoutRemapper::ModifierOrKeyPress(input_event event, 
    int pressEventCode) {
  std::vector<input_event> result;
  result.push_back(event);
  if (event.value == 1) {
    LOG(INFO) << "Pressing " << event.code;
    modifierPressTime_[event.code] = std::chrono::high_resolution_clock::now();
    keyPressedSinceModifier_ = false;
  } else if (event.value == 0) {
    if (!keyPressedSinceModifier_) {
      if (ModifierRecentlyPressed(event.code)) {
        input_event pressEvent = KeyPressEvent(pressEventCode);
        result.push_back(pressEvent);
        pressEvent.value = 0; // key release
        result.push_back(pressEvent);
      }
    }
  }
  return result;
}

bool KozikowLayoutRemapper::ModifierRecentlyPressed(int keyCode) {
  auto elapsed = (std::chrono::high_resolution_clock::now() -
      modifierPressTime_[keyCode]);
  std::chrono::milliseconds elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds> (elapsed);
  LOG(INFO) << "Elapsed duration: " << elapsedMs.count();
  return elapsedMs.count() < modifierTimeoutMillis_;
}
  
input_event KozikowLayoutRemapper::LayerOnRemap(input_event event) {
  LOG(INFO) << "GOT CODE " << event.code;
  event.code = layerRemap_[event.code];
  return event;
}

void KozikowLayoutRemapper::WrapInShift(std::vector<input_event>& events) {
  input_event shiftEvent = KeyPressEvent(KEY_LEFTSHIFT);
  events.insert(events.begin(), shiftEvent);
  shiftEvent.value = 0; // key release
  events.push_back(shiftEvent);
}
  
input_event KozikowLayoutRemapper::KeyPressEvent(int eventCode) {
  input_event ev;
  memset(&ev, 0, sizeof(ev));
  ev.type = EV_KEY;
  ev.code = eventCode;
  ev.value = 1;
  return ev;
}

}  // end namespace keyremaplinux
