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
    std::string rotatebodyname=setmodelwin->getRunmodel()->getModel()->getSolveeq()->getrotatebody();
    std::vector<double> rotationaxis=setmodelwin->getRunmodel()->getModel()->getSolveeq()->getnaxis();
    std::string rotationaxis_x=doubletostring(rotationaxis[0]);
    std::string rotationaxis_y=doubletostring(rotationaxis[1]);
    std::string rotationaxis_z=doubletostring(rotationaxis[2]);
    std::string rotationanglestring=doubletostring(setmodelwin->getRunmodel()->getModel()->getSolveeq()->getrotationangle());
    std::string rotationanglestepstring=doubletostring(setmodelwin->getRunmodel()->getModel()->getSolveeq()->getrotationanglestep());

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
    //solve problem
    setlabel("Solve problem", 10, 110,20);
    rotatebodyEdit=settextandlabel("rotate body name",rotatebodyname, 10, 150, 450, 30, allfontsize);
    setlabel("rotation axis", 10, 240, allfontsize);
    rotationaxis_xEdit = settextandlabel("axis x",rotationaxis_x, 10, 270, 140, 30, allfontsize);
    rotationaxis_yEdit = settextandlabel("axis y",rotationaxis_y, 165, 270, 140, 30, allfontsize);
    rotationaxis_zEdit = settextandlabel("axis z",rotationaxis_z, 320, 270, 140, 30, allfontsize);
    rotationangleEdit = settextandlabel("rotation angle",rotationanglestring, 10, 360, 450, 30, allfontsize);
    rotationanglestepEdit = settextandlabel("rotation angle per step",rotationanglestepstring, 10, 450, 450, 30, allfontsize);

    //post processing setting
    setlabel("Post processing setting", 10, 545,20);
    tolpostprocessingEdit=settextandlabel("contact threshold",tolpostprocessing, 10, 595, 450, 30, allfontsize);

    //Casadi setting
    setlabel("Casadi setting", 660, 110,20);
    tolEdit=settextandlabel("tolerance",tol_string, 660, 150, 450, 30, allfontsize);
    max_iterEdit=settextandlabel("max iteration",max_iter_string, 660, 230, 450, 30, allfontsize);
    linear_solverEdit=settextandlabel("linear solver",linear_solver_string, 660, 310, 450, 30, allfontsize);
    print_levelEdit=settextandlabel("print level",print_level_string, 660, 390, 450, 30, allfontsize);
    hessian_approximationEdit=settextandlabel("hessian approximation",hessian_approximation_string, 660, 470, 450, 30, allfontsize);

    //save button
    QPushButton *savebutton = new QPushButton("Save", this);
    savebutton->setStyleSheet("QPushButton { color: black; background-color: grey;}");
    savebutton->setGeometry(1010, 600, 100, 50);
    connect(savebutton, &QPushButton::clicked, this, &solvesettingpage::savesetting);

}

solvesettingpage::~solvesettingpage(){
    for(int i=0;i<qlabels.size();i++){
        delete qlabels[i];
    }
    
    delete rotatebodyEdit;
    delete rotationaxis_xEdit;
    delete rotationaxis_yEdit;
    delete rotationaxis_zEdit;
    delete rotationangleEdit;
    delete rotationanglestepEdit;

    delete tolpostprocessingEdit;

    delete tolEdit;
    delete max_iterEdit;
    delete linear_solverEdit;
    delete print_levelEdit;
    delete hessian_approximationEdit;
    delete rectangle;
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
    body* findbody=setmodelwin->getRunmodel()->getModel()->getparm()->findbody(rotatebodyEdit->text().toStdString());
    if(findbody==nullptr){
        errorbox("please check rotate body name, it does not exist");
    }
    else{
        std::vector<double> naxisvalue={rotationaxis_xEdit->text().toDouble(),rotationaxis_yEdit->text().toDouble(),rotationaxis_zEdit->text().toDouble()};
        setmodelwin->getRunmodel()->getModel()->getSolveeq()->setproblemtosolve(rotatebodyEdit->text().toStdString(), naxisvalue, rotationangleEdit->text().toDouble(), rotationanglestepEdit->text().toDouble());
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
