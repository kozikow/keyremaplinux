#include <chrono>
#include <ctime>
#include <ratio>
#include <chrono>

#include "kozikow_layout_remapper.h"
#include "../util/logging.h"

namespace keyremaplinux {

using namespace std;
using namespace std::chrono;

vector<input_event> KozikowLayoutRemapper::Remap(input_event event) {
  vector<input_event> result;
  LOG(INFO) << "Got event type: " << event.type << ' ' <<
      " code: " << event.code << " value: " << event.value;
  if (event.type == EV_KEY) {
    switch (event.code) {
      case KEY_RIGHTMETA:
        layerOn_ = event.value;
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
        } else {
          result.push_back(event);
        }
    }
  }
  return result;
}
  
vector<input_event> KozikowLayoutRemapper::ModifierOrKeyPress(input_event event, 
    int pressEventCode) {
  vector<input_event> result;
  result.push_back(event);
  if (event.value == 1) {
    LOG(INFO) << "Pressing " << event.code;
    modifierPressTime_[event.code] = high_resolution_clock::now();
    keyPressedSinceModifier_ = false;
  } else if (event.value == 0) {
    if (!keyPressedSinceModifier_) {
      input_event pressEvent = KeyPressEvent(pressEventCode);
      result.push_back(pressEvent);
      pressEvent.value = 0;
      result.push_back(pressEvent);
    }
  }
  return result;
}
  
input_event KozikowLayoutRemapper::LayerOnRemap(input_event event) {
  LOG(INFO) << "GOT CODE " << event.code;
  switch (event.code) {
    case KEY_A:
      event.code = KEY_KPLEFTPAREN;
      break;
    case KEY_H:
      event.code = KEY_LEFT;
      break;
    case KEY_J:
      event.code = KEY_DOWN;
      break;
    case KEY_K:
      event.code = KEY_UP;
      break;
    case KEY_L:
      event.code = KEY_RIGHT;
      break;
    case KEY_SEMICOLON:
      event.code = KEY_EQUAL;
      break;
    case KEY_Q:
      event.code = KEY_1;
      break;
    case KEY_W:
      event.code = KEY_2;
      break;
    case KEY_E:
      event.code = KEY_3;
      break;
    case KEY_R:
      event.code = KEY_4;
      break;
    case KEY_T:
      event.code = KEY_5;
      break;
    case KEY_Y:
      event.code = KEY_6;
      break;
    case KEY_U:
      event.code = KEY_7;
      break;
    case KEY_I:
      event.code = KEY_8;
      break;
    case KEY_O:
      event.code = KEY_9;
      break;
    case KEY_P:
      event.code = KEY_0;
      break;
  }
  return event;
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
