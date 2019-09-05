#!/bin/bash

#get a copy of cvc4
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

#make the script runable
chmod 755 smtlib2cvc4.sh
