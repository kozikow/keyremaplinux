#ifndef KEYREMAPLINUX_REMAPPER_KEYBOARD_TYPE
#define KEYREMAPLINUX_REMAPPER_KEYBOARD_TYPE

namespace keyremaplinux {

// Used only by kozikow layout to customize behavior depending on
// keyboard type.
enum KeyboardType {
  mac, // Mac keyboard plugged into Linux. I use Matias Quiet Pro.
  standard // Standard keyboard used by majority of linux users.
};

}  // end namespace keyremaplinux
#endif  // KEYREMAPLINUX_REMAPPER_KEYBOARD_TYPE
