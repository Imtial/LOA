#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView->viewport()->installEventFilter(this);
}

void MainWindow::setScene(QGraphicsScene * scene)
{
    ui->graphicsView->setRenderHints(QPainter::Antialiasing);
    ui->graphicsView->setScene(scene);
}

void MainWindow::setLabel(QString p1, QString p2)
{
    ui->player1->setText(p1);
    ui->player2->setText(p2);
}

MainWindow::~MainWindow()
{
    delete ui;
}

