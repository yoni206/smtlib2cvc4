CVC4_HOME="/home/yoniz/git/CVC4"
CXXFLAGS=-no-pie

smtlib2cvc4.out: smtlib2cvc4.cpp
	$(CXX) $(CXXFLAGS) $(DEBUG) -std=c++11 -I$(CVC4_HOME)/src -I$(CVC4_HOME)/src/include -I$(CVC4_HOME)/builds/debug-proofs/src -I$(CVC4_HOME)/builds/debug-proofs/src/include -o smtlib2cvc4.out smtlib2cvc4.cpp -L$(CVC4_HOME)/builds/debug-proofs/src -L$(CVC4_HOME)/builds/debug-proofs/src/parser -lcvc4 -lcvc4parser -lgmp -D__BUILDING_CVC4PARSERLIB

clean:
	rm -rf *.out
