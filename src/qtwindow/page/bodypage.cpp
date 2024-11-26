/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#include "bodypage.h"
#include "../setmodelwindow.h"


bodypage::bodypage(setmodelwindow *setmodelwin,QWidget *parent):
    QWidget(parent),
    setmodelwin(setmodelwin)
{
    std::string bodyname="";
    std::string parentbodyname="fix_space";
    std::string rotationaxis_x="";
    std::string rotationaxis_y="";
    std::string rotationaxis_z="";
    std::string init_rotationangle="";
    std::string positionaxis_x="";
    std::string positionaxis_y="";
    std::string positionaxis_z="";

    std::string shape_name="";
    std::string a_stringvalue="0";
    std::string b_stringvalue="0";
    std::string c_stringvalue="0";
    std::string length_stringvalue="0";
    std::string radius_stringvalue="0";

    rectangle = new QFrame(this);
    rectangle->setGeometry(0, 100, setmodelwin->width(), setmodelwin->width()+1000);
    rectangle->setStyleSheet("background-color: #CCCCCC;");

    int bodynum=setmodelwin->getRunmodel()->getModel()->getparm()->getn_bodies();
    plusbutton= new QPushButton("+", this);
    plusbutton->setStyleSheet("QPushButton { color: black; background-color: white;}");
    connect(plusbutton, &QPushButton::clicked, this, &bodypage::plusbuttonsetting);

    newbodybutton= new QPushButton("new", this);
    newbodybutton->setStyleSheet("QPushButton { color: black; background-color: #CCCCCC;font-weight: bold; border: 2px solid #CCCCCC;}");
    newbodybutton->setGeometry(0, 50, 70, 50);
    connect(newbodybutton, &QPushButton::clicked, this, &bodypage::newbodybuttonsetting);

    if(bodynum){ 
        for(int i=0;i<bodynum;i++){
            QPushButton* bodybutton= new QPushButton(QString::fromStdString(setmodelwin->getRunmodel()->getModel()->getparm()->getbodyindex(i)->getname()), this);
            bodybutton->setStyleSheet("QPushButton { color: black; background-color: white;}");
            bodybutton->setGeometry(i*70, 50, 70, 50);
            bodybuttons.push_back(bodybutton);
        }
        plusbutton->setGeometry(bodynum*70, 50, 50, 50);
        bodybuttons[0]->setStyleSheet("QPushButton { color: black; background-color: #CCCCCC;font-weight: bold; border: 2px solid #CCCCCC;}");
        
        bodyname=setmodelwin->getRunmodel()->getModel()->getparm()->getbodyindex(0)->getname();
        parentbodyname=setmodelwin->getRunmodel()->getModel()->getparm()->getbodyindex(0)->getparent()->getname();
        bodybasic* bodybasicfirstbody=setmodelwin->getRunmodel()->getModel()->getparm()->getbodyindex(0)->getbodybasic();
        std::vector<double> naxisfirstbody=bodybasicfirstbody->getinitialsetting_naxis();
        rotationaxis_x=doubletostring(naxisfirstbody[0]);
        rotationaxis_y=doubletostring(naxisfirstbody[1]);
        rotationaxis_z=doubletostring(naxisfirstbody[2]);
        init_rotationangle=doubletostring(bodybasicfirstbody->getinitialsetting_angle());
        std::vector<double> rhobodyfirstbody=bodybasicfirstbody->getrhobody();
        positionaxis_x=doubletostring(rhobodyfirstbody[0]);
        positionaxis_y=doubletostring(rhobodyfirstbody[1]);
        positionaxis_z=doubletostring(rhobodyfirstbody[2]);

        shape* shapefirstbody=setmodelwin->getRunmodel()->getModel()->getparm()->getbodyindex(0)->getshape();
        shape_name=shapefirstbody->getshapename();
        a_stringvalue=doubletostring(shapefirstbody->geta());
        b_stringvalue=doubletostring(shapefirstbody->getb());
        c_stringvalue=doubletostring(shapefirstbody->getc());
        length_stringvalue=doubletostring(shapefirstbody->getlength());
        radius_stringvalue=doubletostring(shapefirstbody->getradius());
        Body=setmodelwin->getRunmodel()->getModel()->getparm()->getbodyindex(0);

        newbodybutton->setVisible(false);
    }
    else{
        plusbutton->setGeometry(70, 50, 50, 50);
        
    }

    for (int i = 0; i < bodybuttons.size(); i++) {
        connect(bodybuttons[i], &QPushButton::clicked, this, [this, i]() {
            showbodysetting(i);
        });
    }


    int allfontsize=15;
    //bodybasie
    setlabel("Basic information", 10, 110, 20);
    body_nameEdit=settextandlabel("arm name",bodyname, 10, 150, 450, 30, allfontsize);
    parentbody_nameEdit = settextandlabel("parent arm name",parentbodyname, 10, 230, 450, 30, allfontsize);

    buttonGroupgloballocal = new QButtonGroup;
    QRadioButton* radioButtonlocalnull = new QRadioButton(QString::fromStdString("null"), this);
    radioButtonlocalnull->setVisible(false);
    radioButtonsgloballocal.push_back(radioButtonlocalnull);
    buttonGroupgloballocal->addButton(radioButtonsgloballocal[0], -1);
    for(int i=0;i<bodybasic::alltype.size();i++){
        QRadioButton* radioButton = new QRadioButton(QString::fromStdString(bodybasic::alltype[i]), this);
        radioButtonsgloballocal.push_back(radioButton);
        radioButtonsgloballocal[i+1]->setStyleSheet("QRadioButton { color: black; background-color: #CCCCCC;}");
        radioButtonsgloballocal[i+1]->setGeometry(10+i*230, 310, 220, 30);
        buttonGroupgloballocal->addButton(radioButtonsgloballocal[i+1], i);
    }
    connect(buttonGroupgloballocal, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked), this, &bodypage::handleButtonClickedtype);

    radioButtonsgloballocal[1]->setChecked(true);
    selectedValuelocal=0;

    setlabel("rotation axis refers to the parent coordinate", 10, 350, allfontsize);
    rotationrefer=qlabels.back();
    rotationaxisx = settextandlabel("axis x",rotationaxis_x, 10, 375, 140, 30, allfontsize);
    rotationaxisy = settextandlabel("axis y",rotationaxis_y, 165, 375, 140, 30, allfontsize);
    rotationaxisz = settextandlabel("axis z",rotationaxis_z, 320, 375, 140, 30, allfontsize);
    initrotationangle = settextandlabel("rotation angle",init_rotationangle, 10, 455, 140, 30, allfontsize);
    setlabel("position refers to the parent coordinate", 10, 535, allfontsize);
    positionrefer=qlabels.back();
    positionaxisx = settextandlabel("axis x",positionaxis_x, 10, 560, 140, 30, allfontsize);
    positionaxisy = settextandlabel("axis y",positionaxis_y, 165, 560, 140, 30, allfontsize);
    positionaxisz = settextandlabel("axis z",positionaxis_z, 320, 560, 140, 30, allfontsize);

    //shape
    setlabel("Shape information", 660, 110, 20);
    setlabel("shape name", 660, 150, allfontsize);

    buttonGroupshape = new QButtonGroup;
    QRadioButton* radioButtonshapenull = new QRadioButton(QString::fromStdString("null"), this);
    radioButtonshapenull->setVisible(false);
    radioButtonsshape.push_back(radioButtonshapenull);
    buttonGroupshape->addButton(radioButtonsshape[0], -1);
    for(int i=0;i<shape::allshape.size();i++){
        QRadioButton* radioButton = new QRadioButton(QString::fromStdString(shape::allshape[i]), this);
        radioButtonsshape.push_back(radioButton);
        radioButtonsshape[i+1]->setStyleSheet("QRadioButton { color: black; background-color: #CCCCCC;}");
        radioButtonsshape[i+1]->setGeometry(660+i*110, 175, 100, 30);
        buttonGroupshape->addButton(radioButtonsshape[i+1], i);
    }
    connect(buttonGroupshape, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked), this, &bodypage::handleButtonClickedshape);

    int findshape=shape::getshapeindex(shape_name);
    radioButtonsshape[findshape+1]->setChecked(true);
    selectedValueshape=findshape;

    aEdit=settextandlabel("a",a_stringvalue, 660, 224, 450, 30, allfontsize);
    bEdit=settextandlabel("b",b_stringvalue, 660, 298, 450, 30, allfontsize);
    cEdit=settextandlabel("c",c_stringvalue, 660, 372, 450, 30, allfontsize);
    lengthEdit=settextandlabel("length",length_stringvalue, 660, 446, 450, 30, allfontsize);
    radiusEdit=settextandlabel("radius",radius_stringvalue, 660, 520, 450, 30, allfontsize);

    //save button
    savebutton = new QPushButton("Save", this);
    savebutton->setStyleSheet("QPushButton { color: black; background-color: grey;}");
    savebutton->setGeometry(1010, 600, 100, 50);
    connect(savebutton, &QPushButton::clicked, this, &bodypage::savebuttonsetting);

    //delete button
    deletebutton = new QPushButton("Delete body", this);
    deletebutton->setStyleSheet("QPushButton { color: black; background-color: grey;}");
    deletebutton->setGeometry(660, 600, 100, 50);
    connect(deletebutton, &QPushButton::clicked, this, &bodypage::deletebuttonsetting);
}

