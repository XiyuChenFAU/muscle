/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
03.11.2023
Xiyu Chen

*/

#include "mainwindow.h"
#include <QFileDialog> 

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent)
{
    Runmodel=new runmodel();
    setGeometry(100, 100, 800, 480);
    setStyleSheet("background-color: #CCCCCC;");

    jsonFilePathLineEdit = settextandlabel("Select exist model", "", 200, 40, 500, 60);
    selectJsonButton = new QPushButton("Select model", this);
    selectJsonButton->setStyleSheet("QPushButton { color: black; background-color: grey;}");
    selectJsonButton->setGeometry(610, 65, 100, 60);
    connect(selectJsonButton, SIGNAL(clicked()), this, SLOT(selectJsonFile()));

    setlabel("OR", 400, 140);
    jsonFolderPathLineEdit = settextandlabel("Select folder run multiple model", "", 200, 170, 500, 60);
    selectFolderButton = new QPushButton("Select folder", this);
    selectFolderButton->setStyleSheet("QPushButton { color: black; background-color: grey;}");
    selectFolderButton->setGeometry(610, 195, 100, 60);
    connect(selectFolderButton, SIGNAL(clicked()), this, SLOT(selectFolder()));

    setlabel("OR", 400, 270);
    newmodelEdit = settextandlabel("Set new model", "", 200, 300, 400, 40);

    runButton = new QPushButton("Comfirm", this);
    runButton->setStyleSheet("QPushButton { color: black; background-color: grey;}");
    runButton->setGeometry(350, 400, 100, 50);
    connect(runButton, SIGNAL(clicked()), this, SLOT(runModel()));
}
/*
MainWindow::MainWindow(runmodel* Runmodel,QWidget *parent):
    Runmodel(Runmodel),
    QMainWindow(parent)
{
    setGeometry(100, 100, 800, 480);
    setStyleSheet("background-color: #CCCCCC;");

    jsonFilePathLineEdit = settextandlabel("Select exist model", "", 200, 40, 400, 60);

    selectJsonButton = new QPushButton("Select model", this);
    selectJsonButton->setStyleSheet("QPushButton { color: black; background-color: grey;}");
    selectJsonButton->setGeometry(610, 65, 100, 60);
    connect(selectJsonButton, SIGNAL(clicked()), this, SLOT(selectJsonFile()));

    setlabel("OR", 400, 180);

    newmodelEdit = settextandlabel("Set new model", "", 200, 235, 400, 30);

    runButton = new QPushButton("Comfirm", this);
    runButton->setStyleSheet("QPushButton { color: black; background-color: grey;}");
    runButton->setGeometry(350, 320, 100, 50);
    connect(runButton, SIGNAL(clicked()), this, SLOT(runModel()));
}
*/

MainWindow::~MainWindow()
{
    delete selectJsonButton;
    delete runButton;
    delete selectFolderButton;
    delete jsonFilePathLineEdit;
    delete newmodelEdit;
    delete jsonFolderPathLineEdit;
    for(int i=0;i<qlabels.size();i++){
        delete qlabels[i];
    }
}

void MainWindow::selectJsonFile() {
    QString jsonFilePath = QFileDialog::getOpenFileName(this, "Select JSON File", "", "JSON Files (*.json)");
    jsonFilePathLineEdit->setText(jsonFilePath);
}

void MainWindow::selectFolder() {
    QString folderPath = QFileDialog::getExistingDirectory(this, "Select Folder", "");
    if (!folderPath.isEmpty()) {
        jsonFolderPathLineEdit->setText(folderPath);
    }
}

void MainWindow::runModel() {
    QString jsonFilePath = jsonFilePathLineEdit->toPlainText();
    std::string jsonFilePathStdString = jsonFilePath.toStdString();
    QString jsonFolderPath = jsonFolderPathLineEdit->toPlainText();
    std::string jsonFolderPathstring = jsonFolderPath.toStdString();
    QString newmodelname = newmodelEdit->toPlainText();
    std::string newmodelnamestring = newmodelname.toStdString();
    if(jsonFilePathStdString!=""){
        Runmodel->setrunmodel(jsonFilePathStdString,1);
        if(setmodelWindow!=nullptr){delete setmodelWindow;}
        setmodelWindow=new setmodelwindow(Runmodel);
        setmodelWindow->show();
        this->close();
    }

    if(newmodelnamestring!=""){
        Runmodel->setrunmodel(newmodelnamestring);
        if(setmodelWindow!=nullptr){delete setmodelWindow;}
        setmodelWindow=new setmodelwindow(Runmodel);
        setmodelWindow->show();
        this->close();
    }

    if(jsonFolderPathstring!=""){
        for (const auto& entry : std::filesystem::directory_iterator(jsonFolderPathstring)) {
            if (entry.is_regular_file() && entry.path().extension() == ".json") {
                std::string jsonFile = entry.path().string();
                runmodel model(jsonFile,1);
                double time= model.runprogramm();
            }
        }
        QMessageBox::information(this, "task complete", "programm finish");
    }
}

QTextEdit* MainWindow::settext(const std::string& textdefault, int x, int y, int textwidth, int textheight) {
    QTextEdit* body_nameEdit = new QTextEdit(this);
    body_nameEdit->setGeometry(x, y, textwidth, textheight);
    body_nameEdit->setPlainText(QString::fromStdString(textdefault));
    body_nameEdit->setStyleSheet("QTextEdit { color: black;  background-color: white;}");
    return body_nameEdit;
}
void MainWindow::setlabel(const std::string& labelname, int x, int y) {
    QLabel *body_namelabel = new QLabel(this);
    body_namelabel->setText(QString::fromStdString(labelname)); 
    body_namelabel->setGeometry(x, y, 200, 20);
    body_namelabel->setStyleSheet("QLabel { color : black; background-color : #CCCCCC; }");
    qlabels.push_back(body_namelabel);
}

QTextEdit* MainWindow::settextandlabel(const std::string& labelname, const std::string& textdefault, int x, int y, int textwidth, int textheight) {
    setlabel(labelname, x, y);
    QTextEdit* body_nameEdit = settext(textdefault, x, y+25, textwidth, textheight);
    return body_nameEdit;
}
