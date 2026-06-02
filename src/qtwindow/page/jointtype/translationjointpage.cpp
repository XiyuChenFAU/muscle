/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
03.11.2023
Xiyu Chen

*/

#include "translationjointpage.h"
#include "../jointpage.h"
#include "movementsetting.h"
#include <QMessageBox>
#include <algorithm>
#include <sstream>

translationjointpage::translationjointpage(jointpage *Jointpage, QWidget *parent):
    QWidget(parent),
    Jointpage(Jointpage)
{
    rectangle = new QFrame(this);
    rectangle->setGeometry(0, 0, 300, 480);
    rectangle->setStyleSheet("background-color: #CCCCCC;");

    int allfontsize=15;

    modeGroup = new QButtonGroup(this);
    simpleRadio = new QRadioButton("simple", this);
    simpleRadio->setGeometry(0, 0, 110, 28);
    simpleRadio->setStyleSheet("QRadioButton { color: black; background-color: #CCCCCC; }");
    selfDesignRadio = new QRadioButton("self design", this);
    selfDesignRadio->setGeometry(120, 0, 140, 28);
    selfDesignRadio->setStyleSheet("QRadioButton { color: black; background-color: #CCCCCC; }");
    modeGroup->addButton(simpleRadio, 0);
    modeGroup->addButton(selfDesignRadio, 1);
    simpleRadio->setChecked(true);
    connect(modeGroup, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked), this, &translationjointpage::updateModeVisibility);

    QLineEdit *initialanglexedit=settextandlabel("initial translation x","0", 0, 45, 140, 30, allfontsize);
    qedits.push_back(initialanglexedit);
    QLineEdit *initialangleyedit=settextandlabel("initial translation y","0", 0, 125, 140, 30, allfontsize);
    qedits.push_back(initialangleyedit);
    QLineEdit *initialanglezedit=settextandlabel("initial translation z","0", 0, 205, 140, 30, allfontsize);
    qedits.push_back(initialanglezedit);
    QLineEdit *axisxedit=settextandlabel("translation x","0", 160, 45, 140, 30, allfontsize);
    qedits.push_back(axisxedit);
    QLineEdit *axisyedit=settextandlabel("translation y","0", 160, 125, 140, 30, allfontsize);
    qedits.push_back(axisyedit);
    QLineEdit *axiszedit=settextandlabel("translation z","0", 160, 205, 140, 30, allfontsize);
    qedits.push_back(axiszedit);

    simpleLabels = qlabels;
    const std::vector<std::string> axisNames = {"x", "y", "z"};
    for(int i=0;i<3;i++){
        QLabel *label = new QLabel(QString::fromStdString("complex movement " + axisNames[i]), this);
        label->setGeometry(0, 45 + i * 80, 180, 25);
        label->setStyleSheet("QLabel { color : black; background-color : #CCCCCC; }");
        complexMovementLabels.push_back(label);
        QLineEdit *movementEdit = new QLineEdit(this);
        movementEdit->setGeometry(0, 70 + i * 80, 210, 30);
        movementEdit->setStyleSheet("QLineEdit { color: black; }");
        movementEdits.push_back(movementEdit);
        QPushButton *designButton = new QPushButton("design", this);
        designButton->setGeometry(220, 70 + i * 80, 75, 30);
        designButton->setStyleSheet("QPushButton { color: black; background-color: white; }");
        connect(designButton, &QPushButton::clicked, this, [this, i]() { openMovementDesigner(i); });
        designButtons.push_back(designButton);
    }

    setforjointi(0);
    updateModeVisibility();
}

translationjointpage::~translationjointpage(){
    for(int i=0;i<qlabels.size();i++){
        delete qlabels[i];
    }
    for(int i=0;i<qedits.size();i++){
        delete qedits[i];
    }
    delete modeGroup;
    for(QLabel *label : complexMovementLabels){
        delete label;
    }
    for(QLineEdit *edit : movementEdits){
        delete edit;
    }
    for(QPushButton *button : designButtons){
        delete button;
    }
    delete rectangle;

}

