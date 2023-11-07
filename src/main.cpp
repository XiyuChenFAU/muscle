/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#include <ctime>
#include "qtwindow/mainwindow.h"
#include <QApplication>


int main(int argc, char* argv[]){
    
    QApplication a(argc, argv);
    MainWindow mainWin;
    mainWin.show();
    return a.exec();
    
   /*
   runmodel Runmodel("../example/elbow_model.json",1);
   Runmodel.runprogram();
   */
  /*
  <div style="display:none">
cmake -DWITH_PYTHON=ON -DWITH_IPOPT=ON -DCMAKE_C_COMPILER="D:/chenxiyu/teaching/MinGw/mingw64/bin/gcc.exe" -DCMAKE_CXX_COMPILER="D:/chenxiyu/teaching/MinGw/mingw64/bin/g++.exe" -G "MinGW Makefiles" -DIPOPT_LIBRARIES:FILEPATH="D:/chenxiyu/muscle/ipopt/Ipopt-3.13.3-win64-msvs2019-mdd/Ipopt-3.13.3-win64-msvs2019-mdd/lib/ipopt.dll.lib" -DIPOPT_INCLUDE_DIRS:PATH="D:/chenxiyu/muscle/ipopt/Ipopt-3.13.3-win64-msvs2019-mdd/Ipopt-3.13.3-win64-msvs2019-md/include/coin-or" -DSWIG_EXECUTABLE="D:/chenxiyu/muscle/swig/swigwin-4.1.1/swigwin-4.1.1/swig.exe" ..
</div>*/
}

