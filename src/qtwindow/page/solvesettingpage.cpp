/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
03.11.2023
Xiyu Chen

*/

#include "solvesettingpage.h"
#include "../setmodelwindow.h"

solvesettingpage::solvesettingpage(setmodelwindow *setmodelwin, QWidget *parent):
    QWidget(parent),
    setmodelwin(setmodelwin)
{
    std::string modelname_string=setmodelwin->getRunmodel()->getModel()->getmodelname();
    std::string savepath_string=setmodelwin->getRunmodel()->getModel()->getfolderpath();
    std::string length_cons_string=doubletostring(setmodelwin->getRunmodel()->getModel()->getSolveeq()->getObjective()->getlengthconstant());

    std::string tolpostprocessing=doubletostring(setmodelwin->getRunmodel()->getModel()->getPostprocessing()->gettol());

    std::string tol_string = doubletostring(setmodelwin->getRunmodel()->getModel()->getSolveeq()->getipopt()->gettol());
    std::string max_iter_string = std::to_string(setmodelwin->getRunmodel()->getModel()->getSolveeq()->getipopt()->getmax_iter());
    std::string linear_solver_string = setmodelwin->getRunmodel()->getModel()->getSolveeq()->getipopt()->getlinear_solver();
    std::string print_level_string = std::to_string(setmodelwin->getRunmodel()->getModel()->getSolveeq()->getipopt()->getprint_level());
    std::string hessian_approximation_string = setmodelwin->getRunmodel()->getModel()->getSolveeq()->getipopt()->gethessian_approximation();

    rectangle = new QFrame(this);
    rectangle->setGeometry(0, 100, setmodelwin->width(), setmodelwin->width()+1000);
    rectangle->setStyleSheet("background-color: #CCCCCC;");

    int allfontsize=15;
    //model setting
    setlabel("Model setting", 10, 110,20);
    modelnameEdit=settextandlabel("model name",modelname_string, 10, 150, 450, 30, allfontsize);
    savepathEdit = settextandlabel("result save path",savepath_string, 10, 230, 450, 30, allfontsize);
    selectFolderButton = new QPushButton("select folder", this);
    selectFolderButton->setStyleSheet("QPushButton { color: black; background-color: grey;}");
    selectFolderButton->setGeometry(360, 290, 100, 50);
    connect(selectFolderButton, &QPushButton::clicked, this, &solvesettingpage::openFolderDialog);

    check_phi_eta_plus_CheckBox = new QCheckBox("sum phi*eta constraint for hard case convergence", this);
    check_phi_eta_plus_CheckBox->setGeometry(10, 360, 340, 30);
    check_phi_eta_plus_CheckBox->setStyleSheet("QCheckBox { color: black; background-color: #CCCCCC;}");

    check_phi_eta_plus_Value = setmodelwin->getRunmodel()->getModel()->getSolveeq()->getConstraint()->get_phi_eta_plus();
    check_phi_eta_plus_CheckBox->setChecked(check_phi_eta_plus_Value);
    connect(check_phi_eta_plus_CheckBox, &QCheckBox::stateChanged, this, &solvesettingpage::handleCheckBoxChanged_phi_eta_plus);

    check_all_muscle_together_CheckBox = new QCheckBox("calculate all muscle together", this);
    check_all_muscle_together_CheckBox->setGeometry(10, 400, 340, 30);
    check_all_muscle_together_CheckBox->setStyleSheet("QCheckBox { color: black; background-color: #CCCCCC;}");

    check_all_muscle_together_Value = setmodelwin->getRunmodel()->getModel()->getSolveeq()->get_all_muscle_together();
    check_all_muscle_together_CheckBox->setChecked(check_all_muscle_together_Value);
    connect(check_all_muscle_together_CheckBox, &QCheckBox::stateChanged, this, &solvesettingpage::handleCheckBoxChanged_all_muscle_together);

    setlabel("solver setting", 10, 440,20);

    buttonGroup1 = new QButtonGroup;

    QRadioButton* radioButton1_1 = new QRadioButton("no objective", this);
    radioButtons.push_back(radioButton1_1);
    radioButtons[0]->setStyleSheet("QRadioButton { color: black; background-color: #CCCCCC;}");
    radioButtons[0]->setGeometry(10, 480, 450, 30);
    QRadioButton* radioButton1_2 = new QRadioButton("minimize node distance", this);
    radioButtons.push_back(radioButton1_2);
    radioButtons[1]->setStyleSheet("QRadioButton { color: black; background-color: #CCCCCC;}");
    radioButtons[1]->setGeometry(10, 520, 450, 30);
    QRadioButton* radioButton1_3 = new QRadioButton("minimize node distance with weight", this);
    radioButtons.push_back(radioButton1_3);
    radioButtons[2]->setStyleSheet("QRadioButton { color: black; background-color: #CCCCCC;}");
    radioButtons[2]->setGeometry(10, 560, 450, 30);
    QRadioButton* radioButton1_4 = new QRadioButton("minimize node distance with length change", this);
    radioButtons.push_back(radioButton1_4);
    radioButtons[3]->setStyleSheet("QRadioButton { color: black; background-color: #CCCCCC;}");
    radioButtons[3]->setGeometry(10, 600, 450, 30);

    selectedValue = setmodelwin->getRunmodel()->getModel()->getSolveeq()->getObjective()->getcasenum();
    radioButtons[selectedValue]->setChecked(true);

    buttonGroup1->addButton(radioButtons[0], 0);
    buttonGroup1->addButton(radioButtons[1], 1);
    buttonGroup1->addButton(radioButtons[2], 2); 
    buttonGroup1->addButton(radioButtons[3], 3); 
    connect(buttonGroup1, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked), this, &solvesettingpage::handleButtonClicked);

    lengthconstEdit=settextandlabel("lenght spring constant",length_cons_string, 340, 575, 110, 30, allfontsize-4);
    lenghtspringlabel=qlabels.back();
    if(selectedValue==3){
        lengthconstEdit->setVisible(true);
        lenghtspringlabel->setVisible(true);
    }
    else{
        lengthconstEdit->setVisible(false);
        lenghtspringlabel->setVisible(false);
    }

    setlabel("local parameterization", 880, 110, 20);
    buttonGroup_initial_mode = new QButtonGroup;

    QRadioButton* radioButton1_mode_1 = new QRadioButton("no local", this);
    radioButtons_mode.push_back(radioButton1_mode_1);
    radioButtons_mode[0]->setStyleSheet("QRadioButton { color: black; background-color: #CCCCCC;}");
    radioButtons_mode[0]->setGeometry(880, 150, 200, 30);
    QRadioButton* radioButton1_mode_2 = new QRadioButton("local", this);
    radioButtons_mode.push_back(radioButton1_mode_2);
    radioButtons_mode[1]->setStyleSheet("QRadioButton { color: black; background-color: #CCCCCC;}");
    radioButtons_mode[1]->setGeometry(880, 190, 200, 30);
    QRadioButton* radioButton1_mode_3 = new QRadioButton("auto local", this);
    radioButtons_mode.push_back(radioButton1_mode_3);
    radioButtons_mode[2]->setStyleSheet("QRadioButton { color: black; background-color: #CCCCCC;}");
    radioButtons_mode[2]->setGeometry(880, 230, 200, 30);
    QRadioButton* radioButton1_mode_4 = new QRadioButton("auto local dynamic", this);
    radioButtons_mode.push_back(radioButton1_mode_4);
    radioButtons_mode[3]->setStyleSheet("QRadioButton { color: black; background-color: #CCCCCC;}");
    radioButtons_mode[3]->setGeometry(880, 270, 200, 30);

    selectedValue_mode = setmodelwin->getRunmodel()->getModel()->getSolveeq()->getInitialguess()->getmode_nr();
    buttonGroup_initial_mode->addButton(radioButtons_mode[0], 0);
    buttonGroup_initial_mode->addButton(radioButtons_mode[1], 1);
    buttonGroup_initial_mode->addButton(radioButtons_mode[2], 2); 
    buttonGroup_initial_mode->addButton(radioButtons_mode[3], 3); 
    connect(buttonGroup_initial_mode, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked), this, &solvesettingpage::handleButtonClicked_mode);
    radioButtons_mode[selectedValue_mode]->setChecked(true);

    updatevalue();

    check_collision_CheckBox = new QCheckBox("check collision", this);
    check_collision_CheckBox->setGeometry(1100, 110, 340, 30);
    check_collision_CheckBox->setStyleSheet("QCheckBox { color: black; background-color: #CCCCCC;}");

    check_collision_Value = setmodelwin->getRunmodel()->getModel()->getSolveeq()->getInitialguess()->getcollision_check();
    check_collision_CheckBox->setChecked(check_collision_Value);
    connect(check_collision_CheckBox, &QCheckBox::stateChanged, this, &solvesettingpage::handleCheckBoxChanged_collision);

    if(selectedValue_mode==0){
        check_collision_CheckBox->setVisible(false);
    }
    else{
        check_collision_CheckBox->setVisible(true);
    }


    //Casadi setting
    setlabel("Casadi setting", 570, 110, 20);
    tolEdit=settextandlabel("tolerance",tol_string, 570, 150, 200, 30, allfontsize);
    max_iterEdit=settextandlabel("max iteration",max_iter_string, 570, 230, 200, 30, allfontsize);
    linear_solverEdit=settextandlabel("linear solver",linear_solver_string, 570, 310, 200, 30, allfontsize);
    print_levelEdit=settextandlabel("print level",print_level_string, 570, 390, 200, 30, allfontsize);
    hessian_approximationEdit=settextandlabel("hessian approximation",hessian_approximation_string, 570, 470, 200, 30, allfontsize);

    //post processing setting
    setlabel("Post processing setting", 570, 550,20);
    tolpostprocessingEdit=settextandlabel("contact threshold",tolpostprocessing, 570, 600, 200, 30, allfontsize);

    //save button
    savebutton = new QPushButton("Save", this);
    savebutton->setStyleSheet("QPushButton { color: black; background-color: grey;}");
    savebutton->setGeometry(1010, 730, 100, 50);
    connect(savebutton, &QPushButton::clicked, this, &solvesettingpage::savesetting);

}

