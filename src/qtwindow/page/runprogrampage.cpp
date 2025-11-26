/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
03.11.2023
Xiyu Chen

*/

#include "runprogrampage.h"
#include "../setmodelwindow.h"
#include <QVBoxLayout>
#include <iostream>

runprogrampage::runprogrampage(setmodelwindow *setmodelwin, QWidget *parent):
    QWidget(parent),
    setmodelwin(setmodelwin)
{
    std::string save_interval=std::to_string(setmodelwin->getRunmodel()->getModel()->get_save_interval());
    int colornum=255;
    for (int i = 0; i < 80; i++) {
        QColor* color=new QColor(255-colornum, colornum, 255-colornum);
        colors.push_back(color);
        color=new QColor(colornum, 255-colornum, 255-colornum);
        colors.push_back(color);
        color=new QColor(255-colornum, 255-colornum, colornum);
        colors.push_back(color);
        colornum=colornum-50;
        if(colornum<0){
            colornum=colornum+255;
        }
    }
    runButton = new QPushButton("Run Model", this);
    runButton->setStyleSheet("QPushButton { color: black; background-color: #CCCCCC;}");
    runButton->setGeometry(10, 30, 100, 50);
    
    connect(runButton, &QPushButton::clicked, this, &runprogrampage::runModelFunction);

    //save button
    exportbutton = new QPushButton("Export json", this);
    exportbutton->setStyleSheet("QPushButton { color: black; background-color: #CCCCCC;}");
    exportbutton->setGeometry(120, 30, 100, 50);
    connect(exportbutton, &QPushButton::clicked, this, &runprogrampage::saveModel);

    save_intervalEdit=settextandlabel("save interval",save_interval, 230, 10, 80, 40, 15);

    runtimelabel=setlabel("", 10, 155 ,15);
    runtimelabel->setGeometry(320, 40, 200, 30);

    scaleedit=settext("", 450, 30, 100, 50 ,15);
    scaleeditbutton = new QPushButton("zoom figure", this);
    scaleeditbutton->setStyleSheet("QPushButton { color: black; background-color: #CCCCCC;}");
    scaleeditbutton->setGeometry(570, 30, 100, 50);
    connect(scaleeditbutton, &QPushButton::clicked, this, &runprogrampage::setscale);

    rotateedit=settext("", 450, 30, 100, 50 ,15);
    rotateeditbutton = new QPushButton("rotate default", this);
    rotateeditbutton->setStyleSheet("QPushButton { color: black; background-color: #CCCCCC;}");
    rotateeditbutton->setGeometry(670, 30, 100, 50);
    connect(rotateeditbutton, &QPushButton::clicked, this, &runprogrampage::setRotationDefault);

    labels.push_back(setlabel("initial", 500, 100 ,15));
    labels[labels.size()-1]->setGeometry(labels[labels.size()-1]->x(), labels[labels.size()-1]->y(), 200, labels[labels.size()-1]->height());
    int loopnum;
    if(setmodelwin->getRunmodel()->getModel()->getparm()->get_run_total_step()){
        loopnum = setmodelwin->getRunmodel()->getModel()->getparm()->get_run_total_step()+1;
    }
    else{
        loopnum=0;
    }
    QSlider* slider = new QSlider(Qt::Horizontal, this);
    slider->setRange(0, loopnum);
    slider->setGeometry(10,125,1000,30);
    slider->setStyleSheet("QSlider { background-color: white; }");
    connect(slider, &QSlider::valueChanged, this, &runprogrampage::updateSquareSize);
    sliders.push_back(slider);
    QLabel* labelzeroa=setlabel("initial", 10, 155 ,15);
    genrallabels.push_back(labelzeroa);
    int stepnumall=setmodelwin->getRunmodel()->getModel()->getparm()->get_run_total_step();
    labels.push_back(setlabel(std::to_string(stepnumall), 1000, 155 ,15));
    labels[labels.size()-1]->setGeometry(labels[labels.size()-1]->x(), labels[labels.size()-1]->y(), 200, labels[labels.size()-1]->height());

    // Cam Yaw Functionality -> slider, interactive function call 
    sliderCamYaw = new QSlider(Qt::Horizontal, this);
    sliderCamYaw->setRange(-360, 360);
    sliderCamYaw->setGeometry(10,180,1000,30);
    sliderCamYaw->setStyleSheet("QSlider { background-color: white; }");
    labelIdxCamYaw = labels.size();
    connect(sliderCamYaw, &QSlider::valueChanged, this, &runprogrampage::rotateCameraYaw);
    sliders.push_back(sliderCamYaw);
    QLabel* labelcamYaw=setlabel("CamYaw", 10, 210 ,15);
    genrallabels.push_back(labelcamYaw);
    labels.push_back(setlabel(std::to_string(camYaw), 500, 160 ,15));

    // Cam Pitch Functionality -> vertical slider, interactive camera rotation
    sliderCamPitch = new QSlider(Qt::Vertical, this);
    sliderCamPitch->setRange(-90, 90);              
    sliderCamPitch->setGeometry(10, 250, 30, 500);
    sliderCamPitch->setStyleSheet("QSlider { background-color: white; }");
    QLabel* labelCamPitch = setlabel("CamPitch", 10, 800, 15);
    genrallabels.push_back(labelCamPitch);
    labelIdxCamPitch = labels.size();
    labels.push_back(setlabel(std::to_string(camPitch), 40, 500, 15));
    connect(sliderCamPitch, &QSlider::valueChanged, this, &runprogrampage::rotateCameraPitch);
    sliders.push_back(sliderCamPitch);


    view = new Qt3DExtras::Qt3DWindow();

    // root entity
    Qt3DCore::QEntity *rootEntity = new Qt3DCore::QEntity();
    Qt3DCore::QTransform *xrotationTransform = new Qt3DCore::QTransform();
    xrotationTransform->setRotation(QQuaternion::fromAxisAndAngle({1,0,0}, -90));
    xrotationTransform->setTranslation(QVector3D(0,2,0));
    rootEntity->addComponent(xrotationTransform);

    /*Qt3DRender::QPointLight *light = new Qt3DRender::QPointLight(rootEntity);
    light->setColor("white");
    light->setIntensity(1.0);
    light->setLinearAttenuation(0.1);*/
    Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(rootEntity);
    Qt3DRender::QPointLight *light = new Qt3DRender::QPointLight(lightEntity);
    light->setColor("white");
    light->setIntensity(1.0f);
    Qt3DCore::QTransform *lightTransform = new Qt3DCore::QTransform();
    lightTransform->setTranslation(QVector3D(5.0f, 5.0f, 5.0f));  // Position des Lichts
    lightEntity->addComponent(light);
    lightEntity->addComponent(lightTransform);

    // LIGHT 2 ---------------------------------------------------------
    Qt3DCore::QEntity *lightEntity2 = new Qt3DCore::QEntity(rootEntity);
    Qt3DRender::QPointLight *light2 = new Qt3DRender::QPointLight(lightEntity2);
    light2->setColor("white");
    light2->setIntensity(1.0f);
    Qt3DCore::QTransform *lightTransform2 = new Qt3DCore::QTransform();
    lightTransform2->setTranslation(QVector3D(-5.0f, -5.0f, 8.0f));  // Andere Position
    lightEntity2->addComponent(light2);
    lightEntity2->addComponent(lightTransform2);


    // LIGHT 3 ---------------------------------------------------------
    Qt3DCore::QEntity *lightEntity3 = new Qt3DCore::QEntity(rootEntity);
    Qt3DRender::QPointLight *light3 = new Qt3DRender::QPointLight(lightEntity3);
    light2->setColor("white");
    light2->setIntensity(1.0f);
    Qt3DCore::QTransform *lightTransform3 = new Qt3DCore::QTransform();
    lightTransform3->setTranslation(QVector3D(0.0f, -5.0f, 0.0f));  // Andere Position
    lightEntity3->addComponent(light3);
    lightEntity3->addComponent(lightTransform3);

    // create camera
    cameraEntity = view->camera();
    cameraEntity->lens()->setPerspectiveProjection(45.0f, 16.0/9.0, 0.1f, 1000.0f);
    cameraEntity->setPosition(QVector3D(0, 0, 10));
    cameraEntity->setUpVector(QVector3D(0, 1, 0));
    cameraEntity->setViewCenter(QVector3D(0, 0, 0));

    // set root entity
    view->setRootEntity(rootEntity);

    // set Qt3D into QWidget
    container = QWidget::createWindowContainer(view, this);
    container->setMinimumSize(1500, 900); 
    container->setFocusPolicy(Qt::StrongFocus);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    container->setGeometry(70, 250, 1200, 900);

    allentities.push_back(rootEntity);
    
    int bodynum=setmodelwin->getRunmodel()->getModel()->getparm()->getn_bodies();
    for(int i=0;i<bodynum;i++){
        Qt3DCore::QEntity *Entitynew = nullptr;
        allentities.push_back(Entitynew);
    }

    int musclenum=setmodelwin->getRunmodel()->getModel()->getparm()->getn_muscles();
    for(int i=0;i<musclenum;i++){
        muscle* currentmuscle=setmodelwin->getRunmodel()->getModel()->getparm()->getmuscleindex(i);
        for(int j=0;j<currentmuscle->getnodenum()-1;j++){
            Qt3DCore::QEntity *Entitynew = nullptr;
            allmuscleentities.push_back(Entitynew);
        }
    }

    // Coordinate axes  ----------------------------------------------------
    /*
    createAxis(rootEntity, QVector3D(0,0,0), QVector3D(1,0,0), QColor("red"));    // X
    createAxis(rootEntity, QVector3D(0,0,0), QVector3D(0,1,0), QColor("green"));  // Y
    createAxis(rootEntity, QVector3D(0,0,0), QVector3D(0,0,1), QColor("blue"));   // Z
    */
    createAxisCylinder(rootEntity, {0,0,0}, {1,0,0}, Qt::red,   0.02f);
    createAxisCylinder(rootEntity, {0,0,0}, {0,1,0}, Qt::green, 0.02f);
    createAxisCylinder(rootEntity, {0,0,0}, {0,0,1}, Qt::blue,  0.02f);

    drawallbody(0);
    drawallmuscle(0);
}

