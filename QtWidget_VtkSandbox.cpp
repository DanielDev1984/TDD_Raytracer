#include "QtWidget_VtkSandbox.h"

#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QImage>

#include <vtkSphereSource.h>
#include <vtkArrowSource.h>
#include <vtkActor.h>
#include <vtkImageActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkPNMReader.h>
#include <vtkImageCanvasSource2D.h>
#include <vtkImageData.h>
#include <vtkCamera.h>
#include <vtkPointData.h>
#include <vtkGlyph2D.h>
#include <vtkImageSliceMapper.h>
#include <vtkImageSlice.h>
#include <vtkNamedColors.h>
#include <vtkProperty.h>
#include <vtkMaskPoints.h>


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
    //todo: would(nt) it be better to have this as a static function? -> think of a nicer way to actually trigger the rendering
    TDD_Raytracer raytracer{};
    //todo: use the values from the UI instead of hardcoded values!
    constexpr float light_x{ 512.0 }, light_y{ 256.0 }, light_z{ -64.0 };
    raytracer.drawSphereWithPhongShading(light_x, light_y, light_z, "renderedImage", false);

    // read input data
    //todo: outsource this to own function
    vtkSmartPointer<vtkPNMReader> imageReader_bG{ vtkSmartPointer<vtkPNMReader>::New() };
    vtkSmartPointer<vtkPNMReader> imageReader_fG{ vtkSmartPointer<vtkPNMReader>::New() };
    // for some reason, the input data range seems to be restricted to maxVal 128 (?)
    // the input data needs to be a binary (i.e. P6) ppm
    constexpr auto fN_bG{ "C:\\Users\\strai\\source\\TDD_raytracer\\TDD_Raytracer\\renderedImage_P6.ppm" };
    constexpr auto fN_fG{ "C:\\Users\\strai\\source\\TDD_raytracer\\TDD_Raytracer\\vectorfield.ppm" };
    
    //todo: figure out how to use vtkOutpuWindow for managing the application output
    std::cout << "input filename foreground: " << fN_bG << "\n";
    std::cout << "input filename foreground: " << fN_fG << "\n";
    
    auto const fileReadSucces_fG{ (imageReader_fG->CanReadFile(fN_fG) > 0) ? "image file fG could be read" : "image file fG could not be read" };
    std::cout << fileReadSucces_fG << "\n";
    
    // setup the background
    auto const fileReadSucces_bG{ (imageReader_bG->CanReadFile(fN_bG) > 0) ? "image file bG could be read" : "image file bG could not be read" };
    std::cout << fileReadSucces_bG << "\n";
    imageReader_bG->SetFileName(fN_bG);
    imageReader_bG->Update();
    const auto imageData_bG{ imageReader_bG->GetOutput() };
    vtkSmartPointer<vtkImageActor> actor_bG{ vtkSmartPointer<vtkImageActor>::New() };
    actor_bG->SetInputData(imageData_bG);
    m_Renderer_bG->AddViewProp(actor_bG);
    m_Renderer_bG->ResetCamera();
    /////////////////////

    const auto imageDimensions_bG{ imageData_bG->GetDimensions() };
    std::cout << "imageDimensions_bG_X: " << imageDimensions_bG[0] << "\n";
    std::cout << "imageDimensions_bG_Y: " << imageDimensions_bG[1] << "\n";
    std::cout << "imageDimensions_bG_Z: " << imageDimensions_bG[2] << "\n";
   

    // setup the "overlay" / fG
    /////////////////////
    imageReader_fG->SetFileName(fN_fG);
    imageReader_fG->Update();
    auto imageData_fG{ imageReader_fG->GetOutput() };
    

    const auto imageDimensions_fG{ imageData_fG->GetDimensions() };
    std::cout << "imageDimensions_fG_X: " << imageDimensions_fG[0] << "\n";
    std::cout << "imageDimensions_fG_Y: " << imageDimensions_fG[1] << "\n";
    std::cout << "imageDimensions_fG_Z: " << imageDimensions_fG[2] << "\n";
  
    // setup the mask used for populating the glyphfilter
    constexpr auto samplingRate{ 700 };
    vtkNew<vtkMaskPoints> maskPoints;
    maskPoints->SetOnRatio(samplingRate); // keep every nth (== "samplingRate") point

    
    maskPoints->SetInputData(imageData_fG);
    
    
    maskPoints->Update();
    // update needs to be called first, otherwise data is not yet available!
    // make sure the orientation of the input (vectorfield) is used for orienting the glyphs
    maskPoints->GetOutput()->GetPointData()->SetActiveVectors(maskPoints->GetOutput()->GetPointData()->GetScalars()->GetName());
    
    vtkSmartPointer<vtkArrowSource> arrowSource{ vtkSmartPointer<vtkArrowSource>::New() };
    arrowSource->InvertOff();
    arrowSource->Update();
   
    

    

    vtkSmartPointer<vtkGlyph2D> glyphFilter{ vtkSmartPointer<vtkGlyph2D>::New() };
    glyphFilter->SetInputConnection(maskPoints->GetOutputPort());
    glyphFilter->SetSourceConnection(arrowSource->GetOutputPort());
    
    glyphFilter->ScalingOn();
    glyphFilter->OrientOn();
    glyphFilter->SetScaleModeToScaleByScalar();
    glyphFilter->SetScaleFactor(0.1); //todo: normalize the size of the arrow glyphs!
    
    glyphFilter->Update();

    vtkSmartPointer<vtkPolyDataMapper> vectorMapper{ vtkSmartPointer<vtkPolyDataMapper>::New() };
    // without this, trying to set the color of the arrows manually lateron isnt working (and instead a scalar value of the vector is being used)
    vectorMapper->ScalarVisibilityOff();
    vectorMapper->SetInputConnection(glyphFilter->GetOutputPort());

    vtkSmartPointer<vtkActor> vectorActor{ vtkSmartPointer<vtkActor>::New() };

    vectorActor->SetMapper(vectorMapper);
    vtkNew<vtkNamedColors> colors;
    auto constexpr glyphColor{ "White" };
    vectorActor->GetProperty()->SetColor(
        colors->GetColor3d(glyphColor).GetData());

    // setting up the "rest" of the scene

    // todo: what is the image slice used for? it creates a black background when being added to the renderer!
    // could the imageSlice be used to hold the actual "backgroundimage"?
    //m_Renderer_fG->AddViewProp(imageSlice);

    //todo: hack for letting the manually generated vectorfield point in the correct direction (indicating from where the light is comming for rendering the sphere
    vectorActor->SetScale(-1,1,1);
    
    m_Renderer_fG->AddViewProp(vectorActor);
    m_Renderer_fG->ResetCamera();
    m_RenderWindow->Render();


    //this sets the cameras to cover all of the available openglwindow(space)
    ////////////////////////////
    // bG
    auto origin_bG{ imageData_bG->GetOrigin() };
    auto spacing_bG{ imageData_bG->GetSpacing() };
    auto extent_bG{ imageData_bG->GetExtent() };

    auto camera_bG{ m_Renderer_bG->GetActiveCamera() };
    camera_bG->ParallelProjectionOn();

    auto xc_bG{ origin_bG[0] + 0.5 * (extent_bG[0] + extent_bG[1]) * spacing_bG[0] };
    auto yc_bG{ origin_bG[1] + 0.5 * (extent_bG[2] + extent_bG[3]) * spacing_bG[1] };

    auto yd_bG{ (extent_bG[3] - extent_bG[2] + 1) * spacing_bG[1] };
    auto d_bG{ camera_bG->GetDistance() };

    camera_bG->SetParallelScale(0.5 * yd_bG);
    camera_bG->SetFocalPoint(xc_bG, yc_bG, 0.0);
    camera_bG->SetPosition(xc_bG, yc_bG, d_bG);

    // fG
    auto origin_fG{ imageData_fG->GetOrigin() };
    auto spacing_fG{ imageData_fG->GetSpacing() };
    auto extent_fG{ imageData_fG->GetExtent() };

    auto camera_fG{ m_Renderer_fG->GetActiveCamera() };
    camera_fG->ParallelProjectionOn();
    
    auto xc_fG{ origin_fG[0] + 0.5 * (extent_fG[0] + extent_fG[1]) * spacing_fG[0] };
    auto yc_fG{ origin_fG[1] + 0.5 * (extent_fG[2] + extent_fG[3]) * spacing_fG[1] };

    auto yd_fG{ (extent_fG[3] - extent_fG[2] + 1) * spacing_fG[1] };
    auto d_fG{ camera_fG->GetDistance() };
    camera_fG->SetParallelScale(0.5 * yd_fG);
    //todo this breaks when scaling the actor... is this correct now, or not?
    /*camera_fG->SetFocalPoint(xc_fG, yc_fG, 0.0);
    camera_fG->SetPosition(xc_fG, yc_fG, d_fG);*/
    ////////////////////////////
    
    m_RenderWindow->Render();


    
}
