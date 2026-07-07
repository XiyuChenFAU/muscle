/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
03.11.2023
Xiyu Chen

*/

#ifndef POSTPROCESSINGPAGE_H
#define POSTPROCESSINGPAGE_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include "../../run_model.h"

class setmodelwindow;
class MuscleLengthChart;

class postprocessingpage : public QWidget
{
    Q_OBJECT

public:
    postprocessingpage(setmodelwindow *setmodelwin, QWidget *parent = nullptr);
    ~postprocessingpage();
    void updatevalue();

private:
    setmodelwindow *setmodelwin=nullptr;
    QFrame* rectangle=nullptr;
    QLabel* titleLabel=nullptr;
    MuscleLengthChart* lengthChart=nullptr;
};

#endif // POSTPROCESSINGPAGE_H
