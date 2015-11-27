#include "kozikow_layout_remapper.h"
#include "../util/logging.h"

namespace keyremaplinux {

using namespace std;

vector<input_event> KozikowLayoutRemapper::Remap(input_event event) {
  LOG(INFO) << "Got event type: " << event.type << ' ' <<
    " code: " << event.code << " value: " << event.value;
  vector<input_event> result;
  result.push_back(event);
  return result;
}

}  // end namespace keyremaplinux