bodypage::~bodypage(){
    for(int i=0;i<bodybuttons.size();i++){
        delete bodybuttons[i];
    }
    for(int i=0;i<qlabels.size();i++){
        delete qlabels[i];
    }
    for(int i=0;i<radioButtonsshape.size();i++){
        delete radioButtonsshape[i];
    }
    for(int i=0;i<radioButtonsgloballocal.size();i++){
        delete radioButtonsgloballocal[i];
    }
    delete buttonGroupgloballocal;
    delete buttonGroupshape;
    delete savebutton;
    delete deletebutton;
    delete newbodybutton;
    delete plusbutton;
    delete body_nameEdit;
    delete parentbody_nameEdit;
    delete rotationaxisx;
    delete rotationaxisy;
    delete rotationaxisz;
    delete initrotationangle;
    delete positionaxisx;
    delete positionaxisy;
    delete positionaxisz;
    delete aEdit;
    delete bEdit;
    delete cEdit;
    delete lengthEdit;
    delete radiusEdit;
    delete rectangle;
}

QLineEdit* bodypage::settext(const std::string& textdefault, int x, int y, int textwidth, int textheight ,int fontsize) {
    QLineEdit* body_nameEdit = new QLineEdit(this);
    QFont body_nameEditfont = body_nameEdit->font();
    body_nameEditfont.setPointSize(fontsize); 
    body_nameEdit->setFont(body_nameEditfont);
    body_nameEdit->setGeometry(x, y, textwidth, textheight);
    body_nameEdit->setText(QString::fromStdString(textdefault));
    body_nameEdit->setStyleSheet("QLineEdit { color: black; }");
    return body_nameEdit;
}
void bodypage::setlabel(const std::string& labelname, int x, int y ,int fontsize) {
    QLabel *body_namelabel = new QLabel(this);
    QFont body_namefont = body_namelabel->font();
    body_namefont.setPointSize(fontsize); 
    body_namelabel->setFont(body_namefont);
    body_namelabel->setText(QString::fromStdString(labelname)); 
    body_namelabel->move(x, y);
    body_namelabel->setStyleSheet("QLabel { color : black; background-color : #CCCCCC; }");
    qlabels.push_back(body_namelabel);
}

