/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
03.11.2023
Xiyu Chen

*/

#include "jointpage.h"
#include "../setmodelwindow.h"

jointpage::jointpage(setmodelwindow *setmodelwin, QWidget *parent):
    QWidget(parent),
    setmodelwin(setmodelwin)
{
    std::string rotatebodyname="";
    std::string jointname="";
    std::string jointtypename="";
    std::string jointtype="";
    std::string positionaxis_x="0.0";
    std::string positionaxis_y="0.0";
    std::string positionaxis_z="0.0";
    std::string stepnumstring="0";
    int moveallbodyindex=0;    

    rectangle = new QFrame(this);
    rectangle->setGeometry(0, 100, setmodelwin->width(), setmodelwin->width()+1000);
    rectangle->setStyleSheet("background-color: #CCCCCC;");

    int jointnum=setmodelwin->getRunmodel()->getModel()->getparm()->getn_joints();
    plusbutton= new QPushButton("+", this);
    plusbutton->setStyleSheet("QPushButton { color: black; background-color: white;}");
    connect(plusbutton, &QPushButton::clicked, this, &jointpage::plusbuttonsetting);

    newjointbutton= new QPushButton("new", this);
    newjointbutton->setStyleSheet("QPushButton { color: black; background-color: #CCCCCC;font-weight: bold; border: 2px solid #CCCCCC;}");
    newjointbutton->setGeometry(0, 50, 70, 50);
    connect(newjointbutton, &QPushButton::clicked, this, &jointpage::newjointbuttonsetting);

    if(jointnum){ 
        for(int i=0;i<jointnum;i++){
            QPushButton* jointbutton= new QPushButton(QString::fromStdString(setmodelwin->getRunmodel()->getModel()->getparm()->getjointindex(i)->getname()), this);
            jointbutton->setStyleSheet("QPushButton { color: black; background-color: white;}");
            jointbutton->setGeometry(i*70, 50, 70, 50);
            jointbuttons.push_back(jointbutton);
        }
        plusbutton->setGeometry(jointnum*70, 50, 50, 50);
        jointbuttons[0]->setStyleSheet("QPushButton { color: black; background-color: #CCCCCC;font-weight: bold; border: 2px solid #CCCCCC;}");

        Joint=setmodelwin->getRunmodel()->getModel()->getparm()->getjointindex(0);

        jointname=Joint->getname();
        rotatebodyname=Joint->getbodyname();
        std::vector<double> positionrel=Joint->getrelative_pos();
        positionaxis_x=doubletostring(positionrel[0]);
        positionaxis_y=doubletostring(positionrel[1]);
        positionaxis_z=doubletostring(positionrel[2]);
        moveallbodyindex=Joint->getmove_all_body();
        jointtypename=Joint->getjoint_type();
        stepnumstring=std::to_string(Joint->getjoint_stepnum()); 

        newjointbutton->setVisible(false);
    }
    else{
        plusbutton->setGeometry(70, 50, 50, 50);
        
    }

    for (int i = 0; i < jointbuttons.size(); i++) {
        connect(jointbuttons[i], &QPushButton::clicked, this, [this, i]() {
            showjointsetting(i);
        });
    }

    int allfontsize=15;
    //solve problem
    setlabel("Joint information", 10, 110,20);
    jointnameEdit=settextandlabel("joint name",jointname, 10, 150, 450, 30, allfontsize);
    setlabel("joint position refer rotate body", 10, 240, allfontsize);
    positionaxis_xEdit = settextandlabel("axis x",positionaxis_x, 10, 270, 140, 30, allfontsize);
    positionaxis_yEdit = settextandlabel("axis y",positionaxis_y, 165, 270, 140, 30, allfontsize);
    positionaxis_zEdit = settextandlabel("axis z",positionaxis_z, 320, 270, 140, 30, allfontsize);

    buttonGrouprotateallbody = new QButtonGroup(this);
    QRadioButton* radioButtonallbody = new QRadioButton(QString::fromStdString("null"), this);
    radioButtonallbody->setVisible(false);
    radioButtonsrotateallbody.push_back(radioButtonallbody);
    buttonGrouprotateallbody->addButton(radioButtonsrotateallbody[0], -1);

    std::vector<std::string> moveallbody_str={"move child bodys","move all bodys"};

    for(int i=0;i<moveallbody_str.size();i++){
        QRadioButton* radioButton = new QRadioButton(QString::fromStdString(moveallbody_str[i]), this);
        radioButtonsrotateallbody.push_back(radioButton);
        radioButtonsrotateallbody[i+1]->setStyleSheet("QRadioButton { color: black; background-color: #CCCCCC;}");
        radioButtonsrotateallbody[i+1]->setGeometry(10 + i*170, 360, 340, 30);
        radioButtonsrotateallbody[i+1]->show();
        buttonGrouprotateallbody->addButton(radioButtonsrotateallbody[i+1], i);
    }
    connect(buttonGrouprotateallbody, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked), this, &jointpage::handleButtonClickedmoveallbody);
    radioButtonsrotateallbody[moveallbodyindex+1]->setChecked(true);
    selectedValuerotateallbody=moveallbodyindex;


    setlabel("rotate body name", 10, 400, allfontsize);


    updatevalue();


    setlabel("step number for all joints", 660, 110, allfontsize);
    stepnumstringEdit=settext(stepnumstring, 850, 110, 260, 30 ,allfontsize);
    setlabel("joint type", 660, 150, allfontsize);

    stackedWidget = new QStackedWidget(this);

    Revolutejointpage = new revolutejointpage(this);
    Revolutejointpage->setGeometry(820, 180, 300, 480);
    stackedWidget->addWidget(Revolutejointpage);

    Sphericaljointpage = new sphericaljointpage(this);
    Sphericaljointpage->setGeometry(820, 180, 300, 480);
    stackedWidget->addWidget(Sphericaljointpage);

    Translationjointpage = new translationjointpage(this);
    Translationjointpage->setGeometry(820, 180, 300, 480);
    stackedWidget->addWidget(Translationjointpage);

    stackedWidget->setGeometry(820, 180, 300, 480);

    buttonGrouptype = new QButtonGroup;
    QRadioButton* radioButtontypenull = new QRadioButton(QString::fromStdString("null"), this);
    radioButtontypenull->setVisible(false);
    radioButtonstype.push_back(radioButtontypenull);
    buttonGrouptype->addButton(radioButtonstype[0], -1);
    for(int i=0;i<joint::alljoint_type.size();i++){
        QRadioButton* radioButton = new QRadioButton(QString::fromStdString(joint::alljoint_type[i]), this);
        radioButtonstype.push_back(radioButton);
        radioButtonstype[i+1]->setStyleSheet("QRadioButton { color: black; background-color: #CCCCCC;}");
        radioButtonstype[i+1]->setGeometry(660, 180+i*40, 150, 30);
        buttonGrouptype->addButton(radioButtonstype[i+1], i);
    }
    connect(buttonGrouptype, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked), this, &jointpage::handleButtonClickedtype);
    int findtypeindex=setmodelwin->getRunmodel()->getModel()->getparm()->getjointindex(0)->getjointtypeindex(jointtypename);
    radioButtonstype[findtypeindex+1]->setChecked(true);
    selectedValuetype=findtypeindex;
    if(findtypeindex>-1){
        stackedWidget->setCurrentIndex(findtypeindex);
    }
    else{
        stackedWidget->setCurrentIndex(0);
    }

    //save button
    savebutton = new QPushButton("Save", this);
    savebutton->setStyleSheet("QPushButton { color: black; background-color: grey;}");
    savebutton->setGeometry(1010, 670, 100, 50);
    connect(savebutton, &QPushButton::clicked, this, &jointpage::savesetting);

    //delete button
    deletebutton = new QPushButton("Delete", this);
    deletebutton->setStyleSheet("QPushButton { color: black; background-color: grey;}");
    deletebutton->setGeometry(660, 670, 100, 50);
    connect(deletebutton, &QPushButton::clicked, this, &jointpage::deletesetting);

}

