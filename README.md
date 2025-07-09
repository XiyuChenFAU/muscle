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

# Download and install ipopt

## For mac
brew install ipopt

## For windows
download address https://github.com/coin-or/Ipopt/releases, download the corresponding .zip and unzip the software

Or you can compile it based on the compile

* git clone -b stable/3.12 https://github.com/coin-or/Ipopt.git CoinIpopt
* cd CoinIpopt/ThirdParty/
* cd Blas/
* ./get.Blas
* cd ../Lapack/
* ./get.Lapack
* cd ../ASL
* ./get.ASL
* cd ../Metis/
* ./get.Metis
* cd ../Mumps/
* ./get.Mumps


# Download and install casadi programm

## For mac
* git clone -b 3.6.3 https://github.com/casadi/casadi.git
* cd casadi
* mkdir build
* cd build
* cmake -DWITH_IPOPT=ON ..
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
* cmake -DWITH_IPOPT=ON -DCMAKE_C_COMPILER="C:/path/to/gcc/bin/gcc.exe" -DCMAKE_CXX_COMPILER="C:/path/to/g++/bin/g++.exe" -G "MinGW Makefiles" -DIPOPT_LIBRARIES:FILEPATH="C:/path/to/ipopt/lib/ipopt.dll.lib" -DIpopt_DIR="C:/path/to/ipopt" -DIPOPT_INCLUDE_DIRS:PATH="C:/path/to/ipopt/include/coin-or" ..
* cmake --build . --config Release
* cmake --install .


# make this project
* mkdir
* cd build
* cmake ..
* make

# make this project with print information
* cmake -DCMAKE_BUILD_TYPE=Debug ..

# install via Docker
Download and install docker via https://docs.docker.com/desktop/

Start the docker by using:
* docker compose up -d
* docker exec -it muscle /bin/bash

Finish the docker by using:
* docker compose down

