package(default_visibility = ["//keyremaplinux:internal"])

licenses(["notice"])  # Apache 2.0

exports_files(["LICENSE"])

cc_library(
    name = "remapper",
    hdrs = [
        "remapper.h",
        "keyboard_type.h",
        "kozikow_layout_remapper.h",
    ],
    srcs = [
        "kozikow_layout_remapper.cc"
    ],
    deps = [
      "//keyremaplinux/util:logging",
    ],
)
