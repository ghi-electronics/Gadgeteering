STARTDIR=${PWD}
cd ../../

OUTDIR=${PWD}/dist

if [ -d "$OUTDIR" ]; then
  rm -rf $OUTDIR
fi

cd build/gcc-64/libgadgeteering
make

cd ../libgmbFEZLynx
make

cd ../libgmpGHI
make

cd ../libgmpSeeed
make

cd ../../../

cd core
find . -name '*.h' | cpio -pdm ${OUTDIR}/include/core
find . -name '*.hpp' | cpio -pdm ${OUTDIR}/include/core

cd ../mainboards
find . -name '*.h' | cpio -pdm ${OUTDIR}/include/mainboards

cd ghi/FEZLynxS4/lib/linux
find . -name '*.a' | cpio -pdm ${OUTDIR}/lib
find . -name '*.so*' | cpio -pdm ${OUTDIR}/lib

cd ../../../../../modules
find . -name '*.h' | cpio -pdm ${OUTDIR}/include/modules

cd ../bin/linux/
find . -name '*.a' | cpio -pdm ${OUTDIR}/lib
