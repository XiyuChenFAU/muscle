/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#include "musclepage.h"
#include "../setmodelwindow.h"

musclepage::musclepage(setmodelwindow *setmodelwin,QWidget *parent):
    QWidget(parent),
    setmodelwin(setmodelwin)
{   
    std::string musclenamestring="";
    std::string nodenumstring="";
    std::string rhoobodyname="";
    std::string rhoostring_axis_x="0";
    std::string rhoostring_axis_y="0";
    std::string rhoostring_axis_z="0";
    std::string rhoibodyname="";
    std::string rhoistring_axis_x="0";
    std::string rhoistring_axis_y="0";
    std::string rhoistring_axis_z="0";
    rectanglemain = new QFrame(this);
    rectanglemain->setGeometry(0, 100, setmodelwin->width(), setmodelwin->width()+1000);
    rectanglemain->setStyleSheet("background-color: #CCCCCC;");

    int musclenum=setmodelwin->getRunmodel()->getModel()->getparm()->getn_muscles();
    plusbutton= new QPushButton("+", this);
    plusbutton->setStyleSheet("QPushButton { color: black; background-color: white;}");
    connect(plusbutton, &QPushButton::clicked, this, &musclepage::plusbuttonsetting);

    newmusclebutton= new QPushButton("new", this);
    newmusclebutton->setStyleSheet("QPushButton { color: black; background-color: #CCCCCC;font-weight: bold; border: 2px solid #CCCCCC;}");
    newmusclebutton->setGeometry(0, 50, 70, 50);
    connect(newmusclebutton, &QPushButton::clicked, this, &musclepage::newmusclebuttonsetting);

    if(musclenum){ 
        for(int i=0;i<musclenum;i++){
            QPushButton* musclebutton= new QPushButton(QString::fromStdString(setmodelwin->getRunmodel()->getModel()->getparm()->getmuscleindex(i)->getname()), this);
            musclebutton->setStyleSheet("QPushButton { color: black; background-color: white;}");
            musclebutton->setGeometry(i*70, 50, 70, 50);
            musclebuttons.push_back(musclebutton);
        }
        plusbutton->setGeometry(musclenum*70, 50, 50, 50);
        musclebuttons[0]->setStyleSheet("QPushButton { color: black; background-color: #CCCCCC;font-weight: bold; border: 2px solid #CCCCCC;}");
        musclenamestring=setmodelwin->getRunmodel()->getModel()->getparm()->getmuscleindex(0)->getname();
        nodenumstring=std::to_string(setmodelwin->getRunmodel()->getModel()->getparm()->getmuscleindex(0)->getnodenum());
        rhoobodyname=setmodelwin->getRunmodel()->getModel()->getparm()->getmuscleindex(0)->getrhoo_bodyname();
        std::vector<double> rhooaxis=setmodelwin->getRunmodel()->getModel()->getparm()->getmuscleindex(0)->getrho_o();
        rhoostring_axis_x=doubletostring(rhooaxis[0]);
        rhoostring_axis_y=doubletostring(rhooaxis[1]);
        rhoostring_axis_z=doubletostring(rhooaxis[2]);
        rhoibodyname=setmodelwin->getRunmodel()->getModel()->getparm()->getmuscleindex(0)->getrhoi_bodyname();
        std::vector<double> rhoiaxis=setmodelwin->getRunmodel()->getModel()->getparm()->getmuscleindex(0)->getrho_i();
        rhoistring_axis_x=doubletostring(rhoiaxis[0]);
        rhoistring_axis_y=doubletostring(rhoiaxis[1]);
        rhoistring_axis_z=doubletostring(rhoiaxis[2]);
        newmusclebutton->setVisible(false);
    }
    else{
        plusbutton->setGeometry(70, 50, 50, 50);
        
    }

    for (int i = 0; i < musclebuttons.size(); i++) {
        connect(musclebuttons[i], &QPushButton::clicked, this, [this, i]() {
            showmusclesetting(i);
        });
    }
    
    int allfontsize=15;
    //bodybasie
    setlabel("Muscle information", 10, 110,20);

    musclenameEdit=settextandlabel("muscle name",musclenamestring, 10, 150, 450, 30, allfontsize);
    nodenumEdit=settextandlabel("node number",nodenumstring, 660, 150, 450, 30, allfontsize);
    setlabel("origin body name", 10, 230,allfontsize);
    setlabel("origin position refer body coordinate", 300, 230, allfontsize);
    rhooaxisxEdit = settextandlabel("axis x",rhoostring_axis_x, 360, 265, 100, 30, allfontsize);
    rhooaxisyEdit = settextandlabel("axis y",rhoostring_axis_y, 360, 340, 100, 30, allfontsize);
    rhooaxiszEdit = settextandlabel("axis z",rhoostring_axis_z, 360, 425, 100, 30, allfontsize);

    setlabel("insertion body name", 660, 230,allfontsize);
    setlabel("insertion position refer body coordinate", 950, 230, allfontsize);
    rhoiaxisxEdit = settextandlabel("axis x",rhoistring_axis_x, 1010, 255, 100, 30, allfontsize);
    rhoiaxisyEdit = settextandlabel("axis y",rhoistring_axis_y, 1010, 340, 100, 30, allfontsize);
    rhoiaxiszEdit = settextandlabel("axis z",rhoistring_axis_z, 1010, 425, 100, 30, allfontsize);

    buttonGroupo = new QButtonGroup;
    for(int i=0;i<setmodelwin->getRunmodel()->getModel()->getparm()->getn_bodies();i++){
        QRadioButton* radioButton = new QRadioButton(QString::fromStdString(setmodelwin->getRunmodel()->getModel()->getparm()->getbodyindex(i)->getname()), this);
        radioButtonso.push_back(radioButton);
        radioButtonso[i]->setStyleSheet("QRadioButton { color: black; background-color: #CCCCCC;}");
        radioButtonso[i]->setGeometry(10, 260+i*40, 340, 30);
        buttonGroupo->addButton(radioButtonso[i], i);
    }
    connect(buttonGroupo, QOverload<int>::of(&QButtonGroup::buttonClicked), this, &musclepage::handleButtonClickedo);
    int findbodyo=setmodelwin->getRunmodel()->getModel()->getparm()->findbodyindex(rhoobodyname);
    if(findbodyo>-1){
        radioButtonso[findbodyo]->setChecked(true);
    }
    
    buttonGroupi = new QButtonGroup;
    for(int i=0;i<setmodelwin->getRunmodel()->getModel()->getparm()->getn_bodies();i++){
        QRadioButton* radioButton = new QRadioButton(QString::fromStdString(setmodelwin->getRunmodel()->getModel()->getparm()->getbodyindex(i)->getname()), this);
        radioButtonsi.push_back(radioButton);
        radioButtonsi[i]->setStyleSheet("QRadioButton { color: black; background-color: #CCCCCC;}");
        radioButtonsi[i]->setGeometry(660, 260+i*40, 340, 30);
        buttonGroupi->addButton(radioButtonsi[i], i);
    }
    connect(buttonGroupi, QOverload<int>::of(&QButtonGroup::buttonClicked), this, &musclepage::handleButtonClickedi);
    int findbodyi=setmodelwin->getRunmodel()->getModel()->getparm()->findbodyindex(rhoibodyname);
    if(findbodyi>-1){
        radioButtonsi[findbodyi]->setChecked(true);
    }

    //save button
    savebutton = new QPushButton("Save", this);
    savebutton->setStyleSheet("QPushButton { color: black; background-color: grey;}");
    savebutton->setGeometry(1010, 530, 100, 50);
    connect(savebutton, &QPushButton::clicked, this, &musclepage::savebuttonsetting);

    //delete button
    deletebutton = new QPushButton("Delete muscle", this);
    deletebutton->setStyleSheet("QPushButton { color: black; background-color: grey;}");
    deletebutton->setGeometry(1010, 620, 100, 50);
    connect(deletebutton, &QPushButton::clicked, this, &musclepage::deletebuttonsetting);
}