runprogrampage::~runprogrampage()
{
    for(int i=0;i<allentities.size();i++){
        delete allentities[i];
    }
    for(int i=0;i<colors.size();i++){
        delete colors[i];
    }
    for(int i=0;i<allmuscleentities.size();i++){
        delete allmuscleentities[i];
    }

    delete runButton;
    delete exportbutton;

    for(int i=0;i<sliders.size();i++){
        delete sliders[i];
    }
    for(int i=0;i<labels.size();i++){
        delete labels[i];
    }

    delete scaleedit;
    delete scaleeditbutton;
    delete runtimelabel;

    for(int i=0;i<genrallabels.size();i++){
        delete genrallabels[i];
    }
    delete cameraEntity;
    delete view;
    delete container;
    delete save_intervalEdit;
}

void runprogrampage::deleteentitiesnotroot(){
    for(int i=1;i<allentities.size();i++){
        delete allentities[i];
    }
}

void runprogrampage::runModelFunction()
{
    setmodelwin->getRunmodel()->getModel()->getparm()->resetallforrecalc();
    setmodelwin->getRunmodel()->getModel()->set_save_interval(save_intervalEdit->text().toInt());
    double runtime=setmodelwin->getRunmodel()->runprogramm();
    runtimelabel->setText(QString::fromStdString(doubletostring(runtime)+" s")); 
}

