<div id="table-of-contents">
<h2>Table of Contents</h2>
<div id="text-table-of-contents">
<ul>
<li><a href="#orgheadline1">1. Introduction</a></li>
<li><a href="#orgheadline4">2. Running on Linux</a>
<ul>
<li><a href="#orgheadline2">2.1. Kill existing bazel</a></li>
<li><a href="#orgheadline3">2.2. Start bazel with extra paramenters for higher cpu priority</a></li>
</ul>
</li>
<li><a href="#orgheadline9">3. Installation</a>
<ul>
<li><a href="#orgheadline7">3.1. Install bazel</a>
<ul>
<li><a href="#orgheadline5">3.1.1. Mac OS X</a></li>
<li><a href="#orgheadline6">3.1.2. From source</a></li>
</ul>
</li>
<li><a href="#orgheadline8">3.2. Build keyremaplinux</a></li>
</ul>
</li>
<li><a href="#orgheadline10">4. Run tests</a></li>
</ul>
</div>
</div>

# Introduction<a id="orgheadline1"></a>

Define your keyboard layout as C++ class.
Primarily focused on implementing [my keyboard layout](https://kozikow.wordpress.com/2013/11/15/the-only-alternative-keyboard-layout-youll-ever-need-as-a-programmer/).

# Running on Linux<a id="orgheadline4"></a>

## Kill existing bazel<a id="orgheadline2"></a>

    sudo kill -9 $(ps aux | grep [k]eyremaplinux | awk '{print $2}')

## Start bazel with extra paramenters for higher cpu priority<a id="orgheadline3"></a>

    sudo nice -n -20 ./bazel-bin/keyremaplinux/keyremaplinux

# Installation<a id="orgheadline9"></a>

## Install bazel<a id="orgheadline7"></a>

### Mac OS X<a id="orgheadline5"></a>

    brew install bazel

### From source<a id="orgheadline6"></a>

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

## Build keyremaplinux<a id="orgheadline8"></a>

    git clone https://github.com/kozikow/keyremaplinux ~/keyremaplinux
    cd ~/keyremaplinux
    bazel build //keyremaplinux:keyremaplinux

# Run tests<a id="orgheadline10"></a>

    bazel test //keyremaplinux:all --test_output=errors
