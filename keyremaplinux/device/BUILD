package(default_visibility = ["//keyremaplinux:internal"])

licenses(["notice"])  # Apache 2.0

exports_files(["LICENSE"])

cc_library(
    name = "device",
    hdrs = [
        "input_device.h",
        "output_device.h",
    ],
    srcs = [
        "input_device.cc",
        "output_device.cc",
    ],
    deps = [
      "//keyremaplinux/util:logging",
    ],
)
