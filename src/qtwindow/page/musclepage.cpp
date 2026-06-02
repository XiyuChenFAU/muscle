/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
27.10.2023
Xiyu Chen

*/

#include "musclepage.h"
#include "../setmodelwindow.h"
#include <QScrollArea>
#include <QScrollBar>

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
    muscleButtonScrollArea = new QScrollArea(this);
    muscleButtonScrollArea->setFrameShape(QFrame::NoFrame);
    muscleButtonScrollArea->setGeometry(0, 50, 1400, 60);
    muscleButtonScrollArea->setWidgetResizable(false);
    muscleButtonScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    muscleButtonScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    muscleButtonScrollArea->setStyleSheet(
        "QScrollArea { background: #CCCCCC; border: none; }"
        "QScrollArea > QWidget > QWidget { background: #CCCCCC; }"
        "QScrollBar:horizontal { height: 10px; background: #CCCCCC; }"
        "QScrollBar::handle:horizontal { background: white; border-radius: 5px; }"
        "QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal { width: 0px; }"
    );
    muscleButtonScrollArea->viewport()->setStyleSheet("background: #CCCCCC;");
    muscleButtonContent = new QWidget();
    muscleButtonContent->setGeometry(0, 0, std::max(120, (musclenum + 2) * 70), 50);
    muscleButtonScrollArea->setWidget(muscleButtonContent);

    bodyListScrollArea = new QScrollArea(this);
    bodyListScrollArea->setGeometry(0, 270, 1390, 500);
    bodyListScrollArea->setFrameShape(QFrame::NoFrame);
    bodyListScrollArea->setWidgetResizable(false);
    bodyListScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    bodyListScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    bodyListScrollArea->setStyleSheet(
        "QScrollArea { background: #CCCCCC; border: none; }"
        "QScrollArea > QWidget > QWidget { background: #CCCCCC; }"
        "QScrollBar:vertical { width: 10px; background: #CCCCCC; }"
        "QScrollBar::handle:vertical { background: white; border-radius: 5px; }"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0px; }"
    );
    bodyListScrollArea->viewport()->setStyleSheet("background: #CCCCCC;");
    bodyListContent = new QWidget();
    bodyListContent->setStyleSheet("background-color: #CCCCCC;");
    bodyListScrollArea->setWidget(bodyListContent);

    plusbutton= new QPushButton("+", muscleButtonContent);
    plusbutton->setStyleSheet("QPushButton { color: black; background-color: white;}");
    connect(plusbutton, &QPushButton::clicked, this, &musclepage::plusbuttonsetting);

    newmusclebutton= new QPushButton("new", muscleButtonContent);
    newmusclebutton->setStyleSheet("QPushButton { color: black; background-color: #CCCCCC;font-weight: bold; border: 2px solid #CCCCCC;}");
    newmusclebutton->setGeometry(0, 0, 70, 50);
    connect(newmusclebutton, &QPushButton::clicked, this, &musclepage::newmusclebuttonsetting);

    if(musclenum){ 
        for(int i=0;i<musclenum;i++){
            QPushButton* musclebutton= new QPushButton(QString::fromStdString(setmodelwin->getRunmodel()->getModel()->getparm()->getmuscleindex(i)->getname()), muscleButtonContent);
            musclebutton->setStyleSheet("QPushButton { color: black; background-color: white;}");
            musclebutton->setGeometry(i*70, 0, 70, 50);
            musclebuttons.push_back(musclebutton);
        }
        plusbutton->setGeometry(musclenum*70, 0, 50, 50);
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
        Muscle=setmodelwin->getRunmodel()->getModel()->getparm()->getmuscleindex(0);
        newmusclebutton->setVisible(false);
    }
    else{
        plusbutton->setGeometry(70, 0, 50, 50);
        
    }

    for (int i = 0; i < musclebuttons.size(); i++) {
        connect(musclebuttons[i], &QPushButton::clicked, this, [this, i]() {
            showmusclesetting(i);
        });
    }
    
    int allfontsize=15;
    //bodybasie
    setlabel("Muscle information", 10, 110,20);

    musclenameEdit=settextandlabel("muscle name",musclenamestring, 10, 150, 340, 30, allfontsize);
    nodenumEdit=settextandlabel("node number",nodenumstring, 450, 150, 340, 30, allfontsize);
    setlabel("considered body list", 910, 270, allfontsize);
    viapointbutton = new QPushButton("viapoint setting", this);
    viapointbutton->setGeometry(910, 175, 160, 32);
    viapointbutton->setStyleSheet("QPushButton { color: black; background-color: white; }");
    connect(viapointbutton, &QPushButton::clicked, this, &musclepage::openViaPointSetting);

    buttonGroupgloballocal = new QButtonGroup;
    QRadioButton* radioButtonlocalnull = new QRadioButton(QString::fromStdString("null"), this);
    radioButtonlocalnull->setVisible(false);
    radioButtonsgloballocal.push_back(radioButtonlocalnull);
    buttonGroupgloballocal->addButton(radioButtonsgloballocal[0], -1);
    for(int i=0;i<bodybasic::alltype.size();i++){
        QRadioButton* radioButton = new QRadioButton(QString::fromStdString(bodybasic::alltype[i]), this);
        radioButtonsgloballocal.push_back(radioButton);
        radioButtonsgloballocal[i+1]->setStyleSheet("QRadioButton { color: black; background-color: #CCCCCC;}");
        radioButtonsgloballocal[i+1]->setGeometry(10+i*230, 230, 220, 30);
        buttonGroupgloballocal->addButton(radioButtonsgloballocal[i+1], i);
    }
    connect(buttonGroupgloballocal, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked), this, &musclepage::handleButtonClickedtype);

    radioButtonsgloballocal[1]->setChecked(true);
    selectedValuelocal=0;

    setlabel("origin body name", 10, 270,allfontsize);
    setlabel("origin position refer body coordinate", 170, 270, allfontsize);
    positionrefer_o=qlabels.back();
    rhooaxisxEdit = settextandlabel("axis x",rhoostring_axis_x, 230, 305, 100, 30, allfontsize);
    rhooaxisyEdit = settextandlabel("axis y",rhoostring_axis_y, 230, 390, 100, 30, allfontsize);
    rhooaxiszEdit = settextandlabel("axis z",rhoostring_axis_z, 230, 475, 100, 30, allfontsize);

    setlabel("insertion body name", 450, 270,allfontsize);
    setlabel("insertion position refer body coordinate", 620, 270, allfontsize);
    positionrefer_i=qlabels.back();
    rhoiaxisxEdit = settextandlabel("axis x",rhoistring_axis_x, 680, 305, 100, 30, allfontsize);
    rhoiaxisyEdit = settextandlabel("axis y",rhoistring_axis_y, 680, 390, 100, 30, allfontsize);
    rhoiaxiszEdit = settextandlabel("axis z",rhoistring_axis_z, 680, 475, 100, 30, allfontsize);

    updatevalue();
    loadViaPointsFromMuscle();

    //save button
    savebutton = new QPushButton("Save", this);
    savebutton->setStyleSheet("QPushButton { color: black; background-color: grey;}");
    savebutton->setGeometry(680, 570, 100, 50);
    connect(savebutton, &QPushButton::clicked, this, &musclepage::savebuttonsetting);

    //delete button
    deletebutton = new QPushButton("Delete muscle", this);
    deletebutton->setStyleSheet("QPushButton { color: black; background-color: grey;}");
    deletebutton->setGeometry(680, 660, 100, 50);
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
    for(int i=0;i<radioButtonsgloballocal.size();i++){
        delete radioButtonsgloballocal[i];
    }
    delete buttonGroupgloballocal;
    delete buttonGroupo;
    delete buttonGroupi;
    
    delete savebutton;
    delete deletebutton;
    delete newmusclebutton;
    delete plusbutton;
    delete viapointbutton;

    delete musclenameEdit;
    delete nodenumEdit;

    delete rhooaxisxEdit;
    delete rhooaxisyEdit;
    delete rhooaxiszEdit;

    delete rhoiaxisxEdit;
    delete rhoiaxisyEdit;
    delete rhoiaxiszEdit;
    delete rectanglemain;
    delete muscleButtonScrollArea;
    delete bodyListScrollArea;

    for(int i=0; i<checkBoxes_body.size(); i++){
        delete checkBoxes_body[i];
    }
    checkBoxes_body.clear();
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

