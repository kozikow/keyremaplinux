# keyremaplinux
Work in progress.

Define your keyboard layout as C++ class.

To test:
```
bazel test //keyremaplinux:all --test_output=errors
```

To run on linux:
```
sudo kill -9 $(ps aux | grep [k]eyremaplinux | awk '{print $2}')
bazel build //keyremaplinux:keyremaplinux
sudo nice -n -20 ./bazel-bin/keyremaplinux/keyremaplinux 
```

Install instructions:
```
apt-get install -y software-properties-common python-software-properties

# C++
apt-get install -y g++

# Java 8
echo debconf shared/accepted-oracle-license-v1-1 select true | debconf-set-selections
add-apt-repository -y ppa:webupd8team/java
sudo apt-get update
apt-get install -y oracle-java8-installer

# Bazel dependencies
apt-get install -y pkg-config zip zlib1g-dev unzip git

# Build bazel
git clone https://github.com/google/bazel.git ~/bazel && cd ~/bazel
~/bazel/compile.sh
sudo cp ~/bazel/bazel-bin/src/bazel /usr/local/bin

# Build keyremaplinux
git clone https://github.com/kozikow/keyremaplinux ~/keyremaplinux
cd ~/keyremaplinux
bazel build //keyremaplinux:keyremaplinux
```