solvesettingpage::~solvesettingpage(){
    for(int i=0;i<qlabels.size();i++){
        delete qlabels[i];
    }
    
    delete modelnameEdit;
    delete savepathEdit;
    delete lengthconstEdit;

    delete tolpostprocessingEdit;

    delete tolEdit;
    delete max_iterEdit;
    delete linear_solverEdit;
    delete print_levelEdit;
    delete hessian_approximationEdit;
    delete rectangle;
    delete selectFolderButton;
    delete savebutton;
    for(int i=0;i<radioButtons.size();i++){
        delete radioButtons[i];
    }
    delete buttonGroup1;

    for(int i=0;i<radioButtons_mode.size();i++){
        delete radioButtons_mode[i];
    }
    delete buttonGroup_initial_mode;

    for(int i=0;i<radioButtons_body.size();i++){
        delete radioButtons_body[i];
    }
    delete buttonGroup_initial_body;
    delete check_collision_CheckBox;
    delete check_phi_eta_plus_CheckBox;
    delete check_all_muscle_together_CheckBox;
}

QLineEdit* solvesettingpage::settext(const std::string& textdefault, int x, int y, int textwidth, int textheight ,int fontsize) {
    QLineEdit* body_nameEdit = new QLineEdit(this);
    QFont body_nameEditfont = body_nameEdit->font();
    body_nameEditfont.setPointSize(fontsize); 
    body_nameEdit->setFont(body_nameEditfont);
    body_nameEdit->setGeometry(x, y, textwidth, textheight);
    body_nameEdit->setText(QString::fromStdString(textdefault));
    body_nameEdit->setStyleSheet("QLineEdit { color: black; }");
    return body_nameEdit;
}
void solvesettingpage::setlabel(const std::string& labelname, int x, int y ,int fontsize) {
    QLabel *body_namelabel = new QLabel(this);
    QFont body_namefont = body_namelabel->font();
    body_namefont.setPointSize(fontsize); 
    body_namelabel->setFont(body_namefont);
    body_namelabel->setText(QString::fromStdString(labelname)); 
    body_namelabel->move(x, y);
    body_namelabel->setStyleSheet("QLabel { color : black; background-color : #CCCCCC; }");
    qlabels.push_back(body_namelabel);
}