void musclepage::setalltextedit(const std::vector<double>& rho_o, const std::string& rhoo_bodyname, const std::vector<double>& rho_i, const std::string& rhoi_bodyname, const std::string& name, int nodenum, int localglobal, const std::vector<std::string>& selectedBodies_value){
    
    musclenameEdit->setText(QString::fromStdString(name));
    nodenumEdit->setText(QString::fromStdString(std::to_string(nodenum)));

    int findbodyo=setmodelwin->getRunmodel()->getModel()->getparm()->findbodyindex(rhoo_bodyname);
    radioButtonso[findbodyo+2]->setChecked(true);
    selectedValueo=findbodyo+1;
    rhooaxisxEdit->setText(QString::fromStdString(doubletostring(rho_o[0])));
    rhooaxisyEdit->setText(QString::fromStdString(doubletostring(rho_o[1])));
    rhooaxiszEdit->setText(QString::fromStdString(doubletostring(rho_o[2])));

    int findbodyi=setmodelwin->getRunmodel()->getModel()->getparm()->findbodyindex(rhoi_bodyname);
    radioButtonsi[findbodyi+2]->setChecked(true);
    selectedValuei=findbodyi+1;

    rhoiaxisxEdit->setText(QString::fromStdString(doubletostring(rho_i[0])));
    rhoiaxisyEdit->setText(QString::fromStdString(doubletostring(rho_i[1])));
    rhoiaxiszEdit->setText(QString::fromStdString(doubletostring(rho_i[2])));

    for(int i=0; i<checkBoxes_body.size(); i++){
        checkBoxes_body[i]->setChecked(0);
    }

    for(int i=0; i<selectedBodies_value.size(); i++){
        int body_index = setmodelwin->getRunmodel()->getModel()->getparm()->findbodyindex(selectedBodies_value[i]);
        checkBoxes_body[body_index]->setChecked(1);
    }

    radioButtonsgloballocal[localglobal+1]->setChecked(true);
    selectedValuelocal=localglobal;

    setlocalglobal();
}