jointpage::~jointpage(){
    for(int i=0;i<qlabels.size();i++){
        delete qlabels[i];
    }

    for(int i=0;i<radioButtonstype.size();i++){
        delete radioButtonstype[i];
    }

    for(int i=0;i<radioButtonsbody.size();i++){
        delete radioButtonsbody[i];
    }

    for(int i=0;i<radioButtonsrotateallbody.size();i++){
        delete radioButtonsrotateallbody[i];
    }

    for(int i=0;i<jointbuttons.size();i++){
        delete jointbuttons[i];
    }
    
    delete jointnameEdit;
    delete positionaxis_xEdit;
    delete positionaxis_yEdit;
    delete positionaxis_zEdit;
    delete buttonGroupbody;
    delete buttonGrouptype;    
    delete buttonGrouprotateallbody;
    delete stepnumstringEdit;
    delete rectangle;
    delete savebutton;
    delete deletebutton;
    delete plusbutton;
    delete newjointbutton;
    delete Revolutejointpage;
    delete Sphericaljointpage;
    delete Translationjointpage;
    delete stackedWidget;
}

QLineEdit* jointpage::settext(const std::string& textdefault, int x, int y, int textwidth, int textheight ,int fontsize) {
    QLineEdit* body_nameEdit = new QLineEdit(this);
    QFont body_nameEditfont = body_nameEdit->font();
    body_nameEditfont.setPointSize(fontsize); 
    body_nameEdit->setFont(body_nameEditfont);
    body_nameEdit->setGeometry(x, y, textwidth, textheight);
    body_nameEdit->setText(QString::fromStdString(textdefault));
    body_nameEdit->setStyleSheet("QLineEdit { color: black; }");
    return body_nameEdit;
}
void jointpage::setlabel(const std::string& labelname, int x, int y ,int fontsize) {
    QLabel *body_namelabel = new QLabel(this);
    QFont body_namefont = body_namelabel->font();
    body_namefont.setPointSize(fontsize); 
    body_namelabel->setFont(body_namefont);
    body_namelabel->setText(QString::fromStdString(labelname)); 
    body_namelabel->move(x, y);
    body_namelabel->setStyleSheet("QLabel { color : black; background-color : #CCCCCC; }");
    qlabels.push_back(body_namelabel);
}

