/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
03.11.2023
Xiyu Chen

*/

#include "sphericaljointpage.h"
#include "../jointpage.h"

sphericaljointpage::sphericaljointpage(jointpage *Jointpage, QWidget *parent):
    QWidget(parent),
    Jointpage(Jointpage)
{
    rectangle = new QFrame(this);
    rectangle->setGeometry(0, 0, 300, 480);
    rectangle->setStyleSheet("background-color: #CCCCCC;");

    int allfontsize=15;

    QLineEdit *initialanglexedit=settextandlabel("initial rotation angle x","0", 0, 0, 140, 30, allfontsize);
    qedits.push_back(initialanglexedit);
    QLineEdit *initialangleyedit=settextandlabel("initial rotation angle y","0", 0, 80, 140, 30, allfontsize);
    qedits.push_back(initialangleyedit);
    QLineEdit *initialanglezedit=settextandlabel("initial rotation angle z","0", 0, 160, 140, 30, allfontsize);
    qedits.push_back(initialanglezedit);
    QLineEdit *axisxedit=settextandlabel("rotation angle x","0", 160, 0, 140, 30, allfontsize);
    qedits.push_back(axisxedit);
    QLineEdit *axisyedit=settextandlabel("rotation angle y","0", 160, 80, 140, 30, allfontsize);
    qedits.push_back(axisyedit);
    QLineEdit *axiszedit=settextandlabel("rotation angle z","0", 160, 160, 140, 30, allfontsize);
    qedits.push_back(axiszedit);

    setforjointi(0);
}

sphericaljointpage::~sphericaljointpage(){
    for(int i=0;i<qlabels.size();i++){
        delete qlabels[i];
    }
    for(int i=0;i<qedits.size();i++){
        delete qedits[i];
    }
    delete rectangle;

}

QLineEdit* sphericaljointpage::settext(const std::string& textdefault, int x, int y, int textwidth, int textheight ,int fontsize) {
    QLineEdit* body_nameEdit = new QLineEdit(this);
    QFont body_nameEditfont = body_nameEdit->font();
    body_nameEditfont.setPointSize(fontsize); 
    body_nameEdit->setFont(body_nameEditfont);
    body_nameEdit->setGeometry(x, y, textwidth, textheight);
    body_nameEdit->setText(QString::fromStdString(textdefault));
    body_nameEdit->setStyleSheet("QLineEdit { color: black; }");
    return body_nameEdit;
}
void sphericaljointpage::setlabel(const std::string& labelname, int x, int y ,int fontsize) {
    QLabel *body_namelabel = new QLabel(this);
    QFont body_namefont = body_namelabel->font();
    body_namefont.setPointSize(fontsize); 
    body_namelabel->setFont(body_namefont);
    body_namelabel->setText(QString::fromStdString(labelname)); 
    body_namelabel->move(x, y);
    body_namelabel->setStyleSheet("QLabel { color : black; background-color : #CCCCCC; }");
    qlabels.push_back(body_namelabel);
}

QLineEdit* sphericaljointpage::settextandlabel(const std::string& labelname, const std::string& textdefault, int x, int y, int textwidth, int textheight, int fontsize) {
    setlabel(labelname, x, y, fontsize);
    QLineEdit* body_nameEdit = settext(textdefault, x, y+25, textwidth, textheight, fontsize);
    return body_nameEdit;
}

std::string sphericaljointpage::doubletostring(double num) {
    std::ostringstream stream;
    stream << num;
    std::string numStr = stream.str();
    return numStr;
}

std::vector<QLineEdit *> sphericaljointpage::getqedits(){
    return qedits;
}
    
void sphericaljointpage::setforjointi(int index){
    int jointnum=Jointpage->getrunmodel()->getModel()->getparm()->getn_joints();
    if(index>jointnum-1){
        for(int i=0;i<qedits.size();i++){
            qedits[i]->setText(QString::fromStdString("0"));
        }
    }
    else{
        joint* Joint=Jointpage->getrunmodel()->getModel()->getparm()->getjointindex(index);
        std::vector<std::vector<double>> rotationangle=Joint->get_movement();
        for(int i=0; i<3;i++){
            if(rotationangle.size()>i){
                if(!rotationangle[i].empty()){
                    qedits[i]->setText(QString::fromStdString(doubletostring(rotationangle[i][0])));
                    qedits[i+3]->setText(QString::fromStdString(doubletostring(rotationangle[i].back())));
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
        }
    }
}