musclepage::~musclepage(){
    for(int i=0;i<musclebuttons.size();i++){
        delete musclebuttons[i];
    }
    for(int i=0;i<qlabels.size();i++){
        delete qlabels[i];
    }
    
    for(int i=0;i<radioButtonso.size();i++){
        delete radioButtonso[i];
    }
    for(int i=0;i<radioButtonsi.size();i++){
        delete radioButtonsi[i];
    }
    delete buttonGroupo;
    delete buttonGroupi;
    
    delete savebutton;
    delete deletebutton;
    delete newmusclebutton;
    delete plusbutton;

    delete musclenameEdit;
    delete nodenumEdit;

    delete rhooaxisxEdit;
    delete rhooaxisyEdit;
    delete rhooaxiszEdit;

    delete rhoiaxisxEdit;
    delete rhoiaxisyEdit;
    delete rhoiaxiszEdit;
    delete rectanglemain;
}

QLineEdit* musclepage::settext(const std::string& textdefault, int x, int y, int textwidth, int textheight ,int fontsize) {
    QLineEdit* body_nameEdit = new QLineEdit(this);
    QFont body_nameEditfont = body_nameEdit->font();
    body_nameEditfont.setPointSize(fontsize); 
    body_nameEdit->setFont(body_nameEditfont);
    body_nameEdit->setGeometry(x, y, textwidth, textheight);
    body_nameEdit->setText(QString::fromStdString(textdefault));
    body_nameEdit->setStyleSheet("QLineEdit { color: black; }");
    return body_nameEdit;
}
void musclepage::setlabel(const std::string& labelname, int x, int y ,int fontsize) {
    QLabel *body_namelabel = new QLabel(this);
    QFont body_namefont = body_namelabel->font();
    body_namefont.setPointSize(fontsize); 
    body_namelabel->setFont(body_namefont);
    body_namelabel->setText(QString::fromStdString(labelname)); 
    body_namelabel->move(x, y);
    body_namelabel->setStyleSheet("QLabel { color : black; background-color : #CCCCCC; }");
    qlabels.push_back(body_namelabel);
}