QLineEdit* jointpage::settextandlabel(const std::string& labelname, const std::string& textdefault, int x, int y, int textwidth, int textheight, int fontsize) {
    setlabel(labelname, x, y, fontsize);
    QLineEdit* body_nameEdit = settext(textdefault, x, y+25, textwidth, textheight, fontsize);
    return body_nameEdit;
}

std::string jointpage::doubletostring(double num) {
    std::ostringstream stream;
    stream << num;
    std::string numStr = stream.str();
    return numStr;
}

double jointpage::stringtodouble(std::string numStr) {
    double num;
    std::istringstream stream(numStr);
    stream >> num;
    return num;
}

void jointpage::errorbox(std::string errormessage){
    QMessageBox errorMessage;
    errorMessage.setWindowTitle("error");
    errorMessage.setText(QString::fromStdString(errormessage));
    errorMessage.setIcon(QMessageBox::Critical);
    errorMessage.addButton("Yes", QMessageBox::AcceptRole);
    errorMessage.exec();
}

runmodel* jointpage::getrunmodel(){
    return setmodelwin->getRunmodel();
}

void jointpage::setalltextedit(const std::string& jointnamevalue, const std::string& bodynamevalue, const std::string& jointtypevalue, const std::vector<double>& positionvalue, int jointindex, int stepnum, int moveallbodyindex){

    jointnameEdit->setText(QString::fromStdString(jointnamevalue));
    positionaxis_xEdit->setText(QString::fromStdString(doubletostring(positionvalue[0])));
    positionaxis_yEdit->setText(QString::fromStdString(doubletostring(positionvalue[1])));
    positionaxis_zEdit->setText(QString::fromStdString(doubletostring(positionvalue[2])));
    stepnumstringEdit->setText(QString::fromStdString(std::to_string(stepnum)));

    int findbodyrotate=setmodelwin->getRunmodel()->getModel()->getparm()->findbodyindex(bodynamevalue);
    radioButtonsbody[findbodyrotate+2]->setChecked(true);
    selectedValuebody=findbodyrotate+1;
    selectedValuerotateallbody=moveallbodyindex;
    radioButtonsrotateallbody[moveallbodyindex+1]->setChecked(true);
    

    int findtypeindex=setmodelwin->getRunmodel()->getModel()->getparm()->getjointindex(0)->getjointtypeindex(jointtypevalue);
    radioButtonstype[findtypeindex+1]->setChecked(true);
    selectedValuetype=findtypeindex;
    if(findtypeindex>-1){
        stackedWidget->setCurrentIndex(findtypeindex);
        Revolutejointpage->setforjointi(jointindex);
        Sphericaljointpage->setforjointi(jointindex);
        Translationjointpage->setforjointi(jointindex);
    }
    else{
        stackedWidget->setCurrentIndex(0);
        Revolutejointpage->setforjointi(setmodelwin->getRunmodel()->getModel()->getparm()->getn_joints());
        Sphericaljointpage->setforjointi(setmodelwin->getRunmodel()->getModel()->getparm()->getn_joints());
        Translationjointpage->setforjointi(setmodelwin->getRunmodel()->getModel()->getparm()->getn_joints());
    }

}