void runprogrampage::saveModel()
{
    setmodelwin->getRunmodel()->savemodel();
}

void runprogrampage::drawallbody(int rotationindex){
    int bodynum=setmodelwin->getRunmodel()->getModel()->getparm()->getn_bodies();
    for(int i=0;i<bodynum;i++){
        body* currentbody=setmodelwin->getRunmodel()->getModel()->getparm()->getbodyindex(i);
        if(currentbody->getshape()->getshapename()=="ellipsoid"){
            drawellipsoidbody(i, rotationindex);
        }
        if(currentbody->getshape()->getshapename()=="cylinder"){
            drawcylinderbody(i, rotationindex);
        }
        // new torus
        if(currentbody->getshape()->getshapename()=="torus"){
            drawTorusBody(i, rotationindex);
        }
    }
}

void runprogrampage::drawellipsoidbody(int index, int rotationindex){
    if(allentities[index+1]!=nullptr){
        delete allentities[index+1];  
    }
    allentities[index+1] = new Qt3DCore::QEntity(allentities[0]);
    body* currentbody=setmodelwin->getRunmodel()->getModel()->getparm()->getbodyindex(index);
    std::vector<std::vector<double>> currentbodyaxisangle_ref=currentbody->getbodybasic()->getaxisangle_ref();
    std::vector<std::vector<double>> currentq=currentbody->getbodybasic()->getq();
    // Create a mesh for the ellipsoid
    Qt3DExtras::QSphereMesh *shapeMesh = new Qt3DExtras::QSphereMesh();
    shapeMesh->setRadius(1.0*zoomsize); // set radius

    // Create a material for the ellipsoid (e.g., set its color or texture)
    Qt3DExtras::QPhongMaterial *shapeMaterial = new Qt3DExtras::QPhongMaterial();
    shapeMaterial->setDiffuse(*colors[index]); 
    // Transform to apply translation and rotation
    Qt3DCore::QTransform *transform = new Qt3DCore::QTransform();
    transform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(currentbodyaxisangle_ref[rotationindex][0],currentbodyaxisangle_ref[rotationindex][1],currentbodyaxisangle_ref[rotationindex][2]), currentbodyaxisangle_ref[rotationindex][3]));
    transform->setTranslation(QVector3D(currentq[rotationindex][0]*zoomsize,currentq[rotationindex][1]*zoomsize,currentq[rotationindex][2]*zoomsize));
    transform->setScale3D(QVector3D(currentbody->getshape()->geta(), currentbody->getshape()->getb(), currentbody->getshape()->getc())); 
    allentities[index+1]->addComponent(shapeMesh);
    allentities[index+1]->addComponent(shapeMaterial);
    allentities[index+1]->addComponent(transform);
}

