What is VDK?
===========

VDK kernel is part of the Via operating system for use. VDK is an acronym for Via Dynamic Kernel.
VDK is a hybrid kernel combining the MXOS kernel with components from zOS developed at XICreations and a C API for writing apps called Via32.
VDK runs on x86 for single processor configurations.

VDK Source Tree
===============
  * `libsa` - Kernel source code, standard LibC
  * `libkern` - Drivers & IOKit for VDK, IOKit is a Kit for developing drivers for VDK.
  * `libsyscall` - Syscalls for VDK.
  * `osfs` - File system for VDK.
  * `tools` - Utilities for VDK.

How to build VDK
================

Building `DEVELOPMENT` kernel
-----------------------------

To build a kernel for the x86 architecture, just type

    $ make

Additionally, there is support for cleaning to avoid overwrites.

    $ make clean

Hint:
For a more satisfying kernel debugging experience, with access to everything, use BOCHS.

Changes from VDK-old
===============
- Custom Bootloader (VDK Loader)
- FAT & RIFS Cross-Support
