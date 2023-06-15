#include "QtWidget_VtkSandbox.h"

#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QImage>

#include "TDD_Raytracer.h"

//todo: rename this file!
QtWidget_VtkSandbox::QtWidget_VtkSandbox(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    QGraphicsScene* scene = new QGraphicsScene();
    ui.graphicsView->setScene(scene);
    QImage* image = new QImage();
    // todo: use the filename of the relevant ppm / image
    // wait for the rendering to finish before trying to load the image
    //image->load("X_512Y_256Z_-64.ppm");
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(*image));
    scene->addItem(item);
    ui.graphicsView->show();

    QObject::connect(ui.pushButton, &QPushButton::clicked, this, &QtWidget_VtkSandbox::onRenderButtonClicked);
    //todo: use this function to read the light pos set by the user (?), or is there a better way?
    //QObject::connect(ui.lightPos_X, &QLineEdit::selectionChanged, this, &QtWidget_VtkSandbox::onLightPosXChanged);
}

QtWidget_VtkSandbox::~QtWidget_VtkSandbox()
{}

//todo: use this function to read the light pos set by the user (?), or is there a better way?
void QtWidget_VtkSandbox::onLightPosXChanged()
{
}

void QtWidget_VtkSandbox::onRenderButtonClicked()
{
    //todo: would(nt) it be better to have this as a static function? -> think of a nicer way to actually trigger the rendering
    TDD_Raytracer raytracer{};
    //todo: use the values from the UI instead of hardcoded values!
    constexpr float light_x{ 512.0 }, light_y{ 256.0 }, light_z{ -64.0 };
    raytracer.drawSphereWithPhongShading(light_x, light_y, light_z);
}