QLineEdit* solvesettingpage::settextandlabel(const std::string& labelname, const std::string& textdefault, int x, int y, int textwidth, int textheight, int fontsize) {
    setlabel(labelname, x, y, fontsize);
    QLineEdit* body_nameEdit = settext(textdefault, x, y+25, textwidth, textheight, fontsize);
    return body_nameEdit;
}

std::string solvesettingpage::doubletostring(double num) {
    std::ostringstream stream;
    stream << num;
    std::string numStr = stream.str();
    return numStr;
}

double solvesettingpage::stringtodouble(std::string numStr) {
    double num;
    std::istringstream stream(numStr);
    stream >> num;
    return num;
}

void solvesettingpage::savesetting(){
    if(modelnameEdit->text().toStdString()==""){
        errorbox("model name can not be empty");
    }
    else{
        setmodelwin->getRunmodel()->getModel()->getSolveeq()->getObjective()->setcasenum(selectedValue);
        if(selectedValue==3){
            setmodelwin->getRunmodel()->getModel()->getSolveeq()->getObjective()->setlengthconstant(lengthconstEdit->text().toDouble());
        }
        setmodelwin->getRunmodel()->getModel()->setmodelname(modelnameEdit->text().toStdString());
        setmodelwin->getRunmodel()->getModel()->setfolderpath(savepathEdit->text().toStdString());
        setmodelwin->getRunmodel()->getModel()->getPostprocessing()->settol(tolpostprocessingEdit->text().toDouble());
        setmodelwin->getRunmodel()->getModel()->getSolveeq()->setipoptoption(tolEdit->text().toDouble(),max_iterEdit->text().toInt(),linear_solverEdit->text().toStdString(),print_levelEdit->text().toInt(),hessian_approximationEdit->text().toStdString());
        setmodelwin->getRunmodel()->getModel()->getSolveeq()->getInitialguess()->setmode_nr(selectedValue_mode);
        setmodelwin->getRunmodel()->getModel()->getSolveeq()->getInitialguess()->setcollision_check(check_collision_Value);
        setmodelwin->getRunmodel()->getModel()->getSolveeq()->getConstraint()->set_phi_eta_plus(check_phi_eta_plus_Value);
        setmodelwin->getRunmodel()->getModel()->getSolveeq()->set_all_muscle_together(check_all_muscle_together_Value);
        if(selectedValue_mode==1){
            setmodelwin->getRunmodel()->getModel()->getSolveeq()->getInitialguess()->setselect_bodyname(setmodelwin->getRunmodel()->getModel()->getparm()->getbodyindex(selectedValue_body-1)->getname());
        }
    }
}

