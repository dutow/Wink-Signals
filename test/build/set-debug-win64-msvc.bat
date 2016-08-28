call "%VS140COMNTOOLS%..\\..\\VC\\vcvarsall.bat" x86_amd64

cmake ../ -GNinja -DCMAKE_BUILD_TYPE=Debug