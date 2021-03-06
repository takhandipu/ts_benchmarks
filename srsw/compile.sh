set -x
clang++ -c -emit-llvm srsw.cpp -o a.bc
opt -load ~/Documents/llvm/build/lib/LLVMInstrumenter.so -instrumenter < a.bc > a.inst.bc 2> a.log
llc -filetype=obj a.inst.bc -o a.o
clang++ a.o -Wl,/home/takh/eigen-optim/libruntime.so -lpthread -o a.out
