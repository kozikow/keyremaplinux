#include "kozikow_layout_remapper.h"

#include <chrono>
#include <ctime>
#include <ratio>
#include <vector>

#include "keyremaplinux/util/logging.h"

namespace keyremaplinux {

KozikowLayoutRemapper::KozikowLayoutRemapper(int modifierTimeoutMillis, KeyboardType keyboardType) :
  modifierTimeoutMillis_(modifierTimeoutMillis), keyboardType_(keyboardType) {
  struct input_event ev;
  shiftDownEvent_ = (input_event*) malloc(sizeof(ev));
  shiftUpEvent_ = (input_event*) malloc(sizeof(ev));
  reusableEvent1_ = (input_event*) malloc(sizeof(ev));
  reusableEvent2_ = (input_event*) malloc(sizeof(ev));
  memset(&ev, 0, sizeof(ev));
  memset(reusableEvent1_, 0, sizeof(ev));
  memset(reusableEvent2_, 0, sizeof(ev));
  ev.type = EV_KEY;
  ev.code = KEY_LEFTSHIFT;
  ev.value = 1;
  *shiftDownEvent_ = ev;
  *shiftUpEvent_ = ev;
  shiftUpEvent_->value = 0;
}

std::vector<input_event*> KozikowLayoutRemapper::Remap(input_event* event) {
  std::vector<input_event*> result;
  if (event->type == EV_KEY) {
    switch (event->code) {
      case KEY_RIGHTMETA:
        if (keyboardType_ == mac) {
            layerOn_ = event->value;
            return result;
        }
      case KEY_RIGHTALT:
        if (keyboardType_ == standard) {
            layerOn_ = event->value;
            return result;
        }
      case KEY_LEFTALT:
        if (keyboardType_ == standard) {
          event->code = KEY_LEFTMETA;
        }
        result.push_back(event);
        return result;
      case KEY_LEFTMETA:
        if (keyboardType_ == standard) {
          event->code = KEY_LEFTALT;
        }
        result.push_back(event);
        return result;
      case KEY_SYSRQ:
        // It is what prtscrn on Thinkpad Carbon gets registered as.
        if (keyboardType_ == standard) {
          event->code = KEY_RIGHTALT;
        }
        result.push_back(event);
        return result;
      case KEY_CAPSLOCK:
        event->code = KEY_LEFTCTRL;
        return ModifierOrKeyPress(event, KEY_ESC);
      case KEY_ENTER: 
        event->code = KEY_RIGHTCTRL;
        return ModifierOrKeyPress(event, KEY_ENTER);
      default:
        keyPressedSinceModifier_ = true;
        if (layerOn_) {
          int oldCode = event->code;
          result.push_back(LayerOnRemap(event));
          if (oldCode == KEY_M || oldCode == KEY_D || oldCode == KEY_F) {
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
  
std::vector<input_event*> KozikowLayoutRemapper::ModifierOrKeyPress(input_event* event, 
    int pressEventCode) {
  std::vector<input_event*> result;
  result.push_back(event);
  if (event->value == 1) {
    modifierPressTime_ = std::chrono::high_resolution_clock::now();
    keyPressedSinceModifier_ = false;
  } else if (event->value == 0) {
    if (!keyPressedSinceModifier_) {
      if (ModifierRecentlyPressed(event->code)) {
        reusableEvent1_->value = 1;
        reusableEvent1_->type = EV_KEY;
        reusableEvent1_->code = pressEventCode;
        result.push_back(reusableEvent1_);
        *reusableEvent2_ = *reusableEvent1_;
        reusableEvent2_->value = 0;
        result.push_back(reusableEvent2_);
      }
    }
  }
  return result;
}

bool KozikowLayoutRemapper::ModifierRecentlyPressed(int keyCode) {
  auto elapsed = (std::chrono::high_resolution_clock::now() -
      modifierPressTime_);
  std::chrono::milliseconds elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds> (elapsed);
  return elapsedMs.count() < modifierTimeoutMillis_;
}

int KozikowLayoutRemapper::GetRemapCode(int code) {
  // Initially I had an array that I used for lookup.
  // It was too slow.
  if (code <= KEY_P) {
    if (code <= KEY_T) {
      switch(code) {
      case KEY_Q:
        return KEY_1;
      case KEY_W:
        return KEY_2;
      case KEY_E:
        return KEY_3;
      case KEY_R:
        return KEY_4;
      case KEY_T:
        return KEY_5;
      }
    } else {
      switch(code) {
      case KEY_Y:
        return KEY_6;
      case KEY_U:
        return KEY_7;
      case KEY_I:
        return KEY_8;
      case KEY_O:
        return KEY_9;
      case KEY_P:
        return KEY_0;
      }
    }
  } else if (code <= KEY_APOSTROPHE) {
    if (code <= KEY_H) {
      switch(code) {
      case KEY_H:
        return KEY_LEFT;
      case KEY_A:
        return KEY_KPLEFTPAREN;
      case KEY_S:
        return KEY_KPRIGHTPAREN;
      case KEY_D:
        return KEY_LEFTBRACE;
      case KEY_F:
        return KEY_RIGHTBRACE;
      case KEY_G:
        return KEY_GRAVE;
      }
    } else {
      switch(code) {
      case KEY_J:
        return KEY_DOWN;
      case KEY_K:
        return KEY_UP;
      case KEY_L:
        return KEY_RIGHT;
      case KEY_SEMICOLON:
        return KEY_EQUAL;
      case KEY_APOSTROPHE:
        return KEY_BACKSLASH;
      }
    }
  }
  else {
    switch (code) {
    case KEY_Z:
      return KEY_LEFTBRACE;
    case KEY_X:
      return KEY_RIGHTBRACE;
    case KEY_C:
      return KEY_MINUS;
    case KEY_V:
      return KEY_KPPLUS;
    case KEY_M:
      return KEY_MINUS;
    }
  }
  return code;
}
  
input_event* KozikowLayoutRemapper::LayerOnRemap(input_event* event) {
  event->code = GetRemapCode(event->code);
  return event;
}

void KozikowLayoutRemapper::WrapInShift(std::vector<input_event*>& events) {
  events.insert(events.begin(), shiftDownEvent_);
  events.push_back(shiftUpEvent_);
}

}  // end namespace keyremaplinux
