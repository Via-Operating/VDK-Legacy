What is VDK?
===========

VDK kernel is part of the Via operating system for use. VDK is an acronym for Via Dynamic Kernel.
VDK is a hybrid kernel combining the MXOS kernel with components from NanobyteOS and a C API for writing apps called Via32.
VDK runs on x86 for single processor configurations.

VDK Source Tree
===============
  * `src` - A set of utilities for testing, debugging and profiling kernel.
  * `build` - Where binaries of the kernel are stored.

How to build VDK
================

Building `DEVELOPMENT` kernel
-----------------------------

To build a kernel for the i386 architecture, just type

    $ make

Additionally, there is support for cleaning to avoid overwrites.

    $ make clean

Hint:
For a more satisfying kernel debugging experience, with access to everything, use BOCHS.

Changes from VDK-old
===============
- Custom Bootloader (VDK Loader)
- FAT & RIFS Cross-Support
