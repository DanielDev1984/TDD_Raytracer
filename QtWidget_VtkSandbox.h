#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtWidget_VtkSandbox.h"

class QtWidget_VtkSandbox : public QMainWindow
{
    Q_OBJECT

public:
    QtWidget_VtkSandbox(QWidget *parent = nullptr);
    ~QtWidget_VtkSandbox();

    QLabel* label;
private:
    Ui::QtWidget_VtkSandboxClass ui;
};
