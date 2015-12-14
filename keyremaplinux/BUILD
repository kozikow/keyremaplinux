package(default_visibility = ["//visibility:public"])

licenses(["notice"])  # Apache 2.0

exports_files([
    "LICENSE",
])

package_group(
    name = "internal",
    packages = ["//keyremaplinux/..."],
)

cc_binary(
    name = "keyremaplinux",
    srcs = [
      "device_remapping_daemon.cc",
      "device_remapping_daemon.h",
      "keyremaplinux.cc",
    ],
    copts = [
        "-std=c++11",
        "-O2",
    ],
    linkopts = ["-pthread"],
    deps = [
      "//keyremaplinux/remapper:remapper",
      "//keyremaplinux/util:logging",
      "//keyremaplinux/device:device",
    ],
)