QLineEdit* musclepage::settextandlabel(const std::string& labelname, const std::string& textdefault, int x, int y, int textwidth, int textheight, int fontsize) {
    setlabel(labelname, x, y, fontsize);
    QLineEdit* body_nameEdit = settext(textdefault, x, y+25, textwidth, textheight, fontsize);
    return body_nameEdit;
}

std::string musclepage::doubletostring(double num) {
    std::ostringstream stream;
    stream << num;
    std::string numStr = stream.str();
    return numStr;
}

double musclepage::stringtodouble(std::string numStr) {
    double num;
    std::istringstream stream(numStr);
    stream >> num;
    return num;
}

void musclepage::errorbox(std::string errormessage){
    QMessageBox errorMessage;
    errorMessage.setWindowTitle("error");
    errorMessage.setText(QString::fromStdString(errormessage));
    errorMessage.setIcon(QMessageBox::Critical);
    errorMessage.addButton("Yes", QMessageBox::AcceptRole);
    errorMessage.exec();
}

void musclepage::setalltextedit(const std::vector<double>& rho_o, const std::string& rhoo_bodyname, const std::vector<double>& rho_i, const std::string& rhoi_bodyname, const std::string& name, int nodenum){
    
    musclenameEdit->setText(QString::fromStdString(name));
    nodenumEdit->setText(QString::fromStdString(std::to_string(nodenum)));

    int findbodyo=setmodelwin->getRunmodel()->getModel()->getparm()->findbodyindex(rhoo_bodyname);
    if(findbodyo>-1){
        radioButtonso[findbodyo]->setChecked(true);
    }
    rhooaxisxEdit->setText(QString::fromStdString(doubletostring(rho_o[0])));
    rhooaxisyEdit->setText(QString::fromStdString(doubletostring(rho_o[1])));
    rhooaxiszEdit->setText(QString::fromStdString(doubletostring(rho_o[2])));

    int findbodyi=setmodelwin->getRunmodel()->getModel()->getparm()->findbodyindex(rhoi_bodyname);
    if(findbodyi>-1){
        radioButtonsi[findbodyi]->setChecked(true);
    }

    rhoiaxisxEdit->setText(QString::fromStdString(doubletostring(rho_i[0])));
    rhoiaxisyEdit->setText(QString::fromStdString(doubletostring(rho_i[1])));
    rhoiaxiszEdit->setText(QString::fromStdString(doubletostring(rho_i[2])));
}

void musclepage::plusbuttonsetting(){
    if(!newmusclebutton->isVisible()){
        newmusclebutton->setVisible(true);
        newmusclebutton->setStyleSheet("QPushButton { color: black; background-color: #CCCCCC;font-weight: bold; border: 2px solid #CCCCCC;}");
        newmusclebutton->setGeometry(setmodelwin->getRunmodel()->getModel()->getparm()->getn_muscles()*70, 50, 70, 50);
        plusbutton->setGeometry(setmodelwin->getRunmodel()->getModel()->getparm()->getn_muscles()*70+70, 50, 50, 50);
        setalltextedit({0.0,0.0,0.0}, "", {0.0,0.0,0.0}, "", "", 0);
        for(int i=0;i<musclebuttons.size();i++){
            musclebuttons[i]->setStyleSheet("QPushButton { color: black; background-color: white;}");
        }
    }
}