QLineEdit* bodypage::settextandlabel(const std::string& labelname, const std::string& textdefault, int x, int y, int textwidth, int textheight, int fontsize) {
    setlabel(labelname, x, y, fontsize);
    QLineEdit* body_nameEdit = settext(textdefault, x, y+25, textwidth, textheight, fontsize);
    return body_nameEdit;
}

std::string bodypage::doubletostring(double num) {
    std::ostringstream stream;
    stream << num;
    std::string numStr = stream.str();
    return numStr;
}

double bodypage::stringtodouble(std::string numStr) {
    double num;
    std::istringstream stream(numStr);
    stream >> num;
    return num;
}

void bodypage::errorbox(std::string errormessage){
    QMessageBox errorMessage;
    errorMessage.setWindowTitle("error");
    errorMessage.setText(QString::fromStdString(errormessage));
    errorMessage.setIcon(QMessageBox::Critical);
    errorMessage.addButton("Yes", QMessageBox::AcceptRole);
    errorMessage.exec();
}

void bodypage::setalltextedit(const std::string& bodyname, const std::string& parentbodyname, const std::vector<double>& naxis, double rotationangle, const std::vector<double>& rhobody, double a, double b, double c, double length, double radius, const std::string& shapename,int localglobal){
    body_nameEdit->setText(QString::fromStdString(bodyname));
    parentbody_nameEdit->setText(QString::fromStdString(parentbodyname));
    rotationaxisx->setText(QString::fromStdString(doubletostring(naxis[0])));
    rotationaxisy->setText(QString::fromStdString(doubletostring(naxis[1])));
    rotationaxisz->setText(QString::fromStdString(doubletostring(naxis[2])));
    initrotationangle->setText(QString::fromStdString(doubletostring(rotationangle)));
    positionaxisx->setText(QString::fromStdString(doubletostring(rhobody[0])));
    positionaxisy->setText(QString::fromStdString(doubletostring(rhobody[1])));
    positionaxisz->setText(QString::fromStdString(doubletostring(rhobody[2])));
    
    int findshape=shape::getshapeindex(shapename);
    radioButtonsshape[findshape+1]->setChecked(true);
    selectedValueshape=findshape;
    aEdit->setText(QString::fromStdString(doubletostring(a)));
    bEdit->setText(QString::fromStdString(doubletostring(b)));
    cEdit->setText(QString::fromStdString(doubletostring(c)));
    lengthEdit->setText(QString::fromStdString(doubletostring(length)));
    radiusEdit->setText(QString::fromStdString(doubletostring(radius)));
    radioButtonsgloballocal[localglobal+1]->setChecked(true);
    selectedValuelocal = localglobal;
    setlocalglobal();
}

