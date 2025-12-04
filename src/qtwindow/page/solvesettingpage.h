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
#include <QFileDialog> 
#include <QRadioButton>
#include <QButtonGroup>
#include <QCheckBox>
#include "../../run_model.h"


class setmodelwindow;

class solvesettingpage : public QWidget
{
    Q_OBJECT

public:
    solvesettingpage(setmodelwindow *setmodelwin, QWidget *parent = nullptr);
    ~solvesettingpage();
    QLineEdit* settext(const std::string& textdefault, int x, int y, int textwidth, int textheight, int fontsize);
    void setlabel(const std::string& labelname, int x, int y, int fontsize);
    QLineEdit* settextandlabel(const std::string& labelname, const std::string& textdefault, int x, int y, int textwidth, int textheight, int fontsize);
    std::string doubletostring(double num);
    double stringtodouble(std::string numStr);
    void errorbox(std::string errormessage);
    void updatevalue();

private:
    std::vector<QLabel*> qlabels;
    setmodelwindow *setmodelwin=nullptr;
    QLineEdit *modelnameEdit=nullptr;
    QLineEdit *savepathEdit=nullptr;
    QLineEdit *lengthconstEdit=nullptr;
    QLabel* lenghtspringlabel=nullptr;

    QLineEdit *tolpostprocessingEdit=nullptr;

    QLineEdit *tolEdit=nullptr;
    QLineEdit *max_iterEdit=nullptr;
    QLineEdit *linear_solverEdit=nullptr;
    QLineEdit *print_levelEdit=nullptr;
    QLineEdit *hessian_approximationEdit=nullptr;
    QFrame* rectangle =nullptr;
    QPushButton *selectFolderButton=nullptr;
    QPushButton *savebutton =nullptr;

    std::vector<QRadioButton *> radioButtons;
    QButtonGroup *buttonGroup1 = nullptr;
    int selectedValue = 0;
    std::vector<QRadioButton *> radioButtons_mode;
    QButtonGroup *buttonGroup_initial_mode = nullptr;
    int selectedValue_mode = 0;
    std::vector<QRadioButton *> radioButtons_body;
    QButtonGroup *buttonGroup_initial_body = nullptr;
    int selectedValue_body = 0;

    QCheckBox* check_collision_CheckBox = nullptr;
    int check_collision_Value = 0;

    QCheckBox* check_phi_eta_plus_CheckBox = nullptr;
    int check_phi_eta_plus_Value = 0;

private slots:
    void savesetting();
    void openFolderDialog();
    void handleButtonClicked(QAbstractButton* button);
    void handleButtonClicked_mode(QAbstractButton* button);
    void handleButtonClicked_body(QAbstractButton* button);
    void handleCheckBoxChanged_collision(int state);
    void handleCheckBoxChanged_phi_eta_plus(int state);
};

#endif // SOLVESETTINGPAGE_H
