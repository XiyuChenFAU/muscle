/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
03.11.2023
Xiyu Chen

*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "setmodelwindow.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    //MainWindow(runmodel* Runmodel,QWidget *parent = nullptr);
    ~MainWindow();
    QTextEdit* settext(const std::string& textdefault, int x, int y, int textwidth, int textheight);
    void setlabel(const std::string& labelname, int x, int y);
    QTextEdit* settextandlabel(const std::string& labelname, const std::string& textdefault, int x, int y, int textwidth, int textheight);

private:
    QPushButton *selectJsonButton=nullptr;
    QPushButton *runButton=nullptr;
    QPushButton *selectFolderButton=nullptr;
    QTextEdit* jsonFolderPathLineEdit=nullptr;
    QTextEdit* jsonFilePathLineEdit=nullptr;
    QTextEdit* newmodelEdit=nullptr;
    setmodelwindow* setmodelWindow=nullptr;
    runmodel* Runmodel=nullptr;
    std::vector<QLabel*> qlabels;

private slots:
    void selectJsonFile();
    void selectFolder();
    void runModel();
};
#endif // MAINWINDOW_H
