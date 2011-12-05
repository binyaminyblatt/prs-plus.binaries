#!/bin/sh
OLD_PATH=$PATH
export PATH=/media/big/sorcery.600/bin:$OLD_PATH
rm unrar *.o unrar.600
make -f makefile.600 && \
mv unrar unrar.600
export PATH=/opt/cross/gcc-3.2.3-glibc-2.2.5/arm-unknown-linux-gnu/arm-unknown-linux-gnu/bin:$OLD_PATH
rm unrar *.o unrar.300
make -f makefile.300 && \
mv unrar unrar.300

