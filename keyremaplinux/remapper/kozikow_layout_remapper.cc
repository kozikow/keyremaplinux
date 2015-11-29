#include "kozikow_layout_remapper.h"
#include "../util/logging.h"

namespace keyremaplinux {

using namespace std;

vector<input_event> KozikowLayoutRemapper::Remap(input_event event) {
  vector<input_event> result;
  if (event.type == EV_KEY) {
    LOG(INFO) << "Got event type: " << event.type << ' ' <<
      " code: " << event.code << " value: " << event.value;
    if (event.code == KEY_D) {
      event.code += 1;
    }
    result.push_back(event);
    return result;
  } else {
    LOG(INFO) << "Unrecognized event type: " << event.type;
  }
  return result;
}

}  // end namespace keyremaplinux