void musclepage::plusbuttonsetting(){
    if(!confirmViaPointChanges()){
        return;
    }
    if(!newmusclebutton->isVisible()){
        newmusclebutton->setVisible(true);
        newmusclebutton->setStyleSheet("QPushButton { color: black; background-color: #CCCCCC;font-weight: bold; border: 2px solid #CCCCCC;}");
        muscleButtonContent->resize((setmodelwin->getRunmodel()->getModel()->getparm()->getn_muscles()+2)*70, 50);
        newmusclebutton->setGeometry(setmodelwin->getRunmodel()->getModel()->getparm()->getn_muscles()*70, 0, 70, 50);
        plusbutton->setGeometry(setmodelwin->getRunmodel()->getModel()->getparm()->getn_muscles()*70+70, 0, 50, 50);
        std::vector<std::string> init_consider_body={};
        pendingViaPoints.clear();
        viaPointDirty=false;
        setalltextedit({0.0,0.0,0.0}, "", {0.0,0.0,0.0}, "", "", 0, -1,init_consider_body);
        for(int i=0;i<musclebuttons.size();i++){
            musclebuttons[i]->setStyleSheet("QPushButton { color: black; background-color: white;}");
        }
    }
}

void musclepage::savebuttonsetting(){
    if(selectedValueo<0 || selectedValuei<0 || selectedValuelocal<0){
        if(selectedValueo<0 ){
            errorbox("please check origin body name, it does not exist");
        }
        if(selectedValuei<0){
            errorbox("please check insertion body name, it does not exist");
        }
        if(selectedValuelocal<0){
            errorbox("please select local or global");
        }
    }
    else{
        std::vector<double> rhooaxisvalue={rhooaxisxEdit->text().toDouble(),rhooaxisyEdit->text().toDouble(),rhooaxiszEdit->text().toDouble()};
        std::vector<double> rhoiaxisvalue={rhoiaxisxEdit->text().toDouble(),rhoiaxisyEdit->text().toDouble(),rhoiaxiszEdit->text().toDouble()};
        std::vector<std::string> consider_body_list_value={};
        for(int i=0; i<selectedBodies.size(); i++){
            consider_body_list_value.push_back(setmodelwin->getRunmodel()->getModel()->getparm()->getbodyindex(selectedBodies[i])->getname());
        }
        setmodelwin->getRunmodel()->getModel()->getparm()->addmuscle(rhooaxisvalue, setmodelwin->getRunmodel()->getModel()->getparm()->getbodyindex(selectedValueo-1)->getname(), rhoiaxisvalue, setmodelwin->getRunmodel()->getModel()->getparm()->getbodyindex(selectedValuei-1)->getname(), musclenameEdit->text().toStdString(), nodenumEdit->text().toInt(),selectedValuelocal,{}, {}, consider_body_list_value);//set via point later
        applyPendingViaPointsToMuscle();
        viaPointDirty=false;
        if(setmodelwin->getRunmodel()->getModel()->getparm()->getn_muscles()>musclebuttons.size()){
            newmusclebutton->setVisible(false);
            for(int i=0;i<musclebuttons.size();i++){
                musclebuttons[i]->setStyleSheet("QPushButton { color: black; background-color: white;}");
            }
            std::string musclenewbuttonname=musclenameEdit->text().toStdString();
            QPushButton* muscleaddnewbutton= new QPushButton(QString::fromStdString(musclenewbuttonname), muscleButtonContent);
            muscleaddnewbutton->setStyleSheet("QPushButton { color: black; background-color: #CCCCCC;font-weight: bold; border: 2px solid #CCCCCC;}");
            muscleaddnewbutton->setGeometry(musclebuttons.size()*70, 0, 70, 50);
            muscleaddnewbutton->setVisible(true);
            plusbutton->setGeometry(musclebuttons.size()*70+70, 0, 70, 50);
            muscleButtonContent->resize((musclebuttons.size()+2)*70, 50);
            musclebuttons.push_back(muscleaddnewbutton);
            int buttonsize=setmodelwin->getRunmodel()->getModel()->getparm()->getn_muscles()-1;
            connect(musclebuttons[buttonsize], &QPushButton::clicked, this, [this, buttonsize]() {
                showmusclesetting(buttonsize);
            });
        }
    }
}

