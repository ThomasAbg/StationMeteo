#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->Temperature->setValue(15);
    ui->Humidite->setValue(50);
    ui->Pression->setValue(75);
    QObject::connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(close()));
}
void close(){
    exit(EXIT_FAILURE);
}

MainWindow::~MainWindow()
{
    delete ui;
}