QLineEdit* translationjointpage::settext(const std::string& textdefault, int x, int y, int textwidth, int textheight ,int fontsize) {
    QLineEdit* body_nameEdit = new QLineEdit(this);
    QFont body_nameEditfont = body_nameEdit->font();
    body_nameEditfont.setPointSize(fontsize); 
    body_nameEdit->setFont(body_nameEditfont);
    body_nameEdit->setGeometry(x, y, textwidth, textheight);
    body_nameEdit->setText(QString::fromStdString(textdefault));
    body_nameEdit->setStyleSheet("QLineEdit { color: black; }");
    return body_nameEdit;
}
void translationjointpage::setlabel(const std::string& labelname, int x, int y ,int fontsize) {
    QLabel *body_namelabel = new QLabel(this);
    QFont body_namefont = body_namelabel->font();
    body_namefont.setPointSize(fontsize); 
    body_namelabel->setFont(body_namefont);
    body_namelabel->setText(QString::fromStdString(labelname)); 
    body_namelabel->move(x, y);
    body_namelabel->setStyleSheet("QLabel { color : black; background-color : #CCCCCC; }");
    qlabels.push_back(body_namelabel);
}

QLineEdit* translationjointpage::settextandlabel(const std::string& labelname, const std::string& textdefault, int x, int y, int textwidth, int textheight, int fontsize) {
    setlabel(labelname, x, y, fontsize);
    QLineEdit* body_nameEdit = settext(textdefault, x, y+25, textwidth, textheight, fontsize);
    return body_nameEdit;
}

std::string translationjointpage::doubletostring(double num) {
    std::ostringstream stream;
    stream << num;
    std::string numStr = stream.str();
    return numStr;
}

std::vector<QLineEdit *> translationjointpage::getqedits(){
    return qedits;
}
    
void translationjointpage::setforjointi(int index){
    int jointnum=Jointpage->getrunmodel()->getModel()->getparm()->getn_joints();
    if(index>jointnum-1){
        for(int i=0;i<qedits.size();i++){
            qedits[i]->setText(QString::fromStdString("0"));
        }
        customMoveSetting = {{{0.0, 0.0, 0.0, 0.0}}, {{0.0, 0.0, 0.0, 0.0}}, {{0.0, 0.0, 0.0, 0.0}}};
        for(QLineEdit *edit : movementEdits){
            edit->clear();
        }
    }
    else{
        joint* Joint=Jointpage->getrunmodel()->getModel()->getparm()->getjointindex(index);
        std::vector<std::vector<double>> translation=Joint->get_movement();
        customMoveSetting = Joint->get_move_setting();
        customMoveSetting.resize(3);
        for(int i=0; i<3;i++){
            if(translation.size()>i){
                if(!translation[i].empty()){
                    qedits[i]->setText(QString::fromStdString(doubletostring(translation[i][0])));
                    qedits[i+3]->setText(QString::fromStdString(doubletostring(translation[i].back())));
                }
                else{
                    qedits[i]->setText(QString::fromStdString(doubletostring(0.0)));
                    qedits[i+3]->setText(QString::fromStdString(doubletostring(0.0)));
                }
            }
            else{
                qedits[i]->setText(QString::fromStdString(doubletostring(0.0)));
                qedits[i+3]->setText(QString::fromStdString(doubletostring(0.0)));
            }
            if(i<static_cast<int>(movementEdits.size())){
                if(translation.size()>i && !translation[i].empty()){
                    movementEdits[i]->setText(QString::fromStdString(movementToString(translation[i])));
                }
                else{
                    movementEdits[i]->clear();
                }
            }
        }
    }
    updateModeVisibility();
}

bool translationjointpage::isSelfDesign() const
{
    return selfDesignRadio != nullptr && selfDesignRadio->isChecked();
}

std::vector<std::vector<std::vector<double>>> translationjointpage::getMoveSettingValue(double stepnum)
{
    if (isSelfDesign()) {
        customMoveSetting.resize(3);
        normalizeSelfDesignAxes();
        return customMoveSetting;
    }
    double initialtranslation1value=qedits[0]->text().toDouble();
    double initialtranslation2value=qedits[1]->text().toDouble(); 
    double initialtranslation3value=qedits[2]->text().toDouble();
    double translation1value=qedits[3]->text().toDouble();
    double translation2value=qedits[4]->text().toDouble(); 
    double translation3value=qedits[5]->text().toDouble();
    return {{{initialtranslation1value,translation1value,stepnum,0.0}},{{initialtranslation2value,translation2value,stepnum,0.0}},{{initialtranslation3value,translation3value,stepnum,0.0}}};
}

