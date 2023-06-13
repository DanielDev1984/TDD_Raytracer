#include "QtWidget_VtkSandbox.h"

#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QImage>


QtWidget_VtkSandbox::QtWidget_VtkSandbox(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    QGraphicsScene* scene = new QGraphicsScene();
    ui.graphicsView->setScene(scene);
    QImage* image = new QImage();
    image->load("X_512Y_256Z_-64.ppm");
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(*image));
    scene->addItem(item);
    ui.graphicsView->show();
}

QtWidget_VtkSandbox::~QtWidget_VtkSandbox()
{}
