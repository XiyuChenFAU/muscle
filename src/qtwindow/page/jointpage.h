/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
03.11.2023
Xiyu Chen

*/

#ifndef JOINTPAGE_H
#define JOINTPAGE_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QMessageBox>
#include "../../run_model.h"
#include "jointtype/revolutejointpage.h"
#include "jointtype/sphericaljointpage.h"
#include "jointtype/translationjointpage.h"
#include <QRadioButton>
#include <QButtonGroup>
#include <QStackedWidget>

class setmodelwindow;

class jointpage : public QWidget
{
    Q_OBJECT

public:
    jointpage(setmodelwindow *setmodelwin, QWidget *parent = nullptr);
    ~jointpage();
    QLineEdit* settext(const std::string& textdefault, int x, int y, int textwidth, int textheight, int fontsize);
    void setlabel(const std::string& labelname, int x, int y, int fontsize);
    QLineEdit* settextandlabel(const std::string& labelname, const std::string& textdefault, int x, int y, int textwidth, int textheight, int fontsize);
    std::string doubletostring(double num);
    double stringtodouble(std::string numStr);
    void errorbox(std::string errormessage);
    runmodel* getrunmodel();
    void setalltextedit(const std::string& jointnamevalue, const std::string& bodynamevalue, const std::string& jointtypevalue, const std::vector<double>& positionvalue, int jointindex, int stepnum);
    void updatevalue();

private:
    std::vector<QLabel*> qlabels;
    setmodelwindow *setmodelwin=nullptr;
    
    QLineEdit *jointnameEdit=nullptr;
    QLineEdit *positionaxis_xEdit=nullptr;
    QLineEdit *positionaxis_yEdit=nullptr;
    QLineEdit *positionaxis_zEdit=nullptr;
    QLineEdit *stepnumstringEdit=nullptr;

    std::vector<QRadioButton *> radioButtonsbody;
    QButtonGroup *buttonGroupbody = nullptr;
    std::vector<QRadioButton *> radioButtonstype;
    QButtonGroup *buttonGrouptype = nullptr;

    QFrame* rectangle=nullptr;
    QPushButton *savebutton=nullptr;
    QPushButton *deletebutton=nullptr;
    QPushButton *plusbutton=nullptr;
    QPushButton *newjointbutton=nullptr;
    std::vector<QPushButton*> jointbuttons;

    QStackedWidget *stackedWidget=nullptr;
    revolutejointpage *Revolutejointpage = nullptr;
    sphericaljointpage *Sphericaljointpage = nullptr;
    translationjointpage *Translationjointpage = nullptr;
    int selectedValuetype=-1;
    int selectedValuebody=-1;
    joint* Joint = nullptr;



private slots:
    void savesetting();
    void deletesetting();
    void plusbuttonsetting();
    void newjointbuttonsetting();
    void showjointsetting(int index);
    void handleButtonClickedbody(QAbstractButton* button);
    void handleButtonClickedtype(QAbstractButton* button);
};

#endif // JOINTPAGE_H
