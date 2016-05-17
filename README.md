<div id="table-of-contents">
<h2>Table of Contents</h2>
<div id="text-table-of-contents">
<ul>
<li><a href="#orgheadline1">1. Introduction</a></li>
<li><a href="#orgheadline6">2. Installation</a>
<ul>
<li><a href="#orgheadline4">2.1. Install bazel</a>
<ul>
<li><a href="#orgheadline2">2.1.1. Mac OS X</a></li>
<li><a href="#orgheadline3">2.1.2. Build from source</a></li>
</ul>
</li>
<li><a href="#orgheadline5">2.2. Build keyremaplinux</a></li>
</ul>
</li>
<li><a href="#orgheadline9">3. Running on Linux</a>
<ul>
<li><a href="#orgheadline7">3.1. Kill existing bazel</a></li>
<li><a href="#orgheadline8">3.2. Start bazel with extra paramenters for higher cpu priority</a></li>
</ul>
</li>
<li><a href="#orgheadline14">4. Implementing the new layout</a>
<ul>
<li><a href="#orgheadline10">4.1. Extend Remapper</a></li>
<li><a href="#orgheadline11">4.2. For example KozikowLayoutRemapper</a></li>
<li><a href="#orgheadline12">4.3. Consume and produce Linux input<sub>event</sub> from input.h</a></li>
<li><a href="#orgheadline13">4.4. Event codes are defined in input-event-codes.h</a></li>
</ul>
</li>
<li><a href="#orgheadline15">5. Run tests</a></li>
</ul>
</div>
</div>

# Introduction<a id="orgheadline1"></a>

Define your keyboard layout as C++ class.
Primarily focused on implementing [my keyboard layout](https://kozikow.wordpress.com/2013/11/15/the-only-alternative-keyboard-layout-youll-ever-need-as-a-programmer/).

# Installation<a id="orgheadline6"></a>

## Install bazel<a id="orgheadline4"></a>

### Mac OS X<a id="orgheadline2"></a>

    brew install bazel

### Build from source<a id="orgheadline3"></a>

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

## Build keyremaplinux<a id="orgheadline5"></a>

    git clone https://github.com/kozikow/keyremaplinux ~/keyremaplinux
    cd ~/keyremaplinux
    bazel build //keyremaplinux:keyremaplinux

# Running on Linux<a id="orgheadline9"></a>

## Kill existing bazel<a id="orgheadline7"></a>

    sudo kill -9 $(ps aux | grep [k]eyremaplinux | awk '{print $2}')

## Start bazel with extra paramenters for higher cpu priority<a id="orgheadline8"></a>

    sudo nice -n -20 ./bazel-bin/keyremaplinux/keyremaplinux

# Implementing the new layout<a id="orgheadline14"></a>

## Extend [Remapper](https://github.com/kozikow/keyremaplinux/blob/master/keyremaplinux/remapper/remapper.h)<a id="orgheadline10"></a>

## For example [KozikowLayoutRemapper](https://github.com/kozikow/keyremaplinux/blob/master/keyremaplinux/remapper/kozikow_layout_remapper.h)<a id="orgheadline11"></a>

## Consume and produce [Linux input<sub>event</sub> from input.h](https://github.com/torvalds/linux/blob/master/include/uapi/linux/input.h#L25)<a id="orgheadline12"></a>

## [Event codes are defined in input-event-codes.h](https://github.com/torvalds/linux/blob/master/include/uapi/linux/input-event-codes.h)<a id="orgheadline13"></a>

Rather than reading trying to come up with mapping based on this file,
I recommend break pointing or adding logging in your layout to see correct codes.

Sometimes name of key may surprise you.

# Run tests<a id="orgheadline15"></a>

    bazel test //keyremaplinux:all --test_output=errors
