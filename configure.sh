#!/bin/bash
rm -rf CVC4
git clone git@github.com:CVC4/CVC4.git
cd CVC4
cvc4_path=`pwd`
sed "s|<non-existing-path>|$cvc4_path|g" "../Makefile.template" > ../Makefile
contrib/get-antlr-3.4
./configure.sh debug
cd build
make -j16
cd ../../