void musclepage::savebuttonsetting(){
    if(selectedValueo<0 || selectedValuei<0){
        if(selectedValueo<0 ){
            errorbox("please check origin body name, it does not exist");
        }
        if(selectedValuei<0){
            errorbox("please check insertion body name, it does not exist");
        }
    }
    else{
        std::vector<double> rhooaxisvalue={rhooaxisxEdit->text().toDouble(),rhooaxisyEdit->text().toDouble(),rhooaxiszEdit->text().toDouble()};
        std::vector<double> rhoiaxisvalue={rhoiaxisxEdit->text().toDouble(),rhoiaxisyEdit->text().toDouble(),rhoiaxiszEdit->text().toDouble()};
        setmodelwin->getRunmodel()->getModel()->getparm()->addmuscle(rhooaxisvalue, setmodelwin->getRunmodel()->getModel()->getparm()->getbodyindex(selectedValueo)->getname(), rhoiaxisvalue, setmodelwin->getRunmodel()->getModel()->getparm()->getbodyindex(selectedValuei)->getname(), musclenameEdit->text().toStdString(), nodenumEdit->text().toInt());
        if(setmodelwin->getRunmodel()->getModel()->getparm()->getn_muscles()>musclebuttons.size()){
            newmusclebutton->setVisible(false);
            for(int i=0;i<musclebuttons.size();i++){
                musclebuttons[i]->setStyleSheet("QPushButton { color: black; background-color: white;}");
            }
            std::string musclenewbuttonname=musclenameEdit->text().toStdString();
            QPushButton* muscleaddnewbutton= new QPushButton(QString::fromStdString(musclenewbuttonname), this);
            muscleaddnewbutton->setStyleSheet("QPushButton { color: black; background-color: #CCCCCC;font-weight: bold; border: 2px solid #CCCCCC;}");
            muscleaddnewbutton->setGeometry(musclebuttons.size()*70, 50, 70, 50);
            muscleaddnewbutton->setVisible(true);
            plusbutton->setGeometry(musclebuttons.size()*70+70, 50, 70, 50);
            musclebuttons.push_back(muscleaddnewbutton);
            int buttonsize=setmodelwin->getRunmodel()->getModel()->getparm()->getn_muscles()-1;
            connect(musclebuttons[buttonsize], &QPushButton::clicked, this, [this, buttonsize]() {
                showmusclesetting(buttonsize);
            });
        }
    }
}

void musclepage::deletebuttonsetting(){
    Parm* parm=setmodelwin->getRunmodel()->getModel()->getparm();
    int index= parm->deletemuscle(musclenameEdit->text().toStdString());
    if(index<0){
        errorbox("not such muscle");
    }
    else{
        if(newmusclebutton->isVisible()){
            newmusclebutton->setGeometry(parm->getn_muscles()*70, 50, 70, 50);
            plusbutton->setGeometry(parm->getn_muscles()*70+70, 50, 50, 50);
        }
        else{
            plusbutton->setGeometry(parm->getn_muscles()*70, 50, 50, 50);
        }
        delete musclebuttons[index];
        musclebuttons.erase(musclebuttons.begin() + index);
        for(int i=0;i<musclebuttons.size();i++){
            musclebuttons[i]->setGeometry(i*70, 50, 70, 50);
            connect(musclebuttons[i], &QPushButton::clicked, this, [this, i]() {
            showmusclesetting(i);
        });
        }
        if(parm->getn_muscles()>0){
            showmusclesetting(0);
        }
        else{
            newmusclebutton->setVisible(true);
            newmusclebutton->setGeometry(0, 50, 70, 50);
            plusbutton->setGeometry(70, 50, 50, 50);
        }
                
    }
}

void musclepage::newmusclebuttonsetting(){
    for(int i=0;i<musclebuttons.size();i++){
        musclebuttons[i]->setStyleSheet("QPushButton { color: black; background-color: white;}");
    }
    newmusclebutton->setStyleSheet("QPushButton { color: black; background-color: #CCCCCC;font-weight: bold; border: 2px solid #CCCCCC;}");
    setalltextedit({0.0,0.0,0.0}, "", {0.0,0.0,0.0}, "", "", 0);
}

void musclepage::showmusclesetting(int index){
    muscle* Muscle=setmodelwin->getRunmodel()->getModel()->getparm()->getmuscleindex(index);
    setalltextedit(Muscle->getrho_o(), Muscle->getrhoo_bodyname(), Muscle->getrho_i(), Muscle->getrhoi_bodyname(), Muscle->getname(), Muscle->getnodenum());
    for(int i=0;i<musclebuttons.size();i++){
        if(index==i){
            musclebuttons[i]->setStyleSheet("QPushButton { color: black; background-color: #CCCCCC;font-weight: bold; border: 2px solid #CCCCCC;}");
        }
        else{
            musclebuttons[i]->setStyleSheet("QPushButton { color: black; background-color: white;}");
        }
    }
    if(newmusclebutton->isVisible()){
        newmusclebutton->setStyleSheet("QPushButton { color: black; background-color: white;}");
    }
}

void musclepage::handleButtonClickedo(int value){
    selectedValueo = value;
}

void musclepage::handleButtonClickedi(int value){
    selectedValuei = value;
}