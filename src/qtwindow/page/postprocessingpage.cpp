/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#include "postprocessingpage.h"
#include "../setmodelwindow.h"

postprocessingpage::postprocessingpage(setmodelwindow *setmodelwin, QWidget *parent):
    QWidget(parent),
    setmodelwin(setmodelwin)
{
    QFrame* rectangle = new QFrame(this);
    rectangle->setGeometry(0, 100, setmodelwin->width(), setmodelwin->width()+1000);
    rectangle->setStyleSheet("background-color: #CCCCCC;");
}