void solvesettingpage::errorbox(std::string errormessage){
    QMessageBox errorMessage;
    errorMessage.setWindowTitle("error");
    errorMessage.setText(QString::fromStdString(errormessage));
    errorMessage.setIcon(QMessageBox::Critical);
    errorMessage.addButton("Yes", QMessageBox::AcceptRole);
    errorMessage.exec();
}

void solvesettingpage::updatevalue(){
    //delete old information
    for (QRadioButton* btn : radioButtons_body) {
        if (btn) {delete btn;}
    }
    radioButtons_body.clear();
    if (buttonGroup_initial_body) {
        delete buttonGroup_initial_body;
        buttonGroup_initial_body = nullptr;
    }


    buttonGroup_initial_body = new QButtonGroup();
    QRadioButton* radioButtoni = new QRadioButton(QString::fromStdString("null"), this);
    radioButtoni->setVisible(false);
    radioButtons_body.push_back(radioButtoni);
    buttonGroup_initial_body->addButton(radioButtons_body[0], -1);
    for(int i=0;i<setmodelwin->getRunmodel()->getModel()->getparm()->getn_bodies()+1;i++){
        QRadioButton* radioButton_body = new QRadioButton(QString::fromStdString(setmodelwin->getRunmodel()->getModel()->getparm()->getbodyindex(i-1)->getname()), this);
        radioButtons_body.push_back(radioButton_body);
        radioButtons_body[i+1]->setStyleSheet("QRadioButton { color: black; background-color: #CCCCCC;}");
        radioButtons_body[i+1]->setGeometry(1100, 150+i*40, 340, 30);
        radioButtons_body[i+1]->show();
        buttonGroup_initial_body->addButton(radioButtons_body[i+1], i);
    }
    connect(buttonGroup_initial_body, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked), this, &solvesettingpage::handleButtonClicked_body);
    std::string initialbodyname=setmodelwin->getRunmodel()->getModel()->getSolveeq()->getInitialguess()->getselect_bodyname();
    int findbodyi=setmodelwin->getRunmodel()->getModel()->getparm()->findbodyindex(initialbodyname);
    radioButtons_body[findbodyi+2]->setChecked(true);
    selectedValue_body=findbodyi+1;

    if(selectedValue_mode==1){
        for (int i = 1; i < radioButtons_body.size(); ++i) {
            radioButtons_body[i]->setVisible(true);
        }
    }
    else{
        for (int i =  1; i < radioButtons_body.size(); ++i) {
            radioButtons_body[i]->setVisible(false);
        }
    }
    if (rectangle) {
        rectangle->update();
        rectangle->show();
    }
    this->update();
}

