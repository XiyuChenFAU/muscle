/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
03.11.2023
Xiyu Chen

*/

#ifndef REVOLUTEJOINTPAGE_H
#define REVOLUTEJOINTPAGE_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QButtonGroup>
#include <QRadioButton>
#include "../../../run_model.h"

class jointpage;

class revolutejointpage : public QWidget
{
    Q_OBJECT

public:
    revolutejointpage(jointpage *Jointpage, QWidget *parent = nullptr);
    ~revolutejointpage();
    QLineEdit* settext(const std::string& textdefault, int x, int y, int textwidth, int textheight, int fontsize);
    void setlabel(const std::string& labelname, int x, int y, int fontsize);
    QLineEdit* settextandlabel(const std::string& labelname, const std::string& textdefault, int x, int y, int textwidth, int textheight, int fontsize);
    std::string doubletostring(double num);
    std::vector<QLineEdit *> getqedits();
    void setforjointi(int index);
    bool isSelfDesign() const;
    std::vector<std::vector<std::vector<double>>> getMoveSettingValue(double stepnum);
    std::vector<std::vector<double>> getMovementValue();

private:
    std::vector<QLabel*> qlabels;
    std::vector<QLineEdit *> qedits;
    std::vector<QLabel*> simpleLabels;
    QButtonGroup *modeGroup = nullptr;
    QRadioButton *simpleRadio = nullptr;
    QRadioButton *selfDesignRadio = nullptr;
    QLabel *complexMovementLabel = nullptr;
    QLineEdit *movementEdit = nullptr;
    QPushButton *designButton = nullptr;
    std::vector<std::vector<std::vector<double>>> customMoveSetting = {{{0.0, 0.0, 0.0, 0.0}}, {}, {}};
    QFrame* rectangle=nullptr;
    jointpage *Jointpage=nullptr;

    void updateModeVisibility();
    void openMovementDesigner();
    std::string movementToString(const std::vector<double> &movement);
    std::vector<double> movementFromMoveSetting(const std::vector<std::vector<double>> &moveSettingAxis);
};

#endif // REVOLUTEJOINTPAGE_H
