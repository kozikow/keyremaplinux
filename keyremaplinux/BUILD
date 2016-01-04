package(default_visibility = ["//visibility:public"])

licenses(["notice"])  # Apache 2.0

exports_files([
    "LICENSE",
])

package_group(
    name = "internal",
    packages = ["//keyremaplinux/..."],
)

cc_library(
    name = "device_remapping_daemon",
    srcs = [
      "device_remapping_daemon.cc",
      "device_remapping_daemon.h",
      "keyremaplinux.cc",
    ],
    deps = [
      "//keyremaplinux/remapper:remapper",
      "//keyremaplinux/util:logging",
      "//keyremaplinux/device:device",
    ],
)

cc_test(
  name='device_remapping_daemon_test',
  srcs=['device_remapping_daemon_test.cc'],
  size='small',
  deps=[
    ':device_remapping_daemon',
    "//external:gtest_main",
  ],
)

cc_binary(
    name = "keyremaplinux",
    srcs = [
      "keyremaplinux.cc",
    ],
    copts = [
        "-std=c++11",
        "-O2",
    ],
    linkopts = ["-pthread"],
    deps = [
      "//keyremaplinux:device_remapping_daemon",
    ],
)

test_suite(
  name="all_tests",
  tests=[
    ':device_remapping_daemon_test',
  ],
)