void solvesettingpage::openFolderDialog() {
    QString folderPath = QFileDialog::getExistingDirectory(this, "select folder", QDir::homePath());
    if (!folderPath.isEmpty()) {
        savepathEdit->setText(folderPath);
    } 
}

void solvesettingpage::handleButtonClicked(QAbstractButton* button){
    selectedValue = buttonGroup1->id(button);
    if(selectedValue==3){
        lengthconstEdit->setVisible(true);
        lenghtspringlabel->setVisible(true);
    }
    else{
        lengthconstEdit->setVisible(false);
        lenghtspringlabel->setVisible(false);
    }
}

void solvesettingpage::handleButtonClicked_mode(QAbstractButton* button){
    selectedValue_mode = buttonGroup_initial_mode->id(button);
    if(selectedValue_mode==1){
        for (int i = 1; i < radioButtons_body.size(); ++i) {
            radioButtons_body[i]->setVisible(true);
        }
    }
    else{
        for (int i =  1; i < radioButtons_body.size(); ++i) {
            radioButtons_body[i]->setVisible(false);
        }
    }
    if(selectedValue_mode==0){
        check_collision_CheckBox->setVisible(false);
    }
    else{
        check_collision_CheckBox->setVisible(true);
    }
}

void solvesettingpage::handleButtonClicked_body(QAbstractButton* button){
    selectedValue_body = buttonGroup_initial_body->id(button);
}

void solvesettingpage::handleCheckBoxChanged_collision(int state)
{
    check_collision_Value = (state == Qt::Checked) ? 1 : 0;
}

void solvesettingpage::handleCheckBoxChanged_phi_eta_plus(int state)
{
    check_phi_eta_plus_Value = (state == Qt::Checked) ? 1 : 0;
}

void solvesettingpage::handleCheckBoxChanged_all_muscle_together(int state)
{
    check_all_muscle_together_Value = (state == Qt::Checked) ? 1 : 0;
}