./clear.sh
mkdir thirdparty
mkdir thirdparty/antlr
cd thirdparty
cd antlr
wget -c https://www.antlr.org/download/antlr-4.12.0-complete.jar
cd ..
mkdir cpp_runtime
cd cpp_runtime
wget -c https://www.antlr.org/download/antlr4-cpp-runtime-4.12.0-source.zip
unzip -o antlr4-cpp-runtime-4.12.0-source.zip
mkdir build
cd build
cmake ..
make -j3