void runprogrampage::drawTorusBody(int index, int rotationindex) {
    if(allentities[index+1] != nullptr) {
        delete allentities[index+1];
    }

    allentities[index+1] = new Qt3DCore::QEntity(allentities[0]);
    body* currentbody = setmodelwin->getRunmodel()->getModel()->getparm()->getbodyindex(index);
    std::vector<std::vector<double>> currentbodyaxisangle_ref = currentbody->getbodybasic()->getaxisangle_ref();
    std::vector<std::vector<double>> currentq = currentbody->getbodybasic()->getq();

    // Torus-Mesh erstellen
    Qt3DExtras::QTorusMesh* torusMesh = new Qt3DExtras::QTorusMesh();
    torusMesh->setRadius(currentbody->getshape()->geta() * zoomsize);       // großer Radius (von Zentrum zu Rohrmitte)
    torusMesh->setMinorRadius(currentbody->getshape()->getb() * zoomsize);  // kleiner Radius (Rohrradius)
    torusMesh->setRings(100);   // Anzahl der Ringe
    torusMesh->setSlices(20);   // Anzahl der Scheiben pro Ring

    // Material für den Torus
    Qt3DExtras::QPhongMaterial* torusMaterial = new Qt3DExtras::QPhongMaterial();
    torusMaterial->setDiffuse(*colors[index]);

    // Transform (Rotation, Translation, optional Skalierung)
    Qt3DCore::QTransform* transform = new Qt3DCore::QTransform();
    transform->setRotation(QQuaternion::fromAxisAndAngle(
        QVector3D(currentbodyaxisangle_ref[rotationindex][0],
                  currentbodyaxisangle_ref[rotationindex][1],
                  currentbodyaxisangle_ref[rotationindex][2]),
        currentbodyaxisangle_ref[rotationindex][3]
    ));
    transform->setTranslation(QVector3D(
        currentq[rotationindex][0] * zoomsize,
        currentq[rotationindex][1] * zoomsize,
        currentq[rotationindex][2] * zoomsize
    ));
    // Torus-Skalierung falls nötig (meist nur 1:1, da Radius schon gesetzt)
    float c = 1.0; // currentbody->getshape()->getc() * zoomsize;
    transform->setScale3D(QVector3D(1.0, 1.0, c));

    // Komponenten zur Entity hinzufügen
    allentities[index+1]->addComponent(torusMesh);
    allentities[index+1]->addComponent(torusMaterial);
    allentities[index+1]->addComponent(transform);

    // std::cout << "drawing torus: a=" << currentbody->getshape()->geta() << ", b=" << currentbody->getshape()->getb() << ", c=" << currentbody->getshape()->getc() << std::endl;
}