void jointpage::updatevalue(){
    //delete old information
    for (QRadioButton* btn : radioButtonsbody) {
        if (btn) {delete btn;}
    }
    radioButtonsbody.clear();
    if (buttonGroupbody) {
        delete buttonGroupbody;
        buttonGroupbody = nullptr;
    }

    std::string rotatebodyname="";
    int jointnum=setmodelwin->getRunmodel()->getModel()->getparm()->getn_joints();;
    if(jointnum){
        if(Joint!=nullptr){
            rotatebodyname = Joint->getbodyname();
        }
    }

    buttonGroupbody = new QButtonGroup;
    QRadioButton* radioButtonbodynull = new QRadioButton(QString::fromStdString("null"), this);
    radioButtonbodynull->setVisible(false);
    radioButtonsbody.push_back(radioButtonbodynull);
    buttonGroupbody->addButton(radioButtonsbody[0], -1);

    QRadioButton* radioButtonfix = new QRadioButton(QString::fromStdString("fix_space"), this);
    radioButtonsbody.push_back(radioButtonfix);
    radioButtonsbody[1]->setStyleSheet("QRadioButton { color: black; background-color: #CCCCCC;}");
    radioButtonsbody[1]->setGeometry(10, 430, 340, 30);
    radioButtonsbody[1]->show();
    buttonGroupbody->addButton(radioButtonsbody[1], 0);

    for(int i=0;i<setmodelwin->getRunmodel()->getModel()->getparm()->getn_bodies();i++){
        QRadioButton* radioButton = new QRadioButton(QString::fromStdString(setmodelwin->getRunmodel()->getModel()->getparm()->getbodyindex(i)->getname()), this);
        radioButtonsbody.push_back(radioButton);
        radioButtonsbody[i+2]->setStyleSheet("QRadioButton { color: black; background-color: #CCCCCC;}");
        radioButtonsbody[i+2]->setGeometry(10, 430+(i+1)*40, 340, 30);
        radioButtonsbody[i+2]->show();
        buttonGroupbody->addButton(radioButtonsbody[i+2], i+1);
    }
    connect(buttonGroupbody, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked), this, &jointpage::handleButtonClickedbody);
    int findbodyrotate=setmodelwin->getRunmodel()->getModel()->getparm()->findbodyindex(rotatebodyname);
    radioButtonsbody[findbodyrotate+2]->setChecked(true);
    selectedValuebody=findbodyrotate+1;
    if (rectangle) {
        rectangle->update();
        rectangle->show();
    }
    this->update();
}

