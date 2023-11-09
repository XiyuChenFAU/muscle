/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#ifndef MUSCLEPAGE_H
#define MUSCLEPAGE_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QMessageBox>
#include "../../run_model.h"

class setmodelwindow;

class musclepage : public QWidget
{
    Q_OBJECT

public:
    musclepage(setmodelwindow *setmodelwin,QWidget *parent = nullptr);
    ~musclepage();
    QLineEdit* settext(const std::string& textdefault, int x, int y, int textwidth, int textheight, int fontsize);
    void setlabel(const std::string& labelname, int x, int y, int fontsize);
    QLineEdit* settextandlabel(const std::string& labelname, const std::string& textdefault, int x, int y, int textwidth, int textheight, int fontsize);
    std::string doubletostring(double num);
    double stringtodouble(std::string numStr);

    void errorbox(std::string errormessage);

    void setalltextedit(const std::vector<double>& rho_o, const std::string& rhoo_bodyname, const std::vector<double>& rho_i, const std::string& rhoi_bodyname, const std::string& name, int nodenum);


private:
    std::vector<QLabel*> qlabels;
    std::vector<QPushButton*> musclebuttons;
    QPushButton* savebutton=nullptr;
    QPushButton* deletebutton=nullptr;
    QPushButton* newmusclebutton=nullptr;
    QPushButton* plusbutton=nullptr;
    setmodelwindow *setmodelwin=nullptr;
    QLineEdit *musclenameEdit=nullptr;
    QLineEdit *nodenumEdit=nullptr;

    QLineEdit *rhoobodyEdit=nullptr;
    QLineEdit *rhooaxisxEdit=nullptr;
    QLineEdit *rhooaxisyEdit=nullptr;
    QLineEdit *rhooaxiszEdit=nullptr;

    QLineEdit *rhoibodyEdit=nullptr;
    QLineEdit *rhoiaxisxEdit=nullptr;
    QLineEdit *rhoiaxisyEdit=nullptr;
    QLineEdit *rhoiaxiszEdit=nullptr;
    QFrame* rectanglemain;

private slots:
    void plusbuttonsetting();
    void savebuttonsetting();
    void deletebuttonsetting();
    void newmusclebuttonsetting();
    void showmusclesetting(int index);

};

#endif // MUSCLEPAGE_H