void translationjointpage::updateModeVisibility()
{
    const bool selfDesign = isSelfDesign();
    for (QLabel *label : simpleLabels) {
        label->setVisible(!selfDesign);
    }
    for (QLineEdit *edit : qedits) {
        edit->setVisible(!selfDesign);
    }
    for (QLabel *label : complexMovementLabels) {
        label->setVisible(selfDesign);
    }
    for (QLineEdit *edit : movementEdits) {
        edit->setVisible(selfDesign);
    }
    for (QPushButton *button : designButtons) {
        button->setVisible(selfDesign);
    }
}

void translationjointpage::openMovementDesigner(int axisIndex)
{
    customMoveSetting.resize(3);
    movementsetting dialog(&customMoveSetting[axisIndex], this);
    if(dialog.exec()==QDialog::Accepted){
        movementEdits[axisIndex]->setText(QString::fromStdString(movementToString(movementFromMoveSetting(customMoveSetting[axisIndex]))));
    }
}

std::string translationjointpage::movementToString(const std::vector<double> &movement)
{
    std::ostringstream stream;
    for(int i=0;i<static_cast<int>(movement.size());i++){
        if(i){
            stream << ",";
        }
        stream << movement[i];
    }
    return stream.str();
}

std::vector<std::vector<double>> translationjointpage::getMovementValue()
{
    if(!isSelfDesign()){
        return {};
    }
    customMoveSetting.resize(3);
    normalizeSelfDesignAxes();
    return {
        movementFromMoveSetting(customMoveSetting[0]),
        movementFromMoveSetting(customMoveSetting[1]),
        movementFromMoveSetting(customMoveSetting[2])
    };
}

std::vector<double> translationjointpage::movementFromMoveSetting(const std::vector<std::vector<double>> &moveSettingAxis)
{
    if(moveSettingAxis.empty() || moveSettingAxis[0].size()<4){
        return {};
    }
    std::vector<double> movement = {moveSettingAxis[0][0]};
    for(const std::vector<double> &interval : moveSettingAxis){
        if(interval.size()<4){
            continue;
        }
        if(!movement.empty() && interval[0] != movement.back()){
            movement.push_back(interval[0]);
        }
        const int stepCount = static_cast<int>(interval[2]);
        if(stepCount<=0){
            continue;
        }
        const double velocity = (interval[1]-interval[0]-interval[2]*(interval[2]-1)/2.0*interval[3])/interval[2];
        for(int i=0;i<stepCount;i++){
            movement.push_back(movement.back()+velocity+i*interval[3]);
        }
    }
    return movement;
}

int translationjointpage::totalStepnum(const std::vector<std::vector<double>> &moveSettingAxis)
{
    int total = 0;
    for(const std::vector<double> &interval : moveSettingAxis){
        if(interval.size()>=3){
            total += static_cast<int>(interval[2]);
        }
    }
    return total;
}

void translationjointpage::normalizeSelfDesignAxes()
{
    customMoveSetting.resize(3);
    int totalStep = 0;
    for(const auto &axisSetting : customMoveSetting){
        totalStep = std::max(totalStep, totalStepnum(axisSetting));
    }
    if(totalStep<=0){
        return;
    }

    for(auto &axisSetting : customMoveSetting){
        if(totalStepnum(axisSetting)==0){
            axisSetting = {{0.0, 0.0, static_cast<double>(totalStep), 0.0}};
        }
    }

    const std::vector<std::vector<double>> movements = {
        movementFromMoveSetting(customMoveSetting[0]),
        movementFromMoveSetting(customMoveSetting[1]),
        movementFromMoveSetting(customMoveSetting[2])
    };
    int expectedSize = 0;
    for(const auto &movement : movements){
        if(!movement.empty()){
            if(expectedSize==0){
                expectedSize = static_cast<int>(movement.size());
            } else if(expectedSize != static_cast<int>(movement.size())){
                QMessageBox::warning(this, "movement setting", "Self design x/y/z movement sizes are different. Please make their total stepnum the same.");
                return;
            }
        }
    }

    for(int i=0;i<3 && i<static_cast<int>(movementEdits.size());i++){
        movementEdits[i]->setText(QString::fromStdString(movementToString(movements[i])));
    }
}