void jointpage::savesetting(){
    if(selectedValuetype<0 || selectedValuebody<0){
        if(selectedValuebody<0 ){
            errorbox("please check body name, it does not exist");
        }
        if(selectedValuetype<0){
            errorbox("please check joint type name, it does not exist");
        }
    }
    else{
        std::vector<double> relative_posvalue={positionaxis_xEdit->text().toDouble(),positionaxis_yEdit->text().toDouble(),positionaxis_zEdit->text().toDouble()};
        double stepnum_value=stepnumstringEdit->text().toDouble();
        if(selectedValuetype==0){
            std::vector<QLineEdit *> alleditsrevo=Revolutejointpage->getqedits();
            std::vector<double> axisvectorvalue={alleditsrevo[0]->text().toDouble(),alleditsrevo[1]->text().toDouble(),alleditsrevo[2]->text().toDouble()};
            double initialanglevalue=alleditsrevo[3]->text().toDouble();
            double anglevalue=alleditsrevo[4]->text().toDouble();
            std::vector<std::vector<std::vector<double>>> move_setting_value={{{initialanglevalue,anglevalue,stepnum_value,0.0}},{},{}};
            setmodelwin->getRunmodel()->getModel()->getparm()->addjoint(jointnameEdit->text().toStdString(), setmodelwin->getRunmodel()->getModel()->getparm()->getbodyindex(selectedValuebody-1)->getname(), joint::alljoint_type[selectedValuetype], relative_posvalue,  axisvectorvalue, move_setting_value, {},selectedValuerotateallbody);
        }
        if(selectedValuetype==1){
            
            std::vector<QLineEdit *> alleditsspher=Sphericaljointpage->getqedits();
            double initialangle1value=alleditsspher[0]->text().toDouble();
            double initialangle2value=alleditsspher[1]->text().toDouble(); 
            double initialangle3value=alleditsspher[2]->text().toDouble();
            double angle1value=alleditsspher[3]->text().toDouble();
            double angle2value=alleditsspher[4]->text().toDouble(); 
            double angle3value=alleditsspher[5]->text().toDouble();
            std::vector<double> axisvectorvalue={0,0,0};
            std::vector<std::vector<std::vector<double>>> move_setting_value={{{initialangle1value,angle1value,stepnum_value,0.0}},{{initialangle2value,angle2value,stepnum_value,0.0}},{{initialangle3value,angle3value,stepnum_value,0.0}}};
            setmodelwin->getRunmodel()->getModel()->getparm()->addjoint(jointnameEdit->text().toStdString(), setmodelwin->getRunmodel()->getModel()->getparm()->getbodyindex(selectedValuebody-1)->getname(), joint::alljoint_type[selectedValuetype], relative_posvalue, axisvectorvalue, move_setting_value, {},selectedValuerotateallbody);
        }
        if(selectedValuetype==2){
            
            std::vector<QLineEdit *> alleditsspher=Translationjointpage->getqedits();
            double initialtranslation1value=alleditsspher[0]->text().toDouble();
            double initialtranslation2value=alleditsspher[1]->text().toDouble(); 
            double initialtranslation3value=alleditsspher[2]->text().toDouble();
            double translation1value=alleditsspher[3]->text().toDouble();
            double translation2value=alleditsspher[4]->text().toDouble(); 
            double translation3value=alleditsspher[5]->text().toDouble();
            relative_posvalue={0,0,0};
            std::vector<double> axisvectorvalue={0,0,0};
            std::vector<std::vector<std::vector<double>>> move_setting_value={{{initialtranslation1value,translation1value,stepnum_value,0.0}},{{initialtranslation2value,translation2value,stepnum_value,0.0}},{{initialtranslation3value,translation3value,stepnum_value,0.0}}};
            setmodelwin->getRunmodel()->getModel()->getparm()->addjoint(jointnameEdit->text().toStdString(), setmodelwin->getRunmodel()->getModel()->getparm()->getbodyindex(selectedValuebody-1)->getname(), joint::alljoint_type[selectedValuetype], relative_posvalue, axisvectorvalue, move_setting_value, {},selectedValuerotateallbody);
        }

        if(setmodelwin->getRunmodel()->getModel()->getparm()->getn_joints()>jointbuttons.size()){
            newjointbutton->setVisible(false);
            for(int i=0;i<jointbuttons.size();i++){
                jointbuttons[i]->setStyleSheet("QPushButton { color: black; background-color: white;}");
            }
            std::string jointnewbuttonname=jointnameEdit->text().toStdString();
            QPushButton* jointaddnewbutton= new QPushButton(QString::fromStdString(jointnewbuttonname), this);
            jointaddnewbutton->setStyleSheet("QPushButton { color: black; background-color: #CCCCCC;font-weight: bold; border: 2px solid #CCCCCC;}");
            jointaddnewbutton->setGeometry(jointbuttons.size()*70, 50, 70, 50);
            jointaddnewbutton->setVisible(true);
            plusbutton->setGeometry(jointbuttons.size()*70+70, 50, 70, 50);
            jointbuttons.push_back(jointaddnewbutton);
            int buttonsize=setmodelwin->getRunmodel()->getModel()->getparm()->getn_joints()-1;
            connect(jointbuttons[buttonsize], &QPushButton::clicked, this, [this, buttonsize]() {
                showjointsetting(buttonsize);
            });
        }
    }
}