void runprogrampage::drawcylinderbody(int index, int rotationindex){
    if(allentities[index+1]!=nullptr){
        delete allentities[index+1];  
    }
    allentities[index+1] = new Qt3DCore::QEntity(allentities[0]);
    body* currentbody=setmodelwin->getRunmodel()->getModel()->getparm()->getbodyindex(index);
    std::vector<std::vector<double>> currentbodyaxisangle_ref=currentbody->getbodybasic()->getaxisangle_ref();
    std::vector<std::vector<double>> currentq=currentbody->getbodybasic()->getq();
    // Create a mesh for the ellipsoid
    Qt3DExtras::QCylinderMesh *shapeMesh = new Qt3DExtras::QCylinderMesh();
    shapeMesh->setRadius(currentbody->getshape()->geta());
    shapeMesh->setLength(2.0*currentbody->getshape()->getc());

    // Create a material for the ellipsoid (e.g., set its color or texture)
    Qt3DExtras::QPhongMaterial *shapeMaterial = new Qt3DExtras::QPhongMaterial();
    shapeMaterial->setDiffuse(*colors[index]); 
    // Transform to apply translation and rotation
    Qt3DCore::QTransform *transform = new Qt3DCore::QTransform();
    transform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(currentbodyaxisangle_ref[rotationindex][0],currentbodyaxisangle_ref[rotationindex][1],currentbodyaxisangle_ref[rotationindex][2]), currentbodyaxisangle_ref[rotationindex][3]));
    transform->setTranslation(QVector3D(currentq[rotationindex][0]*zoomsize,currentq[rotationindex][1]*zoomsize,currentq[rotationindex][2]*zoomsize));
    transform->setScale3D(QVector3D(1.0, currentbody->getshape()->getb()/currentbody->getshape()->geta(), 1.0)); 
    allentities[index+1]->addComponent(shapeMesh);
    allentities[index+1]->addComponent(shapeMaterial);
    allentities[index+1]->addComponent(transform);
}

