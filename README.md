# muscle
muscle project for c++

# Install c++

## For Windows
Download address from https://github.com/skeeto/w64devkit/releases
* download the ".zip" folder and unzip
* set environment variable "your download folder path/w64devkit/bin"

# Install cmake

## For mac

brew install cmake

## For windows

download the cmake.org/download/ and select version for the cmake

# Download qt software
dowload address: https://download.qt.io/archive/qt/5.14/5.14.2/

## export qt5 directory

### For mac
export Qt5_DIR="your_qt5_install_directory"/Qt5.14.2/5.14.2/clang_64/lib/cmake/Qt5

### For Windows:
set environment variable name Qt5_DIR and value D:\path\to\qt5\msvc2017_64\lib\cmake\Qt5. Not set in the environmental path

# Download and install swig

## For mac
brew install swig

## For windows
download address https://www.swig.org/download.html, download the corresponding .zip and unzip the software

# Download and install ipopt

## For mac
brew install ipopt

## For windows
download address https://github.com/coin-or/Ipopt/releases, download the corresponding .zip and unzip the software

# Download and install casadi programm

## For mac
* git clone -b 3.6.3 https://github.com/casadi/casadi.git
* cd casadi
* mkdir build
* cd build
* cmake -DWITH_PYTHON=ON -DWITH_IPOPT=ON ..
* make
* sudo make install

### export casadi directory
export CasADi_DIR=/usr/local/include/casadi

## For windows
When you do "mkdir build", please sure it contains the CMakeLists.txtin the current folder
* git clone -b 3.6.3 https://github.com/casadi/casadi.git
* cd casadi
* mkdir build
* cd build
* cmake -DWITH_PYTHON=ON -DWITH_IPOPT=ON -DCMAKE_C_COMPILER="C:/path/to/gcc/bin/gcc.exe" -DCMAKE_CXX_COMPILER="C:/path/to/g++/bin/g++.exe" -G "MinGW Makefiles" -DIPOPT_LIBRARIES:FILEPATH="C:/path/to/ipopt/lib/ipopt.dll.lib" -DIPOPT_INCLUDE_DIRS:PATH="C:/path/to/ipopt/include/coin-or" -DSWIG_EXECUTABLE="C:/path/to/swig/swig.exe" ..
* cmake --build . --config Release
* sudo make install
<div style="display:none">

# make this project
* mkdir
* cd build
* cmake ..
* make

cmake -DWITH_PYTHON=ON -DWITH_IPOPT=ON -DCMAKE_C_COMPILER="D:/chenxiyu/teaching/MinGw/mingw64/bin/gcc.exe" -DCMAKE_CXX_COMPILER="D:/chenxiyu/teaching/MinGw/mingw64/bin/g++.exe" -G "MinGW Makefiles" -DIPOPT_LIBRARIES:FILEPATH="D:/chenxiyu/muscle/ipopt/Ipopt-3.13.3-win64-msvs2019-mdd/Ipopt-3.13.3-win64-msvs2019-mdd/lib/ipopt.dll.lib" -DIPOPT_INCLUDE_DIRS:PATH="D:/chenxiyu/muscle/ipopt/Ipopt-3.13.3-win64-msvs2019-mdd/Ipopt-3.13.3-win64-msvs2019-md/include/coin-or" -DSWIG_EXECUTABLE="D:/chenxiyu/muscle/swig/swigwin-4.1.1/swigwin-4.1.1/swig.exe" ..
</div>
