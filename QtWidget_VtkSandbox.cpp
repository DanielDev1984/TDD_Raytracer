#include "QtWidget_VtkSandbox.h"

#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QGraphicsBlurEffect>
#include <QGraphicsOpacityEffect>
#include <QImage>
#include <qpainter.h>

#include <vtkActor.h>
#include <vtkImageActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkPNMReader.h>
#include <vtkImageData.h>
#include <vtkCamera.h>
#include <vtkPointData.h>
#include <vtkGlyph2D.h>
#include <vtkNamedColors.h>
#include <vtkProperty.h>
#include <vtkMaskPoints.h>
#include <vtkGlyphSource2D.h>


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

    m_RenderWindow->AddRenderer(m_Renderer_fG);
    m_RenderWindow->AddRenderer(m_Renderer_bG);
    m_RenderWindow->SetInteractor(m_Interactor);
    m_RenderWindow->SetNumberOfLayers(2);
    m_Renderer_bG->SetLayer(0);
    m_Renderer_fG->SetLayer(1);
    ui.openGLWidget->setRenderWindow(m_RenderWindow);
    m_Interactor->SetInteractorStyle(m_InteractorStyle);
    m_Interactor->Initialize();

    QPixmap pixmap(256, 256);
    pixmap.fill(QColor("transparent"));
    QPainter painter(&pixmap);
    //todo take color set from the GUI


    QRadialGradient gr(128,128,200);
    gr.setColorAt(0.0, QColor(0, 255, 0, 191));
    gr.setColorAt(0.2, QColor(0, 200, 0, 191));
    gr.setColorAt(0.9, QColor(0, 150, 0, 63));
    gr.setColorAt(0.95, QColor(0, 0, 0, 127));
    gr.setColorAt(1, QColor(0, 0, 0, 0));

    painter.setBrush(gr);
    //todo take coordinates set from the GUI
    QPoint centerpoint{ 128, 0 };
    painter.drawEllipse(centerpoint, 128,128);

    

    QImage testImage{ pixmap.toImage() };
    
    QGraphicsPixmapItem item;
    item.setPixmap(QPixmap::fromImage(testImage));
    
    ui.painterLabel->setPixmap(pixmap);

    QPixmap pixmap2(41, 41);
    pixmap2.fill(QColor("transparent"));

    QPainter painter2(&pixmap2);
    //todo take color set from the GUI
    painter2.setBrush(QBrush(Qt::red));
    
    //todo take coordinates set from the GUI
    QPoint centerpoint2{ 20, 20 };
    painter2.drawEllipse(centerpoint2, 20, 20);
    ui.colorPreviewLabel->setPixmap(pixmap2);


    QObject::connect(ui.pushButton, &QPushButton::clicked, this, &QtWidget_VtkSandbox::onRenderButtonClicked);
    QObject::connect(ui.spherePosSliderY, &QSlider::valueChanged, this, &QtWidget_VtkSandbox::redrawRaytracePreview);
    QObject::connect(ui.spherePosSliderX, &QSlider::valueChanged, this, &QtWidget_VtkSandbox::redrawRaytracePreview);
    //rgb sphere color
    QObject::connect(ui.colorSliderR, &QSlider::valueChanged, this, &QtWidget_VtkSandbox::redrawRaytracePreview);
    QObject::connect(ui.colorSliderG, &QSlider::valueChanged, this, &QtWidget_VtkSandbox::redrawRaytracePreview);
    QObject::connect(ui.colorSliderB, &QSlider::valueChanged, this, &QtWidget_VtkSandbox::redrawRaytracePreview);
    //todo: add a function to redraw the sphere preview 
    QObject::connect(ui.lightPosSliderX, &QSlider::valueChanged, this, &QtWidget_VtkSandbox::redrawRaytracePreview);
    QObject::connect(ui.lightPosSliderY, &QSlider::valueChanged, this, &QtWidget_VtkSandbox::redrawRaytracePreview);

    //todo: make it possible to toggle visualization of lightsource overlay by using the checkbox
    
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
    //todo: get rid of magic number used for scaling the sliderPos wrt to the actual light position used for the rendering
    const float light_x{ float(ui.lightPosSliderX->value() *2.0) }, light_y{ float(ui.lightPosSliderY->value() * 2.0) }, light_z{ -64.0 };
    //todo: pass the sphere position parameter from the gui to this function
    std::cout << "lightSourcePos X: " << ui.lightPosSliderX->value() << "\n";
    std::cout << "lightSourcePos Y: " << ui.lightPosSliderY->value() << "\n";

    raytracer.drawSphereWithPhongShading(light_x, light_y, light_z, "renderedImage", false);

    // read input data
    //todo: outsource this to own function
    vtkSmartPointer<vtkPNMReader> imageReader_bG{ vtkSmartPointer<vtkPNMReader>::New() };
    //todo: investigate -> for some reason, the input data range seems to be restricted to maxVal 128 (?)
    // the input data needs to be a binary (i.e. P6) ppm
    constexpr auto fN_bG{ "C:\\Users\\strai\\source\\TDD_raytracer\\TDD_Raytracer\\renderedImage.ppm" };
    
    //todo: figure out how to use vtkOutpuWindow for managing the application output
    std::cout << "input filename background: " << fN_bG << "\n";
    
    // setup the background
    auto const fileReadSucces_bG{ (imageReader_bG->CanReadFile(fN_bG) > 0) ? "image file bG could be read" : "!! image file bG could not be read !!" };
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
    
    const auto imageDimensions_fG{ imageDimensions_bG };
    std::cout << "imageDimensions_fG_X: " << imageDimensions_fG[0] << "\n";
    std::cout << "imageDimensions_fG_Y: " << imageDimensions_fG[1] << "\n";
    std::cout << "imageDimensions_fG_Z: " << imageDimensions_fG[2] << "\n";


    //generate Vectorfield
    //todo: outsource this to TDDRaytracer::generateVectorfield / ::generateLightsourceRepresentation (?)
    vtkNew<vtkImageData> imageData_VectorField;
    imageData_VectorField->SetDimensions(imageDimensions_fG[0], imageDimensions_fG[1], imageDimensions_fG[2]);
    imageData_VectorField->AllocateScalars(VTK_INT, 3);
    
    // this factor takes into account that on the raytracer side sth seems to be off when it comes to placing the lightsource
    //todo: get rid of scaling magicnumber
    constexpr auto cameraPosCorrectionFactor{ 2.0 };
    const auto lightXPos{ light_x / cameraPosCorrectionFactor };
    //todo: get rid of "coordinate-flip" magic number
    const auto lightYPos{ 256 - (light_y / cameraPosCorrectionFactor) };
    constexpr auto glyphScalingFactor{ 7 };

    int* fGImageDims = imageData_VectorField->GetDimensions();

    for (int z = 0; z < fGImageDims[2]; z++)
    {
        for (int y = 0; y < fGImageDims[1]; y++)
        {
            for (int x = 0; x < fGImageDims[0]; x++)
            {
                
                {
                    int* pixel =
                        static_cast<int*>(imageData_VectorField->GetScalarPointer(x, y, z));
                    //todo: calculate unitvector by means of vtk, not own implementation and make this a little bit more efficient
                    const auto length{sqrt(double(x - lightXPos)* double(x - lightXPos) + double(y - lightYPos)* double(y - lightYPos))};
                    pixel[0] = glyphScalingFactor * double(x - lightXPos) / length;
                    pixel[1] = glyphScalingFactor * double(y - lightYPos) / length;
                    pixel[2] = 0;
                }
            }
        }
    }

    imageData_VectorField->GetPointData()->SetActiveVectors(imageData_VectorField->GetPointData()->GetScalars()->GetName());
  
    // setup the mask used for populating the glyphfilter
    constexpr auto samplingRate{ 200 };
    vtkNew<vtkMaskPoints> maskPoints;
    maskPoints->SetOnRatio(samplingRate); // keep every nth (== "samplingRate") point

    maskPoints->SetInputData(imageData_VectorField);
    
    
    maskPoints->Update();
    // update needs to be called first, otherwise data is not yet available!
    // make sure the correct orientation of the input (vectorfield) is used for orienting the glyphs lateron
   maskPoints->GetOutput()->GetPointData()->SetActiveVectors(imageData_VectorField->GetPointData()->GetVectors()->GetName());
    
    
    vtkSmartPointer<vtkGlyphSource2D> arrowSource{ { vtkSmartPointer<vtkGlyphSource2D>::New() } };
    arrowSource->SetGlyphTypeToArrow();
    arrowSource->Update();
    

    vtkSmartPointer<vtkGlyph2D> glyphFilter{ vtkSmartPointer<vtkGlyph2D>::New() };
    glyphFilter->SetInputConnection(maskPoints->GetOutputPort());
    glyphFilter->SetSourceConnection(arrowSource->GetOutputPort());
    glyphFilter->SetOrient(true);
    // this is essential for actually orienting the glyphs correctly lateron!
    glyphFilter->SetScaleModeToScaleByVector(); 
    glyphFilter->SetVectorModeToUseVector();
    glyphFilter->GetOutput()->GetPointData()->SetActiveVectors(imageData_VectorField->GetPointData()->GetVectors()->GetName());
    glyphFilter->Update();

    vtkSmartPointer<vtkPolyDataMapper> vectorMapper{ vtkSmartPointer<vtkPolyDataMapper>::New() };
    // without this, trying to set the color of the arrows manually lateron isnt working (and instead a scalar value of the vector is being used)
    vectorMapper->ScalarVisibilityOff();
    vectorMapper->SetInputConnection(glyphFilter->GetOutputPort());
    vectorMapper->Update();

    vtkSmartPointer<vtkActor> vectorActor{ vtkSmartPointer<vtkActor>::New() };

    vectorActor->SetMapper(vectorMapper);
    vtkNew<vtkNamedColors> colors;
    auto constexpr glyphColor{ "white_smoke" };
    vectorActor->GetProperty()->SetColor(colors->GetColor3d(glyphColor).GetData());

    // setting up the "rest" of the scene

    // todo: what is the image slice used for? it creates a black background when being added to the renderer!
    // could the imageSlice be used to hold the actual "backgroundimage"?
    //m_Renderer_fG->AddViewProp(imageSlice);

    //todo: hack for letting the manually generated vectorfield point in the correct direction (indicating from where the light is comming for rendering the sphere
    vectorActor->SetScale(1,1,1);
    
    // only show lightsource overlay when needed
    if (ui.showLightsourceCB->isChecked())
    {
        m_Renderer_fG->AddViewProp(vectorActor);
    }
    
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
    auto origin_fG{ origin_bG };
    auto spacing_fG{ spacing_bG };
    auto extent_fG{ extent_bG };

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