void runprogrampage::drawallmuscle(int rotationindex){
    int musclenum=setmodelwin->getRunmodel()->getModel()->getparm()->getn_muscles();
    
    for(int j=0;j<allmuscleentities.size();j++){
        if(allmuscleentities[j]!=nullptr){
            delete allmuscleentities[j];
        }
    }
    int allnode=0;
    for(int i=0;i<musclenum;i++){
        muscle* currentmuscle=setmodelwin->getRunmodel()->getModel()->getparm()->getmuscleindex(i);
        drawmuscle(i, rotationindex,allnode);
        allnode=allnode+currentmuscle->getnodenum()-1;
    }
}

void runprogrampage::drawmuscle(int muscleindex, int rotationindex, int previousnodenum){
    muscle* currentmuscle=setmodelwin->getRunmodel()->getModel()->getparm()->getmuscleindex(muscleindex);
    std::vector<std::vector<double>> gammacurrentall=currentmuscle->getgammaall();
    for (int i = 0; i < currentmuscle->getnodenum()-1; ++i) {
        std::vector<double> pointcurrent={gammacurrentall[rotationindex][3*i+0]*zoomsize, gammacurrentall[rotationindex][3*i+1]*zoomsize, gammacurrentall[rotationindex][3*i+2]*zoomsize};
        std::vector<double> pointnext={gammacurrentall[rotationindex][3*i+3]*zoomsize, gammacurrentall[rotationindex][3*i+4]*zoomsize, gammacurrentall[rotationindex][3*i+5]*zoomsize};
        std::vector<double> translationline = vector3timeconstant(vector3plus(pointcurrent,pointnext),0.5);
        std::vector<double> vectorminus=vector3minus(pointnext,pointcurrent);
        std::vector<double> rotationaxis = calculationaxisangle(vectorminus,{0.0,0.0,1.0});
        double currentmusclelength=std::sqrt(vectortime1(vectorminus,vectorminus));

        allmuscleentities[i+previousnodenum] = new Qt3DCore::QEntity(allentities[0]);

        Qt3DExtras::QCylinderMesh *lineMesh = new Qt3DExtras::QCylinderMesh();
        lineMesh->setRadius(0.03);
        lineMesh->setLength(currentmusclelength*2);// set linesize

        Qt3DCore::QTransform *lineTransform = new Qt3DCore::QTransform();
        lineTransform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(rotationaxis[0],rotationaxis[1],rotationaxis[2]), rotationaxis[3]));
        lineTransform->setTranslation(QVector3D(translationline[0], translationline[1], translationline[2]));
        Qt3DExtras::QPhongMaterial *lineMaterial = new Qt3DExtras::QPhongMaterial();
        lineMaterial->setDiffuse(*colors[muscleindex]); 

        allmuscleentities[i+previousnodenum]->addComponent(lineMesh);
        allmuscleentities[i+previousnodenum]->addComponent(lineTransform);
        allmuscleentities[i+previousnodenum]->addComponent(lineMaterial);
    }
}

void runprogrampage::updateSquareSize(int size){
    if(size==0){
        labels[0]->setText(QString::fromStdString("initial"));
    }else{
        labels[0]->setText(QString::fromStdString("stepnum: "+std::to_string(size-1)));
    }
    if(setmodelwin->getRunmodel()->getModel()->getparm()->getn_bodies()>0){
        if(setmodelwin->getRunmodel()->getModel()->getparm()->getbodyindex(0)->getbodybasic()->getaxisangle_ref().size()>size){
            drawallbody(size);
            drawallmuscle(size);
        }
    }
    
}

void runprogrampage::setscale(){
    zoomsize=scaleedit->text().toDouble();
    drawallbody(0);
    drawallmuscle(0);
}

QLineEdit* runprogrampage::settext(const std::string& textdefault, int x, int y, int textwidth, int textheight ,int fontsize) {
    QLineEdit* body_nameEdit = new QLineEdit(this);
    QFont body_nameEditfont = body_nameEdit->font();
    body_nameEditfont.setPointSize(fontsize); 
    body_nameEdit->setFont(body_nameEditfont);
    body_nameEdit->setGeometry(x, y, textwidth, textheight);
    body_nameEdit->setText(QString::fromStdString(textdefault));
    body_nameEdit->setStyleSheet("QLineEdit { color: black; }");
    return body_nameEdit;
}

