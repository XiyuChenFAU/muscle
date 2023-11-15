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
    std::string savepath_string=setmodelwin->getRunmodel()->getio()->getfolderpath();

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

    setlabel("solver setting", 10, 360,20);

    buttonGroup1 = new QButtonGroup;

    QRadioButton* radioButton1_1 = new QRadioButton("no objective", this);
    radioButtons.push_back(radioButton1_1);
    radioButtons[0]->setStyleSheet("QRadioButton { color: black; background-color: #CCCCCC;}");
    radioButtons[0]->setGeometry(10, 400, 450, 30);
    QRadioButton* radioButton1_2 = new QRadioButton("minimize node distance", this);
    radioButtons.push_back(radioButton1_2);
    radioButtons[1]->setStyleSheet("QRadioButton { color: black; background-color: #CCCCCC;}");
    radioButtons[1]->setGeometry(10, 450, 450, 30);
    QRadioButton* radioButton1_3 = new QRadioButton("minimize node distance with weight", this);
    radioButtons.push_back(radioButton1_3);
    radioButtons[2]->setStyleSheet("QRadioButton { color: black; background-color: #CCCCCC;}");
    radioButtons[2]->setGeometry(10, 500, 450, 30);
    QRadioButton* radioButton1_4 = new QRadioButton("minimize node distance with length change", this);
    radioButtons.push_back(radioButton1_4);
    radioButtons[3]->setStyleSheet("QRadioButton { color: black; background-color: #CCCCCC;}");
    radioButtons[3]->setGeometry(10, 550, 450, 30);

    selectedValue = setmodelwin->getRunmodel()->getModel()->getSolveeq()->getObjective()->getcasenum();
    radioButtons[selectedValue]->setChecked(true);

    buttonGroup1->addButton(radioButtons[0], 0);
    buttonGroup1->addButton(radioButtons[1], 1);
    buttonGroup1->addButton(radioButtons[2], 2); 
    buttonGroup1->addButton(radioButtons[3], 3); 
    connect(buttonGroup1, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked), this, &solvesettingpage::handleButtonClicked);

    //Casadi setting
    setlabel("Casadi setting", 660, 110,20);
    tolEdit=settextandlabel("tolerance",tol_string, 660, 150, 450, 30, allfontsize);
    max_iterEdit=settextandlabel("max iteration",max_iter_string, 660, 230, 450, 30, allfontsize);
    linear_solverEdit=settextandlabel("linear solver",linear_solver_string, 660, 310, 450, 30, allfontsize);
    print_levelEdit=settextandlabel("print level",print_level_string, 660, 390, 450, 30, allfontsize);
    hessian_approximationEdit=settextandlabel("hessian approximation",hessian_approximation_string, 660, 470, 450, 30, allfontsize);

    //post processing setting
    setlabel("Post processing setting", 660, 550,20);
    tolpostprocessingEdit=settextandlabel("contact threshold",tolpostprocessing, 660, 600, 450, 30, allfontsize);

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
        setmodelwin->getRunmodel()->getModel()->setmodelname(modelnameEdit->text().toStdString());
        setmodelwin->getRunmodel()->getio()->setfolderpath(savepathEdit->text().toStdString());
        setmodelwin->getRunmodel()->getModel()->getPostprocessing()->settol(tolpostprocessingEdit->text().toDouble());
        setmodelwin->getRunmodel()->getModel()->getSolveeq()->setipoptoption(tolEdit->text().toDouble(),max_iterEdit->text().toInt(),linear_solverEdit->text().toStdString(),print_levelEdit->text().toInt(),hessian_approximationEdit->text().toStdString());
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

void solvesettingpage::openFolderDialog() {
    QString folderPath = QFileDialog::getExistingDirectory(this, "select folder", QDir::homePath());
    savepathEdit->setText(folderPath);
}

void solvesettingpage::handleButtonClicked(QAbstractButton* button){
    selectedValue = buttonGroup1->id(button);
}