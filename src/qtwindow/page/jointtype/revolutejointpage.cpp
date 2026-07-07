/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
03.11.2023
Xiyu Chen

*/

#include "revolutejointpage.h"
#include "../jointpage.h"
#include "movementsetting.h"
#include <sstream>

revolutejointpage::revolutejointpage(jointpage *Jointpage, QWidget *parent):
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
    connect(modeGroup, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked), this, &revolutejointpage::updateModeVisibility);

    QLineEdit *axisxedit=settextandlabel("rotation axis x","0", 0, 45, 300, 30, allfontsize);
    qedits.push_back(axisxedit);
    QLineEdit *axisyedit=settextandlabel("rotation axis y","0", 0, 125, 300, 30, allfontsize);
    qedits.push_back(axisyedit);
    QLineEdit *axiszedit=settextandlabel("rotation axis z","0", 0, 205, 300, 30, allfontsize);
    qedits.push_back(axiszedit);
    QLineEdit *axisinitialangleedit=settextandlabel("initial rotation angle","0", 0, 285, 140, 30, allfontsize);
    qedits.push_back(axisinitialangleedit);
    QLineEdit *axisangleedit=settextandlabel("final angle","0", 160, 285, 140, 30, allfontsize);
    qedits.push_back(axisangleedit);

    simpleLabels = qlabels;
    complexMovementLabel = new QLabel("complex movement", this);
    complexMovementLabel->setGeometry(0, 285, 180, 25);
    complexMovementLabel->setStyleSheet("QLabel { color : black; background-color : #CCCCCC; }");
    movementEdit = new QLineEdit(this);
    movementEdit->setGeometry(0, 315, 210, 30);
    movementEdit->setStyleSheet("QLineEdit { color: black; }");
    designButton = new QPushButton("design", this);
    designButton->setGeometry(220, 315, 75, 30);
    designButton->setStyleSheet("QPushButton { color: black; background-color: white; }");
    connect(designButton, &QPushButton::clicked, this, &revolutejointpage::openMovementDesigner);

    setforjointi(0);
    updateModeVisibility();
}

revolutejointpage::~revolutejointpage(){
    for(int i=0;i<qlabels.size();i++){
        delete qlabels[i];
    }
    for(int i=0;i<qedits.size();i++){
        delete qedits[i];
    }
    delete modeGroup;
    delete complexMovementLabel;
    delete movementEdit;
    delete designButton;
    delete rectangle;

}

QLineEdit* revolutejointpage::settext(const std::string& textdefault, int x, int y, int textwidth, int textheight ,int fontsize) {
    QLineEdit* body_nameEdit = new QLineEdit(this);
    QFont body_nameEditfont = body_nameEdit->font();
    body_nameEditfont.setPointSize(fontsize); 
    body_nameEdit->setFont(body_nameEditfont);
    body_nameEdit->setGeometry(x, y, textwidth, textheight);
    body_nameEdit->setText(QString::fromStdString(textdefault));
    body_nameEdit->setStyleSheet("QLineEdit { color: black; }");
    return body_nameEdit;
}
void revolutejointpage::setlabel(const std::string& labelname, int x, int y ,int fontsize) {
    QLabel *body_namelabel = new QLabel(this);
    QFont body_namefont = body_namelabel->font();
    body_namefont.setPointSize(fontsize); 
    body_namelabel->setFont(body_namefont);
    body_namelabel->setText(QString::fromStdString(labelname)); 
    body_namelabel->move(x, y);
    body_namelabel->setStyleSheet("QLabel { color : black; background-color : #CCCCCC; }");
    qlabels.push_back(body_namelabel);
}

QLineEdit* revolutejointpage::settextandlabel(const std::string& labelname, const std::string& textdefault, int x, int y, int textwidth, int textheight, int fontsize) {
    setlabel(labelname, x, y, fontsize);
    QLineEdit* body_nameEdit = settext(textdefault, x, y+25, textwidth, textheight, fontsize);
    return body_nameEdit;
}

std::string revolutejointpage::doubletostring(double num) {
    std::ostringstream stream;
    stream << num;
    std::string numStr = stream.str();
    return numStr;
}