void musclepage::deletebuttonsetting(){
    if(!confirmViaPointChanges()){
        return;
    }
    Parm* parm=setmodelwin->getRunmodel()->getModel()->getparm();
    int index= parm->deletemuscle(musclenameEdit->text().toStdString());
    if(index<0){
        errorbox("not such muscle");
    }
    else{
        if(newmusclebutton->isVisible()){
            newmusclebutton->setGeometry(parm->getn_muscles()*70, 0, 70, 50);
            plusbutton->setGeometry(parm->getn_muscles()*70+70, 0, 50, 50);
        }
        else{
            plusbutton->setGeometry(parm->getn_muscles()*70, 0, 50, 50);
        }
        delete musclebuttons[index];
        musclebuttons.erase(musclebuttons.begin() + index);
        for(int i=0;i<musclebuttons.size();i++){
            musclebuttons[i]->setGeometry(i*70, 0, 70, 50);
            connect(musclebuttons[i], &QPushButton::clicked, this, [this, i]() {
            showmusclesetting(i);
        });
        }
        if(parm->getn_muscles()>0){
            showmusclesetting(0);
        }
        else{
            newmusclebutton->setVisible(true);
            newmusclebutton->setGeometry(0, 0, 70, 50);
            plusbutton->setGeometry(70, 0, 50, 50);
        }
        muscleButtonContent->resize((parm->getn_muscles()+2)*70, 50);
                
    }
}

void musclepage::newmusclebuttonsetting(){
    if(!confirmViaPointChanges()){
        return;
    }
    for(int i=0;i<musclebuttons.size();i++){
        musclebuttons[i]->setStyleSheet("QPushButton { color: black; background-color: white;}");
    }
    newmusclebutton->setStyleSheet("QPushButton { color: black; background-color: #CCCCCC;font-weight: bold; border: 2px solid #CCCCCC;}");
    std::vector<std::string> init_consider_body={};
    pendingViaPoints.clear();
    viaPointDirty=false;
    setalltextedit({0.0,0.0,0.0}, "", {0.0,0.0,0.0}, "", "", 0, -1,init_consider_body);
}

