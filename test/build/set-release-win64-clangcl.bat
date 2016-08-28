call "%VS140COMNTOOLS%..\\..\\VC\\vcvarsall.bat" x86_amd64

set CC=clang-cl
set CXX=clang-cl
set CFLAGS=-m64 -fmsc-version=1900
set CXXFLAGS=-m64 -fmsc-version=1900
cmake ../ -GNinja -DCMAKE_BUILD_TYPE=Release