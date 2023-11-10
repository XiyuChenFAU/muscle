/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
03.11.2023
Xiyu Chen

*/

#include "setmodelwindow.h"
#include "mainwindow.h"
#include <QStackedWidget>
#include <QVBoxLayout>


setmodelwindow::setmodelwindow(runmodel* Runmodel,QWidget *parent):
    QWidget(parent),
    Runmodel(Runmodel)
{
    setGeometry(100, 100, 1500, 900);
    setStyleSheet("background-color: white;");
    stackedWidget = new QStackedWidget(this);
    
    rectangle = new QFrame(this);
    rectangle->setGeometry(0, 0, 100, this->height()+1000);
    rectangle->setStyleSheet("background-color: gray;");
    
    //set body page
    Bodypage = new bodypage(this);
    Bodypage->setGeometry(100, 0, this->width() - 100, this->height());
    stackedWidget->addWidget(Bodypage);
    QPushButton *bodybutton = new QPushButton("Armbody", this);
    addleftbutton(bodybutton);


    //set muscle page
    Musclepage = new musclepage(this);
    Musclepage->setGeometry(100, 0, this->width() - 100, this->height());
    stackedWidget->addWidget(Musclepage);
    QPushButton *musclebutton = new QPushButton("Muscle", this);
    addleftbutton(musclebutton);

    //set solve setting
    Jointpage = new jointpage(this);
    Jointpage->setGeometry(100, 0, this->width() - 100, this->height());
    stackedWidget->addWidget(Jointpage);
    QPushButton *jointbutton = new QPushButton("Joint setting", this);
    addleftbutton(jointbutton);

    //set solve setting
    Solvesettingpage = new solvesettingpage(this);
    Solvesettingpage->setGeometry(100, 0, this->width() - 100, this->height());
    stackedWidget->addWidget(Solvesettingpage);
    QPushButton *solvesettingbutton = new QPushButton("Solve setting", this);
    addleftbutton(solvesettingbutton);

    //set run page
    Runprogrampage = new runprogrampage(this);
    Runprogrampage->setGeometry(100, 0, this->width() - 100, this->height());
    stackedWidget->addWidget(Runprogrampage);
    QPushButton *runprogrambutton = new QPushButton("Run", this);
    addleftbutton(runprogrambutton);

    //set post process
    Postprocessingpage = new postprocessingpage(this);
    Postprocessingpage->setGeometry(100, 0, this->width() - 100, this->height());
    stackedWidget->addWidget(Postprocessingpage);
    QPushButton *postprocessingbutton = new QPushButton("Post process", this);
    addleftbutton(postprocessingbutton);

    for (int i = 0; i < buttons.size(); ++i) {
        connect(buttons[i], &QPushButton::clicked, this, [this, i]() {
            showpage(i);
        });
    }

    QPushButton *buttonbackmodelpage = new QPushButton("back model", this);
    buttons.push_back(buttonbackmodelpage);
    buttons[buttons.size()-1]->setStyleSheet("background-color: #CCCCCC;");
    buttons[buttons.size()-1]->setGeometry(0, 50*(buttons.size()-1), 100, 50);
    connect(buttons[buttons.size()-1], &QPushButton::clicked, this, &setmodelwindow::showbackmodelpage);

    stackedWidget->setGeometry(110, 0, this->width(), this->height()+100);
    showpage(0);
}

setmodelwindow::~setmodelwindow()
{
    for(int i=0;i<buttons.size();i++){
        delete buttons[i];
    }
    delete rectangle;
    delete Bodypage ;
    delete Musclepage ;
    delete Solvesettingpage ;
    delete Jointpage;
    delete Runprogrampage ;
    delete stackedWidget;
    delete Runmodel;
}

void setmodelwindow::addleftbutton(QPushButton * button){
    buttons.push_back(button);
    buttons[buttons.size()-1]->setStyleSheet("background-color: #CCCCCC;");
    buttons[buttons.size()-1]->setGeometry(0, 50*(buttons.size()-1), 100, 50);
}

void setmodelwindow::showpage(int index){
    stackedWidget->setCurrentIndex(index);
    changebuttonstyle(index);

    if (index == 4) {
        QWidget *currentPage = stackedWidget->currentWidget();
        runprogrampage *page = qobject_cast<runprogrampage *>(currentPage);
        if (page) {
            page->updatevalue();
        }
    }
}

void setmodelwindow::showbackmodelpage(){
    this->close();
    MainWindow *mainWin = new MainWindow();
    mainWin->show();
}

void setmodelwindow::changebuttonstyle(int index){
    for(int i=0;i<buttons.size();i++){
        if(i==index){
            buttons[i]->setStyleSheet("background-color: white; color: #333333; font-weight: bold; border: 2px solid white;");
        }
        else{
            buttons[i]->setStyleSheet("background-color: #CCCCCC;");
        }
    }
}

runmodel* setmodelwindow::getRunmodel(){
    return Runmodel;
}
