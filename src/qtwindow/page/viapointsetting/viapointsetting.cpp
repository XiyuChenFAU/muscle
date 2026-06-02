/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#include "viapointsetting.h"
#include <QLabel>
#include <QScrollBar>
#include <algorithm>

viapointsetting::viapointsetting(Parm *parm, std::vector<ViapointDraft> *drafts, QWidget *parent):
    QDialog(parent),
    parm(parm),
    drafts(drafts)
{
    setWindowTitle("viapoint setting");
    setModal(true);
    resize(1000, 600);
    setStyleSheet("background-color: #CCCCCC;");

    saveButton = new QPushButton("Save", this);
    saveButton->setGeometry(10, 10, 100, 40);
    saveButton->setStyleSheet("QPushButton { color: black; background-color: white; }");
    connect(saveButton, &QPushButton::clicked, this, &viapointsetting::saveDraftsAndClose);

    scrollArea = new QScrollArea(this);
    scrollArea->setGeometry(10, 60, 960, 510);
    scrollArea->setWidgetResizable(false);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setStyleSheet(
        "QScrollArea { background: #CCCCCC; border: none; }"
        "QScrollArea > QWidget > QWidget { background: #CCCCCC; }"
        "QScrollBar:horizontal { height: 10px; background: #CCCCCC; }"
        "QScrollBar::handle:horizontal { background: white; border-radius: 5px; }"
        "QScrollBar:vertical { width: 10px; background: #CCCCCC; }"
        "QScrollBar::handle:vertical { background: white; border-radius: 5px; }"
        "QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal { width: 0px; }"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0px; }"
    );
    scrollArea->viewport()->setStyleSheet("background: #CCCCCC;");

    content = new QWidget();
    content->setStyleSheet("background-color: #CCCCCC;");
    scrollArea->setWidget(content);

    rebuildColumns();
}

void viapointsetting::rebuildColumns()
{
    qDeleteAll(coordinateGroups);
    qDeleteAll(bodyGroups);
    qDeleteAll(content->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly));
    cutoffEdits.clear();
    alphaEdits.clear();
    rhoXEdits.clear();
    rhoYEdits.clear();
    rhoZEdits.clear();
    coordinateGroups.clear();
    bodyGroups.clear();

    const int columnWidth = 230;
    const int bodyCount = parm->getn_bodies();
    const int contentHeight = std::max(500, 340 + bodyCount * 28);
    const int contentWidth = std::max(930, static_cast<int>(drafts->size() + 1) * columnWidth + 20);
    content->resize(contentWidth, contentHeight);

    addButton = new QPushButton("add", content);
    addButton->setGeometry(static_cast<int>(drafts->size()) * columnWidth + 10, 10, 70, 35);
    addButton->setStyleSheet("QPushButton { color: black; background-color: white; }");
    connect(addButton, &QPushButton::clicked, this, &viapointsetting::addDraftColumn);
    addButton->show();

    for (int col = 0; col < static_cast<int>(drafts->size()); ++col) {
        const int x = col * columnWidth + 10;
        ViapointDraft &draft = (*drafts)[col];

        QLabel *title = new QLabel(QString("viapoint %1").arg(col + 1), content);
        title->setGeometry(x, 15, 150, 25);
        title->setStyleSheet("QLabel { color: black; background-color: #CCCCCC; }");

        QLabel *cutoffLabel = new QLabel("cutoff", content);
        cutoffLabel->setGeometry(x, 50, 100, 20);
        cutoffLabel->setStyleSheet("QLabel { color: black; background-color: #CCCCCC; }");
        QLineEdit *cutoffEdit = new QLineEdit(QString::number(draft.cutoff), content);
        cutoffEdit->setGeometry(x, 72, 180, 28);
        cutoffEdit->setStyleSheet("QLineEdit { background-color: white; }");
        cutoffEdits.push_back(cutoffEdit);

        QLabel *alphaLabel = new QLabel("alpha value", content);
        alphaLabel->setGeometry(x, 105, 120, 20);
        alphaLabel->setStyleSheet("QLabel { color: black; background-color: #CCCCCC; }");
        QLineEdit *alphaEdit = new QLineEdit(QString::number(draft.alpha), content);
        alphaEdit->setGeometry(x, 127, 180, 28);
        alphaEdit->setStyleSheet("QLineEdit { background-color: white; }");
        alphaEdits.push_back(alphaEdit);

        QLabel *coordLabel = new QLabel("coordinate", content);
        coordLabel->setGeometry(x, 160, 120, 20);
        coordLabel->setStyleSheet("QLabel { color: black; background-color: #CCCCCC; }");
        QButtonGroup *coordinateGroup = new QButtonGroup(content);
        coordinateGroups.push_back(coordinateGroup);
        QRadioButton *localRadio = new QRadioButton("local", content);
        localRadio->setGeometry(x, 182, 80, 24);
        localRadio->setStyleSheet("QRadioButton { color: black; background-color: #CCCCCC; }");
        QRadioButton *globalRadio = new QRadioButton("global", content);
        globalRadio->setGeometry(x + 85, 182, 90, 24);
        globalRadio->setStyleSheet("QRadioButton { color: black; background-color: #CCCCCC; }");
        coordinateGroup->addButton(localRadio, 0);
        coordinateGroup->addButton(globalRadio, 1);
        if (draft.global) {
            globalRadio->setChecked(true);
        } else {
            localRadio->setChecked(true);
        }
        connect(coordinateGroup, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked), this, [this, col]() {
            syncDraftsFromWidgets();
            setCoordinateEditValues(col);
        });

        QLabel *rhoLabel = new QLabel("rho xyz", content);
        rhoLabel->setGeometry(x, 215, 120, 20);
        rhoLabel->setStyleSheet("QLabel { color: black; background-color: #CCCCCC; }");
        const std::vector<double> &coordinate = draft.global ? draft.gamma : draft.rho;
        QLineEdit *rhoXEdit = new QLineEdit(QString::number(coordinate.size() > 0 ? coordinate[0] : 0.0), content);
        QLineEdit *rhoYEdit = new QLineEdit(QString::number(coordinate.size() > 1 ? coordinate[1] : 0.0), content);
        QLineEdit *rhoZEdit = new QLineEdit(QString::number(coordinate.size() > 2 ? coordinate[2] : 0.0), content);
        rhoXEdit->setGeometry(x, 237, 55, 28);
        rhoYEdit->setGeometry(x + 62, 237, 55, 28);
        rhoZEdit->setGeometry(x + 124, 237, 55, 28);
        rhoXEdit->setStyleSheet("QLineEdit { background-color: white; }");
        rhoYEdit->setStyleSheet("QLineEdit { background-color: white; }");
        rhoZEdit->setStyleSheet("QLineEdit { background-color: white; }");
        rhoXEdits.push_back(rhoXEdit);
        rhoYEdits.push_back(rhoYEdit);
        rhoZEdits.push_back(rhoZEdit);

        QLabel *bodyLabel = new QLabel("ref body", content);
        bodyLabel->setGeometry(x, 275, 120, 20);
        bodyLabel->setStyleSheet("QLabel { color: black; background-color: #CCCCCC; }");
        QButtonGroup *group = new QButtonGroup(content);
        bodyGroups.push_back(group);
        for (int i = 0; i < bodyCount; ++i) {
            std::string bodyName = parm->getbodyindex(i)->getname();
            QRadioButton *radio = new QRadioButton(QString::fromStdString(bodyName), content);
            radio->setGeometry(x, 300 + i * 28, 190, 24);
            radio->setStyleSheet("QRadioButton { color: black; background-color: #CCCCCC; }");
            group->addButton(radio, i);
            if (draft.bodyName == bodyName || (draft.bodyName.empty() && i == 0)) {
                radio->setChecked(true);
            }
        }

        QPushButton *deleteButton = new QPushButton("delete", content);
        deleteButton->setGeometry(x, contentHeight - 45, 100, 32);
        deleteButton->setStyleSheet("QPushButton { color: black; background-color: white; }");
        connect(deleteButton, &QPushButton::clicked, this, [this, col]() {
            deleteDraftColumn(col);
        });
    }
    content->show();
    const auto children = content->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly);
    for (QWidget *child : children) {
        child->show();
    }
}

