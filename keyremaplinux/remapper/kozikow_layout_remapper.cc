#include <linux/input.h>

#include "remapper.h"

namespace keyremaplinux {

using namespace std;

class KozikowLayoutRemapper : Remapper {
public:
  vector<input_event> remap(input_event event) {
    vector<input_event> result;
    result.push_back(event);
    return result;
  }
};

}  // end namespace keyremaplinux
