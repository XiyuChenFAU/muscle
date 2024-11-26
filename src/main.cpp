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
   todo:
   spherical joint
   cylinder plot
   */
}

