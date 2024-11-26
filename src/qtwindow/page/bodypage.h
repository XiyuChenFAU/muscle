/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#ifndef BODYPAGE_H
#define BODYPAGE_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QMessageBox>
#include "../../run_model.h"
#include <QRadioButton>
#include <QButtonGroup>

class setmodelwindow;

class bodypage : public QWidget
{
    Q_OBJECT

public:
    bodypage(setmodelwindow *setmodelwin, QWidget *parent = nullptr);
    ~bodypage();
    QLineEdit* settext(const std::string& textdefault, int x, int y, int textwidth, int textheight, int fontsize);
    void setlabel(const std::string& labelname, int x, int y, int fontsize);
    QLineEdit* settextandlabel(const std::string& labelname,const std::string& textdefault, int x, int y, int textwidth, int textheight, int fontsize);
    std::string doubletostring(double num);
    double stringtodouble(std::string numStr);
    void errorbox(std::string errormessage);

    void setalltextedit(const std::string& bodyname, const std::string& parentbodyname, const std::vector<double>& naxis, double rotationangle, const std::vector<double>& rhobody, double a, double b, double c, double length, double radius, const std::string& shapename,int localglobal);
    void setlocalglobal();

private:
    std::vector<QRadioButton *> radioButtonsshape;
    QButtonGroup *buttonGroupshape = nullptr;
    std::vector<QRadioButton *> radioButtonsgloballocal;
    QButtonGroup *buttonGroupgloballocal = nullptr;
    std::vector<QLabel*> qlabels;
    std::vector<QPushButton*> bodybuttons;
    QPushButton* savebutton=nullptr;
    QPushButton* deletebutton=nullptr;
    QPushButton* newbodybutton=nullptr;
    QPushButton* plusbutton=nullptr;
    setmodelwindow *setmodelwin=nullptr;

    QLabel *positionrefer=nullptr;
    QLabel *rotationrefer=nullptr;

    QLineEdit *body_nameEdit=nullptr;
    QLineEdit *parentbody_nameEdit=nullptr;
    QLineEdit *rotationaxisx=nullptr;
    QLineEdit *rotationaxisy=nullptr;
    QLineEdit *rotationaxisz=nullptr;
    QLineEdit *initrotationangle=nullptr;
    QLineEdit *positionaxisx=nullptr;
    QLineEdit *positionaxisy=nullptr;
    QLineEdit *positionaxisz=nullptr;
    QLineEdit *aEdit=nullptr;
    QLineEdit *bEdit=nullptr;
    QLineEdit *cEdit=nullptr;
    QLineEdit *lengthEdit=nullptr;
    QLineEdit *radiusEdit=nullptr;
    QFrame* rectangle=nullptr;
    int selectedValueshape=-1;
    int selectedValuelocal=-1;
    body* Body=nullptr;

private slots:
    void plusbuttonsetting();
    void savebuttonsetting();
    void deletebuttonsetting();
    void newbodybuttonsetting();
    void showbodysetting(int index);
    void handleButtonClickedshape(QAbstractButton* button);
    void handleButtonClickedtype(QAbstractButton* button);

};

#endif // BODYPAGE_H
