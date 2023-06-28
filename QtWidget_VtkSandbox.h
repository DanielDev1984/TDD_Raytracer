#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtWidget_VtkSandbox.h"

#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>
#include <QVTKInteractor.h>
#include <vtkInteractorStyle.h>

class QtWidget_VtkSandbox : public QMainWindow
{
    Q_OBJECT

public:
    QtWidget_VtkSandbox(QWidget *parent = nullptr);
    ~QtWidget_VtkSandbox();

    QLabel* label;

public slots:
    void onRenderButtonClicked();
    void onSpherPosSliderYChanged(int valY);
    void onSpherPosSliderXChanged(int valX);
    void onSpherColorChanged(int val);
    void onLightPosXChanged();
private:
    Ui::QtWidget_VtkSandboxClass ui;

    vtkSmartPointer<vtkGenericOpenGLRenderWindow> m_RenderWindow;
    vtkSmartPointer<vtkRenderer> m_Renderer_fG;
    vtkSmartPointer<vtkRenderer> m_Renderer_bG;
    vtkSmartPointer<QVTKInteractor> m_Interactor;
    vtkSmartPointer<vtkInteractorStyle> m_InteractorStyle;
};
