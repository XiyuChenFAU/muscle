/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
03.11.2023
Xiyu Chen

*/

#include "translationjointpage.h"
#include "../jointpage.h"

translationjointpage::translationjointpage(jointpage *Jointpage, QWidget *parent):
    QWidget(parent),
    Jointpage(Jointpage)
{
    rectangle = new QFrame(this);
    rectangle->setGeometry(0, 0, 300, 480);
    rectangle->setStyleSheet("background-color: #CCCCCC;");

    int allfontsize=15;

    QLineEdit *initialanglexedit=settextandlabel("initial translation x","0", 0, 0, 140, 30, allfontsize);
    qedits.push_back(initialanglexedit);
    QLineEdit *initialangleyedit=settextandlabel("initial translation y","0", 0, 80, 140, 30, allfontsize);
    qedits.push_back(initialangleyedit);
    QLineEdit *initialanglezedit=settextandlabel("initial translation z","0", 0, 160, 140, 30, allfontsize);
    qedits.push_back(initialanglezedit);
    QLineEdit *axisxedit=settextandlabel("translation x","0", 160, 0, 140, 30, allfontsize);
    qedits.push_back(axisxedit);
    QLineEdit *axisyedit=settextandlabel("translation y","0", 160, 80, 140, 30, allfontsize);
    qedits.push_back(axisyedit);
    QLineEdit *axiszedit=settextandlabel("translation z","0", 160, 160, 140, 30, allfontsize);
    qedits.push_back(axiszedit);

    setforjointi(0);
}

translationjointpage::~translationjointpage(){
    for(int i=0;i<qlabels.size();i++){
        delete qlabels[i];
    }
    for(int i=0;i<qedits.size();i++){
        delete qedits[i];
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
    }
    else{
        joint* Joint=Jointpage->getrunmodel()->getModel()->getparm()->getjointindex(index);
        std::vector<double> initialtranslation=Joint->getinitialtranslation();
        std::vector<double> translation=Joint->gettranslation();
        qedits[0]->setText(QString::fromStdString(doubletostring(initialtranslation[0])));
        qedits[1]->setText(QString::fromStdString(doubletostring(initialtranslation[1])));
        qedits[2]->setText(QString::fromStdString(doubletostring(initialtranslation[2])));
        qedits[3]->setText(QString::fromStdString(doubletostring(translation[0])));
        qedits[4]->setText(QString::fromStdString(doubletostring(translation[1])));
        qedits[5]->setText(QString::fromStdString(doubletostring(translation[2])));
    }
}