void jointpage::deletesetting(){
    Parm* parm=setmodelwin->getRunmodel()->getModel()->getparm();
    int index= parm->deletejoint(jointnameEdit->text().toStdString());
    if(index<0){
        errorbox("not such joint");
    }
    else{
        if(newjointbutton->isVisible()){
            newjointbutton->setGeometry(parm->getn_joints()*70, 50, 70, 50);
            plusbutton->setGeometry(parm->getn_joints()*70+70, 50, 50, 50);
        }
        else{
            plusbutton->setGeometry(parm->getn_joints()*70, 50, 50, 50);
        }
        delete jointbuttons[index];
        jointbuttons.erase(jointbuttons.begin() + index);
        for(int i=0;i<jointbuttons.size();i++){
            jointbuttons[i]->setGeometry(i*70, 50, 70, 50);
            connect(jointbuttons[i], &QPushButton::clicked, this, [this, i]() {
            showjointsetting(i);
        });
        }
        if(parm->getn_joints()>0){
            showjointsetting(0);
        }
        else{
            newjointbutton->setVisible(true);
            newjointbutton->setGeometry(0, 50, 70, 50);
            plusbutton->setGeometry(70, 50, 50, 50);
        }
                
    }
}

void jointpage::plusbuttonsetting(){
    if(!newjointbutton->isVisible()){
        newjointbutton->setVisible(true);
        newjointbutton->setStyleSheet("QPushButton { color: black; background-color: #CCCCCC;font-weight: bold; border: 2px solid #CCCCCC;}");
        newjointbutton->setGeometry(setmodelwin->getRunmodel()->getModel()->getparm()->getn_joints()*70, 50, 70, 50);
        plusbutton->setGeometry(setmodelwin->getRunmodel()->getModel()->getparm()->getn_joints()*70+70, 50, 50, 50);
        setalltextedit("", "", "", {0.0,0.0,0.0}, setmodelwin->getRunmodel()->getModel()->getparm()->getn_joints(),0,0);
        for(int i=0;i<jointbuttons.size();i++){
            jointbuttons[i]->setStyleSheet("QPushButton { color: black; background-color: white;}");
        }
    }
}

void jointpage::newjointbuttonsetting(){
    for(int i=0;i<jointbuttons.size();i++){
        jointbuttons[i]->setStyleSheet("QPushButton { color: black; background-color: white;}");
    }
    newjointbutton->setStyleSheet("QPushButton { color: black; background-color: #CCCCCC;font-weight: bold; border: 2px solid #CCCCCC;}");
    setalltextedit("", "", "", {0.0,0.0,0.0}, setmodelwin->getRunmodel()->getModel()->getparm()->getn_joints(),0,0);
}

void jointpage::showjointsetting(int index){
    Joint=setmodelwin->getRunmodel()->getModel()->getparm()->getjointindex(index);
    setalltextedit(Joint->getname(), Joint->getbodyname(), Joint->getjoint_type(), Joint->getrelative_pos(), index, Joint->getjoint_stepnum(), Joint->getmove_all_body());
    for(int i=0;i<jointbuttons.size();i++){
        if(index==i){
            jointbuttons[i]->setStyleSheet("QPushButton { color: black; background-color: #CCCCCC;font-weight: bold; border: 2px solid #CCCCCC;}");
        }
        else{
            jointbuttons[i]->setStyleSheet("QPushButton { color: black; background-color: white;}");
        }
    }
    if(newjointbutton->isVisible()){
        newjointbutton->setStyleSheet("QPushButton { color: black; background-color: white;}");
    }
}

void jointpage::handleButtonClickedbody(QAbstractButton* button){
    selectedValuebody = buttonGroupbody->id(button);
}

void jointpage::handleButtonClickedmoveallbody(QAbstractButton* button){
    selectedValuerotateallbody = buttonGrouprotateallbody->id(button);
}

void jointpage::handleButtonClickedtype(QAbstractButton* button){
    selectedValuetype = buttonGrouptype->id(button);
    stackedWidget->setCurrentIndex(selectedValuetype);
}