/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#ifndef MOVEMENTSETTING_H
#define MOVEMENTSETTING_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>
#include <vector>

class movementsetting : public QDialog
{
    Q_OBJECT

public:
    movementsetting(std::vector<std::vector<double>> *moveSetting, QWidget *parent = nullptr);

private:
    std::vector<std::vector<double>> *moveSetting = nullptr;
    QScrollArea *scrollArea = nullptr;
    QWidget *content = nullptr;
    QPushButton *saveButton = nullptr;
    QPushButton *addButton = nullptr;
    QPushButton *deleteButton = nullptr;
    std::vector<QLineEdit*> initialEdits;
    std::vector<QLineEdit*> finalEdits;
    std::vector<QLineEdit*> stepEdits;
    std::vector<QLineEdit*> accelerationEdits;

    void rebuildRows();
    void syncRows();
    void addRow();
    void deleteRow();
    void saveAndClose();
};

#endif // MOVEMENTSETTING_H
