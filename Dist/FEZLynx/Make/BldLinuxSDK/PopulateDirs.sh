#!/bin/sh
# cmd line error checking
if [ $# -ne "1" ]
then
   echo "usage: "`basename "$0"`" <absolutepath to gadeteering directory ( /..../gadgeteering )>"
   exit 101
fi
if [ ! -d "$1" ]
then
   echo "usage: "`basename "$0"`" <absolutepath to gadeteering directory ( /..../gadgeteering ) >"
   exit 102
fi
# do it

export REPODIR="$1"

jdo()
{
    
    find "$REPODIR/$1" -name "$3""*.h"   -exec cp {} "base/include/$2/" \;
    find "$REPODIR/$1" -name "$3""*.cpp" -exec cp {} "base/src/$2/" \;
    echo $1
}
jdo Core Core
# leave FEZ Medusa boards out of FEZLynx development ...
jdo Mainboards Mainboards FEZL
jdo Modules Modules
#special case, because this file is generated base on ../Core/*.h, but is kept one directory above
# Core, Modules, and Mainboards
cp $REPODIR/Gadgeteering.h base/include/
# more special cases. We keep FTDI distributions in it's own directory so it doesn't take a lot of work
# to clean it out of your other trees and so new releases are easy to install, we need a couple of include
# files from the root of the FTDI release and the library(s) themself:
cp $REPODIR/libftd2xx1.1.12/release/*.h  base/include/
cp $REPODIR/libftd2xx1.1.12/release/build/i386/*.a  base/lib/
