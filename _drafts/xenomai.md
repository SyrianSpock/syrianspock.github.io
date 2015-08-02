---
layout: post
title: Xenomai installation on a Beaglebone black running Debian
categories: [Linux]
tags: [xenomai, installation, beaglebone, beaglebone black, debian]
fullview: true
---

**[Xenomai](http://xenomai.org/)** is a useful tool for running real-time tasks on an embedded Linux platform such as the Beaglebone black.
That comes in handy when building robotics applications.

A standard **Linux** installation can already be enough if you want to be a soft real-time application, with latency in the order of magnitude of a millisecond you could probably spin a loop at 100Hz easily.
Xenomai allows you to go the extra mile: once installed, it runs a separate kernel from the Linux one that is capable of bringing latency down to the microsecond.
Also the jitter decreases drastically thus allowing hard real-time applications to be built.

For the last few days, I've been trying to install Xenomai on a Beaglebone black.
I found a few links but a lot of them were outdated and didn't work for me as they were [\[1\]](1) [\[2\]](2) [\[3\]](3).
So here's my adaptation or synthesis of all that.

There are two ways you can install Xenomai on the Beaglebone black:
* the hard way which is most flexible but requires patching and recompiling the Linux kernel for the platform
* the easy way which is less flexible but uses a precompiled kernel with the Xenomai patches

### Xenomai installation: the hard way

### Xenomai installation: the easy way

[1]: Installing Xenomai on Beaglebone using Debian distribution from Random Linux Hacks "https://randomlinuxhacks.wordpress.com/2015/02/22/installing-xenomai-on-beaglebone-using-debian-distribution/"
[2]: Cross compiling kernels from Boundary devices "http://boundarydevices.com/cross-compiling-kernels-2014-edition-2/"
[3]: EBC Xenomai page from eLinux "http://elinux.org/EBC_Xenomai"