QLabel* runprogrampage::setlabel(const std::string& labelname, int x, int y ,int fontsize) {
    QLabel *body_namelabel = new QLabel(this);
    QFont body_namefont = body_namelabel->font();
    body_namefont.setPointSize(fontsize); 
    body_namelabel->setFont(body_namefont);
    body_namelabel->setText(QString::fromStdString(labelname)); 
    body_namelabel->move(x, y);
    body_namelabel->setStyleSheet("QLabel { color : black; background-color : white; }");
    return body_namelabel;
}

QLineEdit* runprogrampage::settextandlabel(const std::string& labelname, const std::string& textdefault, int x, int y, int textwidth, int textheight, int fontsize) {
    QLabel* body_label=setlabel(labelname, x, y, fontsize);
    QLineEdit* body_nameEdit = settext(textdefault, x, y+25, textwidth, textheight, fontsize);
    return body_nameEdit;
}

std::string runprogrampage::doubletostring(double num) {
    std::ostringstream stream;
    stream << num;
    std::string numStr = stream.str();
    return numStr;
}

void runprogrampage::updatevalue(){
    int stepnumall=setmodelwin->getRunmodel()->getModel()->getparm()->get_run_total_step();
    labels[1]->setText(QString::fromStdString(std::to_string(stepnumall)));
    int loopnum;
    if(stepnumall){
        loopnum = setmodelwin->getRunmodel()->getModel()->getparm()->get_run_total_step()+1;
    }
    else{
        loopnum=0;
    }
    sliders[0]->setRange(0, loopnum);
    int allentitiesnum=allentities.size();
    for(int i=1;i<allentitiesnum;i++){
        if(allentities[1]!=nullptr){
            delete allentities[1];
        }
        allentities.erase(allentities.begin() + 1); 
    }

    int allmuscleentitiesnum=allmuscleentities.size();
    for(int j=0;j<allmuscleentitiesnum;j++){
        if(allmuscleentities[0]!=nullptr){
            delete allmuscleentities[0];
        }
        allmuscleentities.erase(allmuscleentities.begin() + 0); 
    }

    int bodynum=setmodelwin->getRunmodel()->getModel()->getparm()->getn_bodies();
    for(int i=0;i<bodynum;i++){
        Qt3DCore::QEntity *Entitynew = nullptr;
        allentities.push_back(Entitynew);
    }

    int musclenum=setmodelwin->getRunmodel()->getModel()->getparm()->getn_muscles();
    for(int i=0;i<musclenum;i++){
        muscle* currentmuscle=setmodelwin->getRunmodel()->getModel()->getparm()->getmuscleindex(i);
        for(int j=0;j<currentmuscle->getnodenum()-1;j++){
            Qt3DCore::QEntity *Entitynew = nullptr;
            allmuscleentities.push_back(Entitynew);
        }
    }
    drawallbody(0);
    drawallmuscle(0);
}

void runprogrampage::rotateCameraYaw(int value)
{
    camYaw = value;  // oder z.B. value * 0.1f für feinere Kontrolle

    // Rotation um Y-Achse (Yaw)
    QQuaternion rotation = QQuaternion::fromAxisAndAngle(QVector3D(0,1,0), camYaw);

    cameraEntity->setViewCenter(QVector3D(0,0,0));
    cameraEntity->setPosition(rotation.rotatedVector(QVector3D(0,0,10)));

    labels[labelIdxCamYaw]->setText(QString::number(camYaw));
}

