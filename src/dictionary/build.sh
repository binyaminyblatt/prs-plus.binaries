#!/bin/sh
OLD_PATH=$PATH
#VERSION=1.0.4na
#export PATH=/media/big/sorcery.600/bin:$OLD_PATH
#make clean && \
#make -f Makefile.600 VERSION=600_${VERSION} && \
#mv unix_dist/fb2toepub fb2toepub.600
export PATH=/opt/cross/gcc-3.2.3-glibc-2.2.5/arm-unknown-linux-gnu/arm-unknown-linux-gnu/bin:$OLD_PATH
make
#make clean && \
#make VERSION=300_${VERSION} && \
#mv unix_dist/fb2toepub fb2toepub.300

