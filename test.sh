#!/bin/bash
./smtlib2cvc4.sh tests/test_qf_lia.smt2 QF_LIA  >> CVC4/src/preprocessing/passes/int_to_bv.cpp
cd CVC4/build
make -j16
git checkout -- ../src/preprocessing/passes/int_to_bv.cpp
cd ../../