void musclepage::showmusclesetting(int index){
    if(!confirmViaPointChanges()){
        return;
    }
    Muscle=setmodelwin->getRunmodel()->getModel()->getparm()->getmuscleindex(index);
    loadViaPointsFromMuscle();
    std::vector<std::string> consider_list=Muscle->get_consider_body_list();
    if(consider_list.empty()){
        std::vector<body*> allbody=setmodelwin->getRunmodel()->getModel()->getparm()->getallbody();
        for(int i=1; i<allbody.size();i++){
            consider_list.push_back(allbody[i]->getname());
        }
    }
    setalltextedit(Muscle->getrho_o(), Muscle->getrhoo_bodyname(), Muscle->getrho_i(), Muscle->getrhoi_bodyname(), Muscle->getname(), Muscle->getnodenum(), 0,consider_list);
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

bool musclepage::confirmViaPointChanges(){
    if(!viaPointDirty){
        return true;
    }
    QMessageBox messageBox(this);
    messageBox.setWindowTitle("viapoint setting");
    messageBox.setText("Via point setting has unsaved changes. Save them to this muscle?");
    messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    messageBox.setDefaultButton(QMessageBox::Yes);
    int result = messageBox.exec();
    if(result == QMessageBox::Cancel){
        return false;
    }
    if(result == QMessageBox::Yes){
        savebuttonsetting();
    } else {
        loadViaPointsFromMuscle();
        viaPointDirty=false;
    }
    return true;
}

void musclepage::loadViaPointsFromMuscle(){
    pendingViaPoints.clear();
    if(Muscle==nullptr){
        viaPointDirty=false;
        return;
    }
    for(viapoint* viaPoint : Muscle->getvia_point_list()){
        ViapointDraft draft;
        draft.cutoff = viaPoint->get_cutoff();
        draft.alpha = viaPoint->get_alpha_value();
        draft.rho = viaPoint->get_rho();
        draft.gamma = viaPoint->get_gamma_node(0);
        draft.global = 0;
        if(viaPoint->get_ref_body()!=nullptr){
            draft.bodyName = viaPoint->get_ref_body()->getname();
        }
        pendingViaPoints.push_back(draft);
    }
    viaPointDirty=false;
}

void musclepage::applyPendingViaPointsToMuscle(){
    std::string muscleName = musclenameEdit->text().toStdString();
    std::vector<std::string> bodyNames;
    std::vector<std::vector<double>> rhoValues;
    std::vector<std::vector<double>> etaValues;
    std::vector<double> alphaValues;
    std::vector<double> cutoffValues;
    std::vector<int> globalValues;
    for(const ViapointDraft &draft : pendingViaPoints){
        const std::vector<double> &coordinate = draft.global ? draft.gamma : draft.rho;
        if(draft.bodyName.empty() || coordinate.size() < 3){
            continue;
        }
        bodyNames.push_back(draft.bodyName);
        rhoValues.push_back(coordinate);
        etaValues.push_back({0.0});
        alphaValues.push_back(draft.alpha);
        cutoffValues.push_back(draft.cutoff);
        globalValues.push_back(draft.global);
    }
    setmodelwin->getRunmodel()->getModel()->getparm()->set_muscle_viapoint_node(muscleName, bodyNames, rhoValues, etaValues, alphaValues, cutoffValues, globalValues);
    for(int i=0;i<setmodelwin->getRunmodel()->getModel()->getparm()->getn_muscles();i++){
        muscle* candidate = setmodelwin->getRunmodel()->getModel()->getparm()->getmuscleindex(i);
        if(candidate->getname()==muscleName){
            Muscle=candidate;
            break;
        }
    }
}

void musclepage::openViaPointSetting(){
    viapointsetting dialog(setmodelwin->getRunmodel()->getModel()->getparm(), &pendingViaPoints, this);
    if(dialog.exec()==QDialog::Accepted){
        viaPointDirty=true;
    }
}

void musclepage::handleButtonClickedo(QAbstractButton* button){
    selectedValueo = buttonGroupo->id(button);
}

void musclepage::handleButtonClickedi(QAbstractButton* button){
    selectedValuei = buttonGroupi->id(button);
}

void musclepage::setlocalglobal(){
        std::vector<node*> allnodes=Muscle->get_allnodes();
        if(selectedValuelocal==0){
            positionrefer_o->setText(QString::fromStdString("origin position refer body coordinate")); 
            positionrefer_i->setText(QString::fromStdString("insertion position refer body coordinate")); 
            if(Muscle!=nullptr){
                std::vector<double> rho_o_position_local=allnodes[0]->get_rho();
                rhooaxisxEdit->setText(QString::fromStdString(doubletostring(rho_o_position_local[0])));
                rhooaxisyEdit->setText(QString::fromStdString(doubletostring(rho_o_position_local[1])));
                rhooaxiszEdit->setText(QString::fromStdString(doubletostring(rho_o_position_local[2])));
                std::vector<double> rho_i_position_local=allnodes.back()->get_rho();
                rhoiaxisxEdit->setText(QString::fromStdString(doubletostring(rho_i_position_local[0])));
                rhoiaxisyEdit->setText(QString::fromStdString(doubletostring(rho_i_position_local[1])));
                rhoiaxiszEdit->setText(QString::fromStdString(doubletostring(rho_i_position_local[2])));
            }
        }
        if(selectedValuelocal==1){
            positionrefer_o->setText(QString::fromStdString("origin position refer fix space")); 
            positionrefer_i->setText(QString::fromStdString("insertion position refer fix space"));
            if(Muscle!=nullptr){
                
                std::vector<double> rho_o_position_global=allnodes[0]->get_gamma_node(0);
                rhooaxisxEdit->setText(QString::fromStdString(doubletostring(rho_o_position_global[0])));
                rhooaxisyEdit->setText(QString::fromStdString(doubletostring(rho_o_position_global[1])));
                rhooaxiszEdit->setText(QString::fromStdString(doubletostring(rho_o_position_global[2])));
                
                std::vector<double> rho_i_position_global=allnodes.back()->get_gamma_node(0);
                rhoiaxisxEdit->setText(QString::fromStdString(doubletostring(rho_i_position_global[0])));
                rhoiaxisyEdit->setText(QString::fromStdString(doubletostring(rho_i_position_global[1])));
                rhoiaxiszEdit->setText(QString::fromStdString(doubletostring(rho_i_position_global[2])));
            }
        }

}

void musclepage::updatevalue(){
    //delete old information
    for (QRadioButton* btn : radioButtonso) {
        if (btn) {delete btn;}
    }
    radioButtonso.clear();
    for (QRadioButton* btn : radioButtonsi) {
        if (btn) {delete btn;}
    }
    radioButtonsi.clear();
    if (buttonGroupo) {
        delete buttonGroupo;
        buttonGroupo = nullptr;
    }
    if (buttonGroupi) {
        delete buttonGroupi;
        buttonGroupi = nullptr;
    }
    for (QCheckBox* btn : checkBoxes_body) {
        if (btn) {delete btn;}
    }
    checkBoxes_body.clear();

    std::string rhoobodyname="";
    std::string rhoibodyname="";
    int musclenum=setmodelwin->getRunmodel()->getModel()->getparm()->getn_muscles();
    if(musclenum){
        if(Muscle!=nullptr){
            rhoobodyname = Muscle->getrhoo_bodyname();
            rhoibodyname = Muscle->getrhoi_bodyname();
        }
    }

    buttonGroupo = new QButtonGroup(this);
    selectedBodies.clear();
    QRadioButton* radioButtono = new QRadioButton(QString::fromStdString("null"), bodyListContent);
    radioButtono->setVisible(false);
    radioButtonso.push_back(radioButtono);
    buttonGroupo->addButton(radioButtonso[0], -1);

    QRadioButton* radioButtonfixo = new QRadioButton(QString::fromStdString("fix_space"), bodyListContent);
    radioButtonso.push_back(radioButtonfixo);
    radioButtonso[1]->setStyleSheet("QRadioButton { color: black; background-color: #CCCCCC;}");
    radioButtonso[1]->setGeometry(10, 30, 200, 30);
    radioButtonso[1]->show();
    buttonGroupo->addButton(radioButtonso[1], 0);

    for(int i=0;i<setmodelwin->getRunmodel()->getModel()->getparm()->getn_bodies();i++){
        QRadioButton* radioButton = new QRadioButton(QString::fromStdString(setmodelwin->getRunmodel()->getModel()->getparm()->getbodyindex(i)->getname()), bodyListContent);
        radioButtonso.push_back(radioButton);
        radioButtonso[i+2]->setStyleSheet("QRadioButton { color: black; background-color: #CCCCCC;}");
        radioButtonso[i+2]->setGeometry(10, 30+(i+1)*40, 200, 30);
        radioButtonso[i+2]->show();
        buttonGroupo->addButton(radioButtonso[i+2], i+1);
    }
    connect(buttonGroupo, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked), this, &musclepage::handleButtonClickedo);
    int findbodyo=setmodelwin->getRunmodel()->getModel()->getparm()->findbodyindex(rhoobodyname);
    radioButtonso[findbodyo+2]->setChecked(true);
    selectedValueo=findbodyo+1;

    buttonGroupi = new QButtonGroup(this);
    QRadioButton* radioButtoni = new QRadioButton(QString::fromStdString("null"), bodyListContent);
    radioButtoni->setVisible(false);
    radioButtonsi.push_back(radioButtoni);
    buttonGroupi->addButton(radioButtonsi[0], -1);

    QRadioButton* radioButtonfixi = new QRadioButton(QString::fromStdString("fix_space"), bodyListContent);
    radioButtonsi.push_back(radioButtonfixi);
    radioButtonsi[1]->setStyleSheet("QRadioButton { color: black; background-color: #CCCCCC;}");
    radioButtonsi[1]->setGeometry(450, 30, 200, 30);
    radioButtonsi[1]->show();
    buttonGroupi->addButton(radioButtonsi[1], 0);


    for(int i=0;i<setmodelwin->getRunmodel()->getModel()->getparm()->getn_bodies();i++){
        QRadioButton* radioButton = new QRadioButton(QString::fromStdString(setmodelwin->getRunmodel()->getModel()->getparm()->getbodyindex(i)->getname()), bodyListContent);
        radioButtonsi.push_back(radioButton);
        radioButtonsi[i+2]->setStyleSheet("QRadioButton { color: black; background-color: #CCCCCC;}");
        radioButtonsi[i+2]->setGeometry(450, 30+(i+1)*40, 200, 30);
        radioButtonsi[i+2]->show();
        buttonGroupi->addButton(radioButtonsi[i+2], i+1);
    }
    connect(buttonGroupi, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked), this, &musclepage::handleButtonClickedi);
    int findbodyi=setmodelwin->getRunmodel()->getModel()->getparm()->findbodyindex(rhoibodyname);
    radioButtonsi[findbodyi+2]->setChecked(true);
    selectedValuei=findbodyi+1;
    std::vector<std::string> consider_bodies = {};
    if(Muscle!=nullptr){
        consider_bodies = Muscle->get_consider_body_list();
    }
    int nBodies = setmodelwin->getRunmodel()->getModel()->getparm()->getn_bodies();
    for (int i = 0; i < nBodies; ++i) {
        std::string bodyName = setmodelwin->getRunmodel()->getModel()->getparm()->getbodyindex(i)->getname();
        QCheckBox* checkBox = new QCheckBox(QString::fromStdString(bodyName), bodyListContent);
        checkBox->setStyleSheet("QCheckBox { color: black; background-color: #CCCCCC; }");
        checkBox->setGeometry(910, 30+i * 40, 340, 30);
        checkBox->show();
        checkBoxes_body.push_back(checkBox);
        connect(checkBox, &QCheckBox::stateChanged,this, &musclepage::handleCheckBoxChanged_body);
    }
    if(consider_bodies.size()){
        for(int i=0; i<consider_bodies.size(); i++){
            int body_index = setmodelwin->getRunmodel()->getModel()->getparm()->findbodyindex(consider_bodies[i]);
            checkBoxes_body[body_index]->setChecked(1);
        }
    } else {
        for(int i=0; i<checkBoxes_body.size(); i++){
            checkBoxes_body[i]->setChecked(1);
        }
    }


    if (rectanglemain) {
        rectanglemain->update();
        rectanglemain->show();
    }
    bodyListContent->resize(1260, std::max(350, 150 + nBodies * 40));
    this->update();
}

void musclepage::handleButtonClickedtype(QAbstractButton* button){
    selectedValuelocal = buttonGroupgloballocal->id(button);
    setlocalglobal();
}

void musclepage::handleCheckBoxChanged_body(int state){
    selectedBodies.clear();
    for (int i = 0; i < checkBoxes_body.size(); ++i) {
        if (checkBoxes_body[i]->isChecked()) {
            selectedBodies.push_back(i);
        }
    }
}