std::vector<QLineEdit *> revolutejointpage::getqedits(){
    return qedits;
}
    
void revolutejointpage::setforjointi(int index){
    int jointnum=Jointpage->getrunmodel()->getModel()->getparm()->getn_joints();
    if(index>jointnum-1){
        for(int i=0;i<qedits.size();i++){
            qedits[i]->setText(QString::fromStdString("0"));
        }
        customMoveSetting = {{{0.0, 0.0, 0.0, 0.0}}, {}, {}};
        movementEdit->clear();
    }
    else{
        joint* Joint=Jointpage->getrunmodel()->getModel()->getparm()->getjointindex(index);
        std::vector<double> axisvector=Joint->getaxisvector();
        std::vector<std::vector<double>> rotationangle=Joint->get_movement();
        customMoveSetting = Joint->get_move_setting();
        customMoveSetting.resize(3);
        qedits[0]->setText(QString::fromStdString(doubletostring(axisvector[0])));
        qedits[1]->setText(QString::fromStdString(doubletostring(axisvector[1])));
        qedits[2]->setText(QString::fromStdString(doubletostring(axisvector[2])));

        for(int i=0; i<1;i++){
            if(rotationangle.size()>i){
                if(!rotationangle[i].empty()){
                    qedits[i+3]->setText(QString::fromStdString(doubletostring(rotationangle[i][0])));
                    qedits[i+4]->setText(QString::fromStdString(doubletostring(rotationangle[i].back())));
                }
                else{
                    qedits[i+3]->setText(QString::fromStdString(doubletostring(0.0)));
                    qedits[i+4]->setText(QString::fromStdString(doubletostring(0.0)));
                }
            }
            else{
                qedits[i+3]->setText(QString::fromStdString(doubletostring(0.0)));
                qedits[i+4]->setText(QString::fromStdString(doubletostring(0.0)));
            }
        }
        if(rotationangle.size()>0 && !rotationangle[0].empty()){
            movementEdit->setText(QString::fromStdString(movementToString(rotationangle[0])));
        }
        else{
            movementEdit->clear();
        }
    }
    updateModeVisibility();
}

bool revolutejointpage::isSelfDesign() const
{
    return selfDesignRadio != nullptr && selfDesignRadio->isChecked();
}

std::vector<std::vector<std::vector<double>>> revolutejointpage::getMoveSettingValue(double stepnum)
{
    if (isSelfDesign()) {
        customMoveSetting.resize(3);
        return customMoveSetting;
    }
    double initialanglevalue=qedits[3]->text().toDouble();
    double anglevalue=qedits[4]->text().toDouble();
    return {{{initialanglevalue,anglevalue,stepnum,0.0}},{},{}};
}

void revolutejointpage::updateModeVisibility()
{
    const bool selfDesign = isSelfDesign();
    for (int i=0;i<static_cast<int>(simpleLabels.size());i++) {
        simpleLabels[i]->setVisible(!selfDesign || i < 3);
    }
    for (int i=0;i<static_cast<int>(qedits.size());i++) {
        qedits[i]->setVisible(!selfDesign || i < 3);
    }
    complexMovementLabel->setVisible(selfDesign);
    movementEdit->setVisible(selfDesign);
    designButton->setVisible(selfDesign);
}

void revolutejointpage::openMovementDesigner()
{
    customMoveSetting.resize(3);
    movementsetting dialog(&customMoveSetting[0], this);
    if(dialog.exec()==QDialog::Accepted){
        if(!customMoveSetting[0].empty()){
            movementEdit->setText(QString::fromStdString(movementToString(movementFromMoveSetting(customMoveSetting[0]))));
        }
    }
}

std::string revolutejointpage::movementToString(const std::vector<double> &movement)
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

std::vector<std::vector<double>> revolutejointpage::getMovementValue()
{
    if(!isSelfDesign()){
        return {};
    }
    customMoveSetting.resize(3);
    return {movementFromMoveSetting(customMoveSetting[0])};
}

std::vector<double> revolutejointpage::movementFromMoveSetting(const std::vector<std::vector<double>> &moveSettingAxis)
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
