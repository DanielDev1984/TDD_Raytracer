#include "QtWidget_VtkSandbox.h"

#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QImage>

#include <vtkSphereSource.h>
#include <vtkActor.h>
#include <vtkImageActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkJPEGReader.h>
#include <vtkImageCanvasSource2D.h>
#include <vtkImageData.h>
#include <vtkCamera.h>

#include "TDD_Raytracer.h"

//todo: rename this file!
QtWidget_VtkSandbox::QtWidget_VtkSandbox(QWidget *parent)
    : QMainWindow(parent),
    m_RenderWindow{ vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New() },
    m_Renderer_fG{ vtkSmartPointer<vtkRenderer>::New() },
    m_Renderer_bG{ vtkSmartPointer<vtkRenderer>::New() },
    m_Interactor{ vtkSmartPointer<QVTKInteractor>::New() },
    m_InteractorStyle{ vtkSmartPointer<vtkInteractorStyle>::New() }
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

    m_RenderWindow->AddRenderer(m_Renderer_fG);
    m_RenderWindow->AddRenderer(m_Renderer_bG);
    m_RenderWindow->SetInteractor(m_Interactor);
    m_RenderWindow->SetNumberOfLayers(2);
    m_Renderer_bG->SetLayer(0);
    m_Renderer_fG->SetLayer(1);
    ui.openGLWidget->setRenderWindow(m_RenderWindow);
    m_Interactor->SetInteractorStyle(m_InteractorStyle);
    m_Interactor->Initialize();


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
    vtkSmartPointer<vtkSphereSource> sphereSource{ vtkSmartPointer<vtkSphereSource>::New() };
    sphereSource->SetRadius(2);
    sphereSource->Update();
    vtkSmartPointer<vtkPolyDataMapper> sphereMapper{ vtkSmartPointer<vtkPolyDataMapper>::New() };
    sphereMapper->SetInputData(sphereSource->GetOutput());
    vtkSmartPointer<vtkActor> sphere{ vtkSmartPointer<vtkActor>::New() };
    sphere->SetMapper(sphereMapper);

    // setting up the background actor
    vtkSmartPointer<vtkJPEGReader> jpegReader{ vtkSmartPointer<vtkJPEGReader>::New() };
    const auto fN{ "C:\\Users\\strai\\source\\TDD_raytracer\\TDD_Raytracer\\X_512Y_256Z_-64.jpg" };
    jpegReader->CanReadFile(fN);

    jpegReader->SetFileName(fN);
    jpegReader->Update();
    auto imageData{ jpegReader->GetOutput() };
    vtkSmartPointer<vtkImageActor> bg_Image{ vtkSmartPointer<vtkImageActor>::New() };
    bg_Image->SetInputData(imageData);
    m_Renderer_bG->AddViewProp(bg_Image);
    m_Renderer_bG->ResetCamera();

    // setting up the "rest" of the scene

    m_Renderer_fG->AddViewProp(sphere);
    m_Renderer_fG->ResetCamera();
    m_RenderWindow->Render();

    // setup bg camera to fill the renderer with the image
    auto origin{ imageData->GetOrigin() };
    auto spacing{ imageData->GetSpacing() };
    auto extent{ imageData->GetExtent() };

    auto camera{ m_Renderer_bG->GetActiveCamera() };
    camera->ParallelProjectionOn();

    auto xc{ origin[0] + 0.5 * (extent[0] + extent[1]) * spacing[0] };
    auto yc{ origin[1] + 0.5 * (extent[2] + extent[3]) * spacing[1] };

    auto yd{ (extent[3] - extent[2] + 1) * spacing[1] };
    auto d{ camera->GetDistance() };
    camera->SetParallelScale(0.5 * yd);
    camera->SetFocalPoint(xc, yc, 0.0);
    camera->SetPosition(xc, yc, d);

    m_RenderWindow->Render();


    //todo: would(nt) it be better to have this as a static function? -> think of a nicer way to actually trigger the rendering
    TDD_Raytracer raytracer{};
    //todo: use the values from the UI instead of hardcoded values!
    constexpr float light_x{ 512.0 }, light_y{ 256.0 }, light_z{ -64.0 };
    raytracer.drawSphereWithPhongShading(light_x, light_y, light_z);
}
