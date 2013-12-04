#!/bin/sh
# bin & build are created by Makefile if they don't exist, they are not created by installer
# lib is created for 3rd party libraries distributed with product (currently Lynx none, Medusa FTDI)
# de-facto for Unix and Linux is to pretty much use all lower case for directories, we leave subdirs
#   with caps, so code is portable to Windows set-up
mkdir base base/doc base/include base/src base/lib base/test base/examples
(
cd base
mkdir src/Core src/Mainboards src/Modules
mkdir include/Core include/Mainboards include/Modules
)
