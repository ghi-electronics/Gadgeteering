STARTDIR=${PWD}
cd ../../

#Set 64 bit distribution package
OUTNAME=Gadgeteering-$(date +"%Y%m%d")-linux-x86
OUTDIR=${PWD}/dist/Gadgeteering-$(date +"%Y%m%d")-linux-x86

if [ -d "$OUTDIR" ]; then
  cd ${OUTDIR}/../
  rm -rf $OUTNAME
  cd ../
fi

#Build 64 bit distribution package

cd mainboards/ghi/FEZLynxS4/lib/linux/i386
find . -name '*.a' | cpio -pdm ../../../../../../bin/linux/x86
find . -name '*.so*' | cpio -pdm ../../../../../../bin/linux/x86

cd ../../../../../../build/gcc-lin/libgadgeteering
make arch=32

cd ../libgmbFEZLynx
make arch=32 platform=lin
rm -rf obj

cd ../libgmpGHI
make arch=32 platform=lin
rm -rf obj

cd ../libgmpSeeed
make arch=32 platform=lin
rm -rf obj

cd ../../../

cd core
find . -name '*.h' | cpio -pdm ${OUTDIR}/include/core
find . -name '*.hpp' | cpio -pdm ${OUTDIR}/include/core

cd ../mainboards
find . -name '*.h' | cpio -pdm ${OUTDIR}/include/mainboards

cd ../modules
find . -name '*.h' | cpio -pdm ${OUTDIR}/include/modules

cd ../bin/linux/x86/
find . -name '*.a' | cpio -pdm ${OUTDIR}/lib
find . -name '*.so*' | cpio -pdm ${OUTDIR}/lib

cd ../../../

rm -rf bin/linux/x86

cd ${OUTDIR}/../

tar -zcvf ${OUTNAME}.tar.gz ${OUTNAME}
rm -rf ${OUTNAME}
