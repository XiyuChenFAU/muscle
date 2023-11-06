# muscle
muscle project for c++

# Download qt software
dowload address: https://download.qt.io/archive/qt/5.14/5.14.2/

## export qt5 directory

### For mac
export Qt5_DIR="your_qt5_install_directory"/Qt5.14.2/5.14.2/clang_64/lib/cmake/Qt5

### For Windows:
Setting the environment viriable


# Download and install casadi programm
* git clone -b 3.6.3 https://github.com/casadi/casadi.git
* cd casadi
* mkdir build
* cd build
* cmake -DWITH_PYTHON=ON -DWITH_IPOPT=ON ..
* make
* sudo make install

## export casadi directory

### For mac
export CasADi_DIR=/usr/local/include/casadi

### For windows
Setting the environment viriable

# make this project
* mkdir
* cd build
* cmake ..
* make

