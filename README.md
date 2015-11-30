# keyremaplinux
Work in progress.

Define your keyboard layout as C++ class.

To run on linux:
```
sudo kill -9 $(ps aux | grep [k]eyremaplinux | awk '{print $2}')
bazel build //keyremaplinux:keyremaplinux
sudo ./bazel-bin/keyremaplinux/keyremaplinux 
```