void bodypage::plusbuttonsetting(){
    if(!newbodybutton->isVisible()){
        newbodybutton->setVisible(true);
        newbodybutton->setStyleSheet("QPushButton { color: black; background-color: #CCCCCC;font-weight: bold; border: 2px solid #CCCCCC;}");
        newbodybutton->setGeometry(setmodelwin->getRunmodel()->getModel()->getparm()->getn_bodies()*70, 50, 70, 50);
        plusbutton->setGeometry(setmodelwin->getRunmodel()->getModel()->getparm()->getn_bodies()*70+70, 50, 50, 50);
        setalltextedit("", "fix_space", {0.0,0.0,0.0}, 0.0, {0.0,0.0,0.0}, 0.0, 0.0, 0.0, 0.0, 0.0, "",-1);
        for(int i=0;i<bodybuttons.size();i++){
            bodybuttons[i]->setStyleSheet("QPushButton { color: black; background-color: white;}");
        }
    }
}

void bodypage::savebuttonsetting(){
    body* findparentbody=setmodelwin->getRunmodel()->getModel()->getparm()->findbody(parentbody_nameEdit->text().toStdString());
    if(findparentbody==nullptr || selectedValueshape<0 || selectedValuelocal<0){
        if(findparentbody==nullptr){
            errorbox("please check parent body name, parent does not exist");
        }
        if(selectedValueshape<0){
            errorbox("please check shape name, it does not exist");
        }
        if(selectedValuelocal<0){
            errorbox("please select local or global coordinate");
        }
    }
    else{
        std::vector<double> naxisvalue={rotationaxisx->text().toDouble(),rotationaxisy->text().toDouble(),rotationaxisz->text().toDouble()};
        std::vector<double> rhobodyvalue={positionaxisx->text().toDouble(),positionaxisy->text().toDouble(),positionaxisz->text().toDouble()};
        setmodelwin->getRunmodel()->getModel()->getparm()->addbody(body_nameEdit->text().toStdString(), parentbody_nameEdit->text().toStdString(), naxisvalue, initrotationangle->text().toDouble(), rhobodyvalue, aEdit->text().toDouble(), bEdit->text().toDouble(), cEdit->text().toDouble(), lengthEdit->text().toDouble(), radiusEdit->text().toDouble(), shape::allshape[selectedValueshape],selectedValuelocal);
        if(setmodelwin->getRunmodel()->getModel()->getparm()->getn_bodies()>bodybuttons.size()){
            newbodybutton->setVisible(false);
            for(int i=0;i<bodybuttons.size();i++){
                bodybuttons[i]->setStyleSheet("QPushButton { color: black; background-color: white;}");
            }
            std::string bodynewbuttonname=body_nameEdit->text().toStdString();
            QPushButton* bodyaddnewbutton= new QPushButton(QString::fromStdString(bodynewbuttonname), this);
            bodyaddnewbutton->setStyleSheet("QPushButton { color: black; background-color: #CCCCCC;font-weight: bold; border: 2px solid #CCCCCC;}");
            bodyaddnewbutton->setGeometry(bodybuttons.size()*70, 50, 70, 50);
            bodyaddnewbutton->setVisible(true);
            plusbutton->setGeometry(bodybuttons.size()*70+70, 50, 70, 50);
            bodybuttons.push_back(bodyaddnewbutton);
            int buttonsize=setmodelwin->getRunmodel()->getModel()->getparm()->getn_bodies()-1;
                connect(bodybuttons[buttonsize], &QPushButton::clicked, this, [this, buttonsize]() {
                    showbodysetting(buttonsize);
            });
        }
    }
}