void viapointsetting::addDraftColumn()
{
    syncDraftsFromWidgets();
    drafts->push_back(ViapointDraft());
    if (parm->getn_bodies() > 0) {
        drafts->back().bodyName = parm->getbodyindex(0)->getname();
    }
    rebuildColumns();
}

void viapointsetting::deleteDraftColumn(int index)
{
    syncDraftsFromWidgets();
    if (index >= 0 && index < static_cast<int>(drafts->size())) {
        drafts->erase(drafts->begin() + index);
        rebuildColumns();
    }
}

void viapointsetting::syncDraftsFromWidgets()
{
    const int count = std::min(static_cast<int>(drafts->size()), static_cast<int>(cutoffEdits.size()));
    for (int i = 0; i < count; ++i) {
        (*drafts)[i].cutoff = cutoffEdits[i]->text().toDouble();
        (*drafts)[i].alpha = alphaEdits[i]->text().toDouble();
        const std::vector<double> coordinate = {rhoXEdits[i]->text().toDouble(), rhoYEdits[i]->text().toDouble(), rhoZEdits[i]->text().toDouble()};
        if ((*drafts)[i].global) {
            (*drafts)[i].gamma = coordinate;
        } else {
            (*drafts)[i].rho = coordinate;
        }
        (*drafts)[i].global = coordinateGroups[i]->checkedId() == 1 ? 1 : 0;
        int bodyIndex = bodyGroups[i]->checkedId();
        if (bodyIndex >= 0 && bodyIndex < parm->getn_bodies()) {
            (*drafts)[i].bodyName = parm->getbodyindex(bodyIndex)->getname();
        }
    }
}

void viapointsetting::setCoordinateEditValues(int index)
{
    if (index < 0 || index >= static_cast<int>(drafts->size()) || index >= static_cast<int>(rhoXEdits.size())) {
        return;
    }
    const ViapointDraft &draft = (*drafts)[index];
    const std::vector<double> &coordinate = draft.global ? draft.gamma : draft.rho;
    rhoXEdits[index]->setText(QString::number(coordinate.size() > 0 ? coordinate[0] : 0.0));
    rhoYEdits[index]->setText(QString::number(coordinate.size() > 1 ? coordinate[1] : 0.0));
    rhoZEdits[index]->setText(QString::number(coordinate.size() > 2 ? coordinate[2] : 0.0));
}

void viapointsetting::saveDraftsAndClose()
{
    syncDraftsFromWidgets();
    accept();
}
