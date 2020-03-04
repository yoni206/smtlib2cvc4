#!/bin/bash
./smtlib2cvc4.sh tests/test.smt2 cvc4 >> CVC4/src/preprocessing/passes/int_to_bv.cpp
cd CVC4/build
make -j16 | grep err
git checkout -- ../src/preprocessing/passes/int_to_bv.cpp
cd ../../

./smtlib2cvc4.sh tests/test_int.smt2 smt-switch >> smt-switch/src/identity_walker.cpp
cd smt-switch/build
make | grep err
git checkout -- ../src/identity_walker.cpp
cd ../../
