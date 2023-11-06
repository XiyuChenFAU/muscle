/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
03.11.2023
Xiyu Chen

*/

#ifndef SOLVESETTINGPAGE_H
#define SOLVESETTINGPAGE_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QMessageBox>
#include "../../run_model.h"

class setmodelwindow;

class solvesettingpage : public QWidget
{
    Q_OBJECT

public:
    solvesettingpage(setmodelwindow *setmodelwin, QWidget *parent = nullptr);
    QLineEdit* settext(const std::string& textdefault, int x, int y, int textwidth, int textheight, int fontsize);
    void setlabel(const std::string& labelname, int x, int y, int fontsize);
    QLineEdit* settextandlabel(const std::string& labelname, const std::string& textdefault, int x, int y, int textwidth, int textheight, int fontsize);
    std::string doubletostring(double num);
    double stringtodouble(std::string numStr);
    void errorbox(std::string errormessage);

private:
    setmodelwindow *setmodelwin=nullptr;
    QLineEdit *rotatebodyEdit=nullptr;
    QLineEdit *rotationaxis_xEdit=nullptr;
    QLineEdit *rotationaxis_yEdit=nullptr;
    QLineEdit *rotationaxis_zEdit=nullptr;
    QLineEdit *rotationangleEdit=nullptr;
    QLineEdit *rotationanglestepEdit=nullptr;

    QLineEdit *tolpostprocessingEdit=nullptr;

    QLineEdit *tolEdit=nullptr;
    QLineEdit *max_iterEdit=nullptr;
    QLineEdit *linear_solverEdit=nullptr;
    QLineEdit *print_levelEdit=nullptr;
    QLineEdit *hessian_approximationEdit=nullptr;

private slots:
    void savesetting();
};

#endif // SOLVESETTINGPAGE_H
