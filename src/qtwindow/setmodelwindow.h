/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
03.11.2023
Xiyu Chen

*/

#ifndef SETMODELWINDOW_H
#define SETMODELWINDOW_H

#include <QWidget>
#include "page/bodypage.h"
#include "page/musclepage.h"
#include "page/solvesettingpage.h"
#include "page/runprogrampage.h"
#include "page/postprocessingpage.h"

class QPushButton;
class QStackedWidget;
class MainWindow;

class setmodelwindow : public QWidget
{
    Q_OBJECT

public:
    setmodelwindow(runmodel* Runmodel,QWidget *parent = nullptr);
    ~setmodelwindow();
    void addleftbutton(QPushButton * button);
    void changebuttonstyle(int index);
    runmodel* getRunmodel();

private slots:
    void showpage(int index);
    void showbackmodelpage();

private:

    std::vector<QPushButton*> buttons;
    QStackedWidget *stackedWidget=nullptr;
    runmodel* Runmodel=nullptr;
};

#endif // SETMODELWINDOW_H
