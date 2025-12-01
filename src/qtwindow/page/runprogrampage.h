/*

Copyright (C) 2023-2031 Friedrich-Alexander-Universität Erlangen-Nürnberg
03.11.2023
Xiyu Chen

*/

#ifndef RUNPROGRAMPAGE_H
#define RUNPROGRAMPAGE_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include "../../run_model.h"
#include <QSlider>


#include <Qt3DCore/QEntity>
#include <Qt3DExtras/QSphereMesh>
#include <Qt3DExtras/QCylinderMesh>
#include <Qt3DExtras/QTorusMesh>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QPointLight>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras/QOrbitCameraController>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DCore/QTransform>

#include <Qt3DRender/QGeometry>
#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DRender/QAttribute>
#include <Qt3DRender/QBuffer>


class setmodelwindow;

class runprogrampage : public QWidget
{
    Q_OBJECT

public:
    runprogrampage(setmodelwindow *setmodelwin, QWidget *parent = nullptr);
    ~runprogrampage();
    void deleteentitiesnotroot();
    void drawallbody(int rotationindex);
    void drawellipsoidbody(int index, int rotationindex);
    void drawcylinderbody(int index, int rotationindex);
    void drawTorusBody(int index, int rotationindex); // new torus
    void drawallmuscle(int rotationindex);
    void drawmuscle(int muscleindex, int rotationindex, int previousnodenum);
    QLineEdit* settext(const std::string& textdefault, int x, int y, int textwidth, int textheight ,int fontsize);
    QLabel* setlabel(const std::string& labelname, int x, int y ,int fontsize);
    QLineEdit* settextandlabel(const std::string& labelname, const std::string& textdefault, int x, int y, int textwidth, int textheight, int fontsize);
    std::string doubletostring(double num);
    void updatevalue();
    
    
private:
    setmodelwindow *setmodelwin=nullptr;
    QPushButton *runButton=nullptr;
    QPushButton *exportbutton=nullptr;

    QLineEdit *save_intervalEdit=nullptr;

    int currentrotationangle=0;
    std::vector<QSlider *> sliders;
    std::vector<QLabel *> labels;

    QLineEdit* scaleedit=nullptr;
    QPushButton *scaleeditbutton=nullptr;
    QLineEdit* rotateedit=nullptr;
    QPushButton *rotateeditbutton=nullptr;

    std::vector<Qt3DCore::QEntity*> allentities;
    std::vector<Qt3DCore::QEntity*> allmuscleentities;
    QLabel* runtimelabel = nullptr;
    QWidget *container = nullptr;
    std::vector<QLabel *> genrallabels;
    Qt3DRender::QCamera *cameraEntity = nullptr;
    Qt3DExtras::Qt3DWindow *view = nullptr;

    std::vector<QColor*> colors;
    double zoomsize=5.0;

    float camYaw = 0.0f;
    float camPitch = 0.0f;
    int labelIdxCamYaw;
    int labelIdxCamPitch;
    QSlider* sliderCamPitch;
    QSlider* sliderCamYaw;

    void rotateCameraYaw(int value);
    void rotateCameraPitch(int value);
    void setRotationDefault();
    Qt3DCore::QEntity* createAxis(Qt3DCore::QEntity* parent,const QVector3D& start,const QVector3D& end,const QColor& color);
    Qt3DCore::QEntity* createAxisCylinder(
        Qt3DCore::QEntity *root,
        const QVector3D &start,
        const QVector3D &end,
        const QColor &color,
        float radius);

private slots:
    void runModelFunction();
    void saveModel();
    void updateSquareSize(int size);
    void setscale();
};

#endif // RUNPROGRAMPAGE_H
