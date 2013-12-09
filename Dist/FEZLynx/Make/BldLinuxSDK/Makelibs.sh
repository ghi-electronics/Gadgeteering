#!/bin/sh
set -v -x

export BLDBASE="`pwd`"
export INC_DIR="$BLDBASE/base/include"
export FEZLYNX_INCLUDE_PATH="-I $INC_DIR -I $INC_DIR/Mainboards -I $INC_DIR/Core -I $INC_DIR/Modules"

jdo() {
    cd $BLDBASE/base/src/$1
#-m32 -march=i486
    g++ -g -O1   -c $FEZLYNX_INCLUDE_PATH $BLDBASE/base/src/$1/*.cpp   
    ar -rc "$BLDBASE/base/lib/lib$2.a" $BLDBASE/base/src/$1/*.o
    rm $BLDBASE/base/src/$1/*.o
    echo $1
}
jdo Mainboards mainboards
jdo Core core
jdo Modules modules