void bodypage::deletebuttonsetting(){
    Parm* parm=setmodelwin->getRunmodel()->getModel()->getparm();
    int index= parm->deletebody(body_nameEdit->text().toStdString());
    if(index<0){
        errorbox("not such body");
    }
    else{
        if(newbodybutton->isVisible()){
            newbodybutton->setGeometry(parm->getn_bodies()*70, 50, 70, 50);
            plusbutton->setGeometry(parm->getn_bodies()*70+70, 50, 50, 50);
        }
        else{
            plusbutton->setGeometry(parm->getn_bodies()*70, 50, 50, 50);
        }
        delete bodybuttons[index];
        bodybuttons.erase(bodybuttons.begin() + index);
        for(int i=0;i<bodybuttons.size();i++){
            bodybuttons[i]->setGeometry(i*70, 50, 70, 50);
            connect(bodybuttons[i], &QPushButton::clicked, this, [this, i]() {
            showbodysetting(i);
        });
        }
        if(parm->getn_bodies()>0){
            showbodysetting(0);
        }
        else{
            newbodybutton->setVisible(true);
            newbodybutton->setGeometry(0, 50, 70, 50);
            plusbutton->setGeometry(70, 50, 50, 50);
        }
                
    }
}

void bodypage::newbodybuttonsetting(){
    for(int i=0;i<bodybuttons.size();i++){
        bodybuttons[i]->setStyleSheet("QPushButton { color: black; background-color: white;}");
    }
    newbodybutton->setStyleSheet("QPushButton { color: black; background-color: #CCCCCC;font-weight: bold; border: 2px solid #CCCCCC;}");
    setalltextedit("", "fix_space", {0.0,0.0,0.0}, 0.0, {0.0,0.0,0.0}, 0.0, 0.0, 0.0, 0.0, 0.0, "",-1);
}