void runprogrampage::rotateCameraPitch(int value)
{
    camPitch = value;

    QQuaternion qyaw = QQuaternion::fromAxisAndAngle(QVector3D(0,1,0), camYaw);
    QQuaternion qpitch = QQuaternion::fromAxisAndAngle(QVector3D(1,0,0), camPitch);

    QVector3D basePos(0, 0, 10);
    QVector3D rotated = qyaw.rotatedVector(qpitch.rotatedVector(basePos));

    cameraEntity->setPosition(rotated);
    cameraEntity->setViewCenter(QVector3D(0,0,0));

    labels[labelIdxCamPitch]->setText(QString::number(camPitch));
}

Qt3DCore::QEntity* runprogrampage::createAxis(Qt3DCore::QEntity* parent,const QVector3D& start,const QVector3D& end,const QColor& color)
{
    using namespace Qt3DCore;
    using namespace Qt3DRender;

    QEntity* entity = new QEntity(parent);

    // Geometry ---------------------------
    QGeometry *geometry = new QGeometry(entity);

    QByteArray bufferBytes;
    bufferBytes.resize(2 * 3 * sizeof(float)); // 2 Punkte * 3 floats
    float *positions = reinterpret_cast<float*>(bufferBytes.data());

    positions[0] = start.x();
    positions[1] = start.y();
    positions[2] = start.z();
    positions[3] = end.x();
    positions[4] = end.y();
    positions[5] = end.z();

    QBuffer *buffer = new QBuffer(geometry);
    buffer->setData(bufferBytes);

    QAttribute *positionAttribute = new QAttribute();
    positionAttribute->setName(QAttribute::defaultPositionAttributeName());
    positionAttribute->setBuffer(buffer);
    positionAttribute->setVertexBaseType(QAttribute::Float);
    positionAttribute->setVertexSize(3);
    positionAttribute->setCount(2);
    positionAttribute->setByteStride(3 * sizeof(float));
    positionAttribute->setByteOffset(0);
    positionAttribute->setAttributeType(QAttribute::VertexAttribute);

    geometry->addAttribute(positionAttribute);

    // Renderer ----------------------------
    QGeometryRenderer *renderer = new QGeometryRenderer(entity);
    renderer->setPrimitiveType(QGeometryRenderer::Lines);
    renderer->setGeometry(geometry);

    // Material ----------------------------
    Qt3DExtras::QPhongMaterial *material = new Qt3DExtras::QPhongMaterial(entity);
    material->setDiffuse(color);

    entity->addComponent(renderer);
    entity->addComponent(material);

    return entity;
}

Qt3DCore::QEntity* runprogrampage::createAxisCylinder(Qt3DCore::QEntity *root,const QVector3D &start,const QVector3D &end,const QColor &color,float radius)
{
    auto *entity = new Qt3DCore::QEntity(root);

    // Länge
    QVector3D diff = end - start;
    float length = diff.length();

    // Mesh
    auto *cylinder = new Qt3DExtras::QCylinderMesh(entity);
    cylinder->setRadius(radius);         // ← Dicke einstellen!
    cylinder->setLength(length);
    cylinder->setRings(10);
    cylinder->setSlices(20);

    // Material
    auto *mat = new Qt3DExtras::QPhongMaterial(entity);
    mat->setDiffuse(color);

    // Transform
    auto *tr = new Qt3DCore::QTransform();

    // Position
    tr->setTranslation(start + diff * 0.5f);

    // Rotation: Richtung der Achse
    QQuaternion rot = QQuaternion::rotationTo(QVector3D(0,1,0), diff.normalized());
    tr->setRotation(rot);

    entity->addComponent(cylinder);
    entity->addComponent(mat);
    entity->addComponent(tr);

    return entity;
}

void runprogrampage::setRotationDefault(){

    camPitch = 0.f;
    camYaw = 0.f;

    labels[labelIdxCamYaw]->setText(QString::number(camYaw));
    sliderCamYaw->setValue(camPitch);
    labels[labelIdxCamPitch]->setText(QString::number(camPitch));
    sliderCamPitch->setValue(camYaw);

    rotateCameraYaw(camPitch);
    rotateCameraPitch(camYaw);
}


