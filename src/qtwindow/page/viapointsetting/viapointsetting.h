/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#ifndef VIAPOINTSETTING_H
#define VIAPOINTSETTING_H

#include <QDialog>
#include <QLineEdit>
#include <QRadioButton>
#include <QButtonGroup>
#include <QPushButton>
#include <QScrollArea>
#include <vector>
#include <string>

#include "../../../parm/parm.h"

struct ViapointDraft {
    double cutoff = 0.05;
    double alpha = 100.0;
    std::vector<double> rho = {0.0, 0.0, 0.0};
    std::vector<double> gamma = {0.0, 0.0, 0.0};
    std::string bodyName;
    int global = 0;
};

class viapointsetting : public QDialog
{
    Q_OBJECT

public:
    viapointsetting(Parm *parm, std::vector<ViapointDraft> *drafts, QWidget *parent = nullptr);

private:
    Parm *parm = nullptr;
    std::vector<ViapointDraft> *drafts = nullptr;
    QScrollArea *scrollArea = nullptr;
    QWidget *content = nullptr;
    QPushButton *saveButton = nullptr;
    QPushButton *addButton = nullptr;
    std::vector<QLineEdit*> cutoffEdits;
    std::vector<QLineEdit*> alphaEdits;
    std::vector<QLineEdit*> rhoXEdits;
    std::vector<QLineEdit*> rhoYEdits;
    std::vector<QLineEdit*> rhoZEdits;
    std::vector<QButtonGroup*> coordinateGroups;
    std::vector<QButtonGroup*> bodyGroups;

    void rebuildColumns();
    void syncDraftsFromWidgets();
    void setCoordinateEditValues(int index);
    void addDraftColumn();
    void deleteDraftColumn(int index);
    void saveDraftsAndClose();
};

#endif // VIAPOINTSETTING_H
