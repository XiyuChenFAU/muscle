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

private:
    std::vector<QLabel*> qlabels;
    std::vector<QLineEdit *> qedits;
    QFrame* rectangle=nullptr;
    jointpage *Jointpage=nullptr;
};

#endif // REVOLUTEJOINTPAGE_H