void QtWidget_VtkSandbox::redrawRaytracePreview()
{
    QPixmap pixmapColorPreview(41, 41);//todo take the acutal size of the label
    pixmapColorPreview.fill(QColor("transparent"));

    QPainter painterColorPreview(&pixmapColorPreview);
    QBrush brushColorPreview{ QColor(ui.colorSliderR->value(), ui.colorSliderG->value(), ui.colorSliderB->value()) };



    painterColorPreview.setBrush(brushColorPreview);

    //todo take coordinates set from the GUI
    QPoint centerpointColorPreviewLabel{ 20, 20 };
    painterColorPreview.drawEllipse(centerpointColorPreviewLabel, 20, 20);
    ui.colorPreviewLabel->setPixmap(pixmapColorPreview);

    // draw the "big" preview of the shaded "sphere"
    QRadialGradient gr(ui.lightPosSliderX->value(), ui.lightPosSliderY->value(), 200);//todo: take position from the actual pos-sliders
    gr.setColorAt(0.0, QColor(ui.colorSliderR->value(), ui.colorSliderG->value(), ui.colorSliderB->value(), 0));
    gr.setColorAt(0.2, QColor(ui.colorSliderR->value(), ui.colorSliderG->value(), ui.colorSliderB->value(), 63));
    gr.setColorAt(0.9, QColor(ui.colorSliderR->value(), ui.colorSliderG->value(), ui.colorSliderB->value(), 191));
    gr.setColorAt(1.0, QColor(ui.colorSliderR->value(), ui.colorSliderG->value(), ui.colorSliderB->value(), 255));
    //gr.setColorAt(0.95, QColor(ui.colorSliderR->value(), ui.colorSliderG->value(), ui.colorSliderB->value(), 127));
    gr.setColorAt(1, QColor(0, 0, 0, 0));

    QPixmap pixmap(256, 256);
    pixmap.fill(QColor("transparent"));
    QPainter painter(&pixmap);
    //todo take color set from the GUI
    painter.setBrush(gr);
    //painter.setOpacity(0.5);
    //todo take coordinates set from the GUI
    QPoint centerpoint{ ui.spherePosSliderX->value(), ui.spherePosSliderY->value() };
    painter.drawEllipse(centerpoint, 128, 128);
    ui.painterLabel->setPixmap(pixmap);

    QPixmap pixmap_Ambient(256, 256);
    pixmap_Ambient.fill(QColor("transparent"));
    QPainter painter_Ambient(&pixmap_Ambient);
    //todo take color set from the GUI
    painter_Ambient.setBrush(brushColorPreview);
    painter_Ambient.setOpacity(0.3);
    //painter.setOpacity(0.5);
    //todo take coordinates set from the GUI
    painter_Ambient.drawEllipse(centerpoint, 128, 128);
    ui.painterLabel_2->setPixmap(pixmap_Ambient);
}