void bodypage::showbodysetting(int index){
    Body=setmodelwin->getRunmodel()->getModel()->getparm()->getbodyindex(index);
    setalltextedit(Body->getname(), Body->getparent()->getname(), Body->getbodybasic()->getinitialsetting_naxis(), Body->getbodybasic()->getinitialsetting_angle(), Body->getbodybasic()->getrhobody(), Body->getshape()->geta(), Body->getshape()->getb(), Body->getshape()->getc(), Body->getshape()->getlength(), Body->getshape()->getradius(), Body->getshape()->getshapename(),0);
    for(int i=0;i<bodybuttons.size();i++){
        if(index==i){
            bodybuttons[i]->setStyleSheet("QPushButton { color: black; background-color: #CCCCCC;font-weight: bold; border: 2px solid #CCCCCC;}");
        }
        else{
            bodybuttons[i]->setStyleSheet("QPushButton { color: black; background-color: white;}");
        }
    }
    if(newbodybutton->isVisible()){
        newbodybutton->setStyleSheet("QPushButton { color: black; background-color: white;}");
    }
}

void bodypage::handleButtonClickedshape(QAbstractButton* button){
    selectedValueshape = buttonGroupshape->id(button);
}

void bodypage::setlocalglobal(){
    
        if(selectedValuelocal==0){
            positionrefer->setText(QString::fromStdString("position refers to the parent coordinate")); 
            rotationrefer->setText(QString::fromStdString("rotation axis refers to the parent coordinate")); 
            if(Body!=nullptr){
                std::vector<double> naxis_local=Body->getbodybasic()->getinitialsetting_naxis();
                rotationaxisx->setText(QString::fromStdString(doubletostring(naxis_local[0])));
                rotationaxisy->setText(QString::fromStdString(doubletostring(naxis_local[1])));
                rotationaxisz->setText(QString::fromStdString(doubletostring(naxis_local[2])));
                std::vector<double> position_local=Body->getbodybasic()->getrhobody();
                positionaxisx->setText(QString::fromStdString(doubletostring(position_local[0])));
                positionaxisy->setText(QString::fromStdString(doubletostring(position_local[1])));
                positionaxisz->setText(QString::fromStdString(doubletostring(position_local[2])));
            }
        }
        if(selectedValuelocal==1){
            positionrefer->setText(QString::fromStdString("position refers to the fix space")); 
            rotationrefer->setText(QString::fromStdString("rotation axis refers to the fix space"));
            if(Body!=nullptr){
                std::vector<double>  naxis_global=Body->getbodyglobalnaxis();
                rotationaxisx->setText(QString::fromStdString(doubletostring(naxis_global[0])));
                rotationaxisy->setText(QString::fromStdString(doubletostring(naxis_global[1])));
                rotationaxisz->setText(QString::fromStdString(doubletostring(naxis_global[2])));
                
                std::vector<double> position_global=Body->getbodybasic()->getposition();
                positionaxisx->setText(QString::fromStdString(doubletostring(position_global[0])));
                positionaxisy->setText(QString::fromStdString(doubletostring(position_global[1])));
                positionaxisz->setText(QString::fromStdString(doubletostring(position_global[2])));
            }
        }

}

void bodypage::handleButtonClickedtype(QAbstractButton* button){
    selectedValuelocal = buttonGroupgloballocal->id(button);
    setlocalglobal();
}
