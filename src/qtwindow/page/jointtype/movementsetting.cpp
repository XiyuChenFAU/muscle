/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#include "movementsetting.h"
#include <QLabel>
#include <QScrollBar>
#include <algorithm>

movementsetting::movementsetting(std::vector<std::vector<double>> *moveSetting, QWidget *parent):
    QDialog(parent),
    moveSetting(moveSetting)
{
    setWindowTitle("movement design");
    setModal(true);
    resize(600, 520);
    setStyleSheet("background-color: #CCCCCC;");

    saveButton = new QPushButton("Save", this);
    saveButton->setGeometry(10, 10, 90, 36);
    saveButton->setStyleSheet("QPushButton { color: black; background-color: white; }");
    connect(saveButton, &QPushButton::clicked, this, &movementsetting::saveAndClose);

    scrollArea = new QScrollArea(this);
    scrollArea->setGeometry(10, 60, 560, 430);
    scrollArea->setWidgetResizable(false);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setStyleSheet(
        "QScrollArea { background: #CCCCCC; border: none; }"
        "QScrollArea > QWidget > QWidget { background: #CCCCCC; }"
        "QScrollBar:vertical { width: 10px; background: #CCCCCC; }"
        "QScrollBar::handle:vertical { background: white; border-radius: 5px; }"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0px; }"
    );
    scrollArea->viewport()->setStyleSheet("background: #CCCCCC;");

    content = new QWidget();
    content->setStyleSheet("background-color: #CCCCCC;");
    scrollArea->setWidget(content);

    if (this->moveSetting != nullptr && this->moveSetting->empty()) {
        this->moveSetting->push_back({0.0, 0.0, 0.0, 0.0});
    }
    rebuildRows();
}

void movementsetting::rebuildRows()
{
    qDeleteAll(content->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly));
    initialEdits.clear();
    finalEdits.clear();
    stepEdits.clear();
    accelerationEdits.clear();

    const int rowCount = moveSetting == nullptr ? 0 : static_cast<int>(moveSetting->size());
    content->resize(560, std::max(400, 70 + rowCount * 55));

    QLabel *initialLabel = new QLabel("initial", content);
    initialLabel->setGeometry(10, 10, 110, 20);
    initialLabel->setStyleSheet("QLabel { color: black; background-color: #CCCCCC; }");
    QLabel *finalLabel = new QLabel("final", content);
    finalLabel->setGeometry(130, 10, 110, 20);
    finalLabel->setStyleSheet("QLabel { color: black; background-color: #CCCCCC; }");
    QLabel *stepLabel = new QLabel("stepnum", content);
    stepLabel->setGeometry(250, 10, 110, 20);
    stepLabel->setStyleSheet("QLabel { color: black; background-color: #CCCCCC; }");
    QLabel *accLabel = new QLabel("acceleration", content);
    accLabel->setGeometry(370, 10, 120, 20);
    accLabel->setStyleSheet("QLabel { color: black; background-color: #CCCCCC; }");

    for (int row = 0; row < rowCount; ++row) {
        const int y = 35 + row * 55;
        std::vector<double> values = (*moveSetting)[row];
        values.resize(4, 0.0);

        QLineEdit *initialEdit = new QLineEdit(QString::number(values[0]), content);
        initialEdit->setGeometry(10, y, 105, 28);
        initialEdit->setStyleSheet("QLineEdit { background-color: white; }");
        initialEdit->setVisible(row == 0);
        initialEdits.push_back(initialEdit);

        QLineEdit *finalEdit = new QLineEdit(QString::number(values[1]), content);
        finalEdit->setGeometry(130, y, 105, 28);
        finalEdit->setStyleSheet("QLineEdit { background-color: white; }");
        finalEdits.push_back(finalEdit);

        QLineEdit *stepEdit = new QLineEdit(QString::number(values[2]), content);
        stepEdit->setGeometry(250, y, 105, 28);
        stepEdit->setStyleSheet("QLineEdit { background-color: white; }");
        stepEdits.push_back(stepEdit);

        QLineEdit *accelerationEdit = new QLineEdit(QString::number(values[3]), content);
        accelerationEdit->setGeometry(370, y, 105, 28);
        accelerationEdit->setStyleSheet("QLineEdit { background-color: white; }");
        accelerationEdits.push_back(accelerationEdit);
    }

    const int buttonY = 35 + std::max(0, rowCount - 1) * 55;
    addButton = new QPushButton("add", content);
    addButton->setGeometry(485, buttonY, 35, 28);
    addButton->setStyleSheet("QPushButton { color: black; background-color: white; }");
    connect(addButton, &QPushButton::clicked, this, &movementsetting::addRow);

    deleteButton = new QPushButton("delete", content);
    deleteButton->setGeometry(525, buttonY, 35, 28);
    deleteButton->setStyleSheet("QPushButton { color: black; background-color: white; }");
    connect(deleteButton, &QPushButton::clicked, this, &movementsetting::deleteRow);

    content->show();
    const auto children = content->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly);
    for (QWidget *child : children) {
        child->show();
    }
    for (int row = 1; row < static_cast<int>(initialEdits.size()); ++row) {
        initialEdits[row]->hide();
    }
}

void movementsetting::syncRows()
{
    if (moveSetting == nullptr) {
        return;
    }
    moveSetting->clear();
    double currentInitial = initialEdits.empty() ? 0.0 : initialEdits[0]->text().toDouble();
    for (int i = 0; i < static_cast<int>(finalEdits.size()); ++i) {
        const double finalValue = finalEdits[i]->text().toDouble();
        const double stepValue = stepEdits[i]->text().toDouble();
        const double accelerationValue = accelerationEdits[i]->text().toDouble();
        moveSetting->push_back({currentInitial, finalValue, stepValue, accelerationValue});
        currentInitial = finalValue;
    }
}

void movementsetting::addRow()
{
    syncRows();
    if (moveSetting != nullptr) {
        const double initialValue = moveSetting->empty() ? 0.0 : moveSetting->back()[1];
        moveSetting->push_back({initialValue, initialValue, 0.0, 0.0});
    }
    rebuildRows();
}

void movementsetting::deleteRow()
{
    syncRows();
    if (moveSetting != nullptr) {
        if (moveSetting->size() > 1) {
            moveSetting->pop_back();
        } else if (moveSetting->size() == 1) {
            (*moveSetting)[0] = {0.0, 0.0, 0.0, 0.0};
        }
    }
    rebuildRows();
}

void movementsetting::saveAndClose()
{
    syncRows();
    accept();
}
