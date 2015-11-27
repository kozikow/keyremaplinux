FROM ubuntu:14.04

MAINTAINER Robert Kozikowski <r.kozikowski@gmail.com>

RUN useradd kozikow --create-home

RUN apt-get update && apt-get dist-upgrade -y

RUN apt-get install -y software-properties-common python-software-properties

# C++
RUN apt-get install -y g++

# Java 8
RUN echo debconf shared/accepted-oracle-license-v1-1 select true | debconf-set-selections
RUN add-apt-repository -y ppa:webupd8team/java
RUN sudo apt-get update
RUN apt-get install -y oracle-java8-installer

# Bazel dependencies
RUN apt-get install -y pkg-config zip zlib1g-dev unzip git

# Build bazel
RUN git clone https://github.com/google/bazel.git /bazel && cd /bazel
RUN /bazel/compile.sh
RUN cp /bazel/bazel-bin/src/bazel /usr/local/bin

# Build keyremaplinux
RUN git clone https://github.com/kozikow/keyremaplinux /keyremaplinux
RUN cd /keyremaplinux

# Uncomment soon:
# RUN bazel build //keyremaplinux:keyremaplinux

