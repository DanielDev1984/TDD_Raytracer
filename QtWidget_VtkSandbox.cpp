#include "QtWidget_VtkSandbox.h"

#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QImage>

#include <vtkSphereSource.h>
#include <vtkArrowSource.h>
#include <vtkActor.h>
#include <vtkImageActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkJPEGReader.h>
#include <vtkImageCanvasSource2D.h>
#include <vtkImageData.h>
#include <vtkCamera.h>
#include <vtkPointData.h>
#include <vtkGlyph2D.h>
#include <vtkImageSliceMapper.h>
#include <vtkImageSlice.h>
#include <vtkNamedColors.h>
#include <vtkProperty.h>

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
    // setting up the background actor
    /////////////////////
    vtkSmartPointer<vtkJPEGReader> jpegReader{ vtkSmartPointer<vtkJPEGReader>::New() };
    //todo: output of the raytracer currently is ppm -> how to convert to jpeg? / make it readable from vtk?
    const auto fN{ "C:\\Users\\strai\\source\\TDD_raytracer\\TDD_Raytracer\\bgImage.jpg" };
    //todo: figure out how to use vtkOutpuWindow for managing the application output
    std::cout << "input filename: " << fN << "\n";
    //todo: would imageprovider be the correct "imagesource"?
    auto const fileReadSucces{ (jpegReader->CanReadFile(fN) > 0) ? "jpeg file could be read" : "jpeg file could not be read" };
    std::cout << fileReadSucces << "\n";

    jpegReader->SetFileName(fN);
    jpegReader->Update();
    auto imageData{ jpegReader->GetOutput() };
    vtkSmartPointer<vtkImageActor> bg_Image{ vtkSmartPointer<vtkImageActor>::New() };
    bg_Image->SetInputData(imageData);
    m_Renderer_bG->AddViewProp(bg_Image);
    m_Renderer_bG->ResetCamera();
    /////////////////////

    // arrow / arrow field
    /////////////////////
    vtkSmartPointer<vtkArrowSource> arrowSource{ vtkSmartPointer<vtkArrowSource>::New() };
    arrowSource->InvertOn();
    arrowSource->Update();
    /*vtkSmartPointer<vtkPolyDataMapper> arrowMapper{ vtkSmartPointer<vtkPolyDataMapper>::New() };
    arrowMapper->SetInputData(arrowSource->GetOutput());
    vtkSmartPointer<vtkActor> arrow{ vtkSmartPointer<vtkActor>::New() };
    arrow->SetMapper(arrowMapper);*/

    vtkSmartPointer<vtkImageData> imageDataVectorField{ vtkSmartPointer<vtkImageData>::New() };
    //imageDataVectorField->SetDimensions(imageData->GetDimensions());
    //imageDataVectorField->SetDimensions(481,321,1);
    // the aspect ratio of the openGWindow
    auto const aspectRatio{ 418.0 / 321.0 };
    //todo: what do these vecFieldDimensions actually mean wrt the background image
    //->arrows get smaller when vecFieldDims increase
    const double vectFieldDim_X{ 40.0 }; 
    //imageDataVectorField->SetDimensions(vectFieldDim_X, vectFieldDim_X / aspectRatio, 1);
    const auto imageDimensions{ imageData->GetDimensions() };
    std::cout << "imageDimensionsX: " << imageDimensions[0] << "\n";
    std::cout << "imageDimensionsY: " << imageDimensions[1] << "\n";
    std::cout << "imageDimensionsZ: " << imageDimensions[2] << "\n";
    imageDataVectorField->SetDimensions(imageDimensions);
    imageDataVectorField->AllocateScalars(VTK_FLOAT, 3);

    const auto vectorFieldDimensions{ imageDataVectorField->GetDimensions() };
    constexpr auto samplingRateX{ 20 };
    constexpr auto samplingRateY{ 20 };
    constexpr auto arrowLengthScalingFactor{ 25 };
    // fill the vectorfield
    for (auto z = 0; z < vectorFieldDimensions[2]; ++z)
    {
        for (auto y = 0; y < vectorFieldDimensions[1]; ++y)
        {
            for (auto x = 0; x < vectorFieldDimensions[0]; ++x)
            {
                //todo: instead of doing this manually, filter the (dense) vectorfield lateron
                if (((x % samplingRateX == 0) && (y % samplingRateY == 0)))
                {
                    auto pixel = static_cast<float*>(imageDataVectorField->GetScalarPointer(x, y, z));
                    //todo sth is not yet correct wrt to calculating the vector to/from the lightsource
                    double xcomponent = x - 512.0;
                    double yComponent = y + 256;
                    //todo: do these vectors (in the final vectorfield) really have to be normalized "manually" at this point?
                    double vectLenght{ std::sqrt(xcomponent * xcomponent + yComponent * yComponent) };
                    pixel[0] = xcomponent / vectLenght * arrowLengthScalingFactor;
                    pixel[1] = yComponent / vectLenght * arrowLengthScalingFactor;
                    pixel[2] = 0;//todo: dont forget to take z component into account. somehow this seems to be ignored for the rendering of the arrows atm :(
                    
                }
                else
                {
                    auto pixel = static_cast<float*>(imageDataVectorField->GetScalarPointer(x, y, z));
                    pixel[0] = 0.0;
                    pixel[1] = 0.0;
                    pixel[2] = 0.0;
                }
            }
        }
    }


    //todo: what does this do?
    imageDataVectorField->GetPointData()->SetActiveVectors(imageDataVectorField->GetPointData()->GetScalars()->GetName()); 
    vtkSmartPointer<vtkGlyph2D> glyphFilter{ vtkSmartPointer<vtkGlyph2D>::New() };
    glyphFilter->SetSourceConnection(arrowSource->GetOutputPort());
    glyphFilter->OrientOn();
    //glyphFilter->SetVectorModeToUseVector();
    glyphFilter->SetInputData(imageDataVectorField);
    glyphFilter->Update();

    vtkSmartPointer<vtkImageSliceMapper> imageMapper{ vtkSmartPointer<vtkImageSliceMapper>::New() };
    imageMapper->SetInputData(imageDataVectorField);

    vtkSmartPointer<vtkImageSlice> imageSlice{ vtkSmartPointer<vtkImageSlice>::New() };
    imageSlice->SetMapper(imageMapper);

    vtkSmartPointer<vtkPolyDataMapper> vectorMapper{ vtkSmartPointer<vtkPolyDataMapper>::New() };
    vectorMapper->ScalarVisibilityOff();
    vectorMapper->SetInputConnection(glyphFilter->GetOutputPort());

    vtkSmartPointer<vtkActor> vectorActor{ vtkSmartPointer<vtkActor>::New() };
    vtkNew<vtkNamedColors> colors;
   
    vectorActor->SetMapper(vectorMapper);
    //todo: why is it not possible to color the glyphs?
    auto constexpr glyphColor{ "White" };//"LimeGreen"
    vectorActor->GetProperty()->SetColor(
        colors->GetColor3d(glyphColor).GetData());
    /////////////////////

    // setting up the "rest" of the scene

    //m_Renderer_fG->AddViewProp(arrow);
    // todo: what is the image slice used for? it creates a black background when being added to the renderer!
    // could the imageSlice be used to hold the actual "backgroundimage"?
    //m_Renderer_fG->AddViewProp(imageSlice);
    m_Renderer_fG->AddViewProp(vectorActor);
    m_Renderer_fG->ResetCamera();
    m_RenderWindow->Render();


    auto origin_bG{ imageData->GetOrigin() };
    auto spacing_bG{ imageData->GetSpacing() };
    auto extent_bG{ imageData->GetExtent() };

    //auto camera{ m_Renderer_bG->GetActiveCamera() };
    auto camera_bG{ m_Renderer_bG->GetActiveCamera() };
    camera_bG->ParallelProjectionOn();

    auto xc_bG{ origin_bG[0] + 0.5 * (extent_bG[0] + extent_bG[1]) * spacing_bG[0] };
    auto yc_bG{ origin_bG[1] + 0.5 * (extent_bG[2] + extent_bG[3]) * spacing_bG[1] };

    auto yd_bG{ (extent_bG[3] - extent_bG[2] + 1) * spacing_bG[1] };
    auto d_bG{ camera_bG->GetDistance() };
    //this sets the background camera to cover all of the available openglwindow(space)
    camera_bG->SetParallelScale(0.5 * yd_bG);
    camera_bG->SetFocalPoint(xc_bG, yc_bG, 0.0);
    camera_bG->SetPosition(xc_bG, yc_bG, d_bG);


    // setup bg camera to fill the renderer with the image
    auto origin_fG{ imageDataVectorField->GetOrigin() };
    auto spacing_fG{ imageDataVectorField->GetSpacing() };
    auto extent_fG{ imageDataVectorField->GetExtent() };

    //auto camera{ m_Renderer_bG->GetActiveCamera() };
    auto camera_fG{ m_Renderer_fG->GetActiveCamera() };
    camera_fG->ParallelProjectionOn();

    auto xc_fG{ origin_fG[0] + 0.5 * (extent_fG[0] + extent_fG[1]) * spacing_fG[0] };
    auto yc_fG{ origin_fG[1] + 0.5 * (extent_fG[2] + extent_fG[3]) * spacing_fG[1] };

    auto yd_fG{ (extent_fG[3] - extent_fG[2] + 1) * spacing_fG[1] };
    auto d_fG{ camera_fG->GetDistance() };
    //this sets the background camera to cover all of the available openglwindow(space)
    camera_fG->SetParallelScale(0.5 * yd_fG);
    camera_fG->SetFocalPoint(xc_fG, yc_fG, 0.0);
    camera_fG->SetPosition(xc_fG, yc_fG, d_fG);

    
    m_RenderWindow->Render();


    //todo: would(nt) it be better to have this as a static function? -> think of a nicer way to actually trigger the rendering
    TDD_Raytracer raytracer{};
    //todo: use the values from the UI instead of hardcoded values!
    constexpr float light_x{ 512.0 }, light_y{ 256.0 }, light_z{ -64.0 };
    raytracer.drawSphereWithPhongShading(light_x, light_y, light_z);
}
