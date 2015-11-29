#include "kozikow_layout_remapper.h"
#include "../util/logging.h"

namespace keyremaplinux {

using namespace std;

vector<input_event> KozikowLayoutRemapper::Remap(input_event event) {
  vector<input_event> result;
  if (event.type == EV_KEY || event.type == EV_SYN) {
    LOG(INFO) << "Got event type: " << event.type << ' ' <<
      " code: " << event.code << " value: " << event.value;
    event.code += 1;
    result.push_back(event);
    return result;
  }
  return result;
}

}  // end namespace keyremaplinux
