#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "controller.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView->viewport()->installEventFilter(this);
    ui->winner->hide();
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

void MainWindow::setController(Controller * con)
{
    this->controller = con;
    connect(controller, SIGNAL(foundWinner(QString&)), this, SLOT(onWinnerFound(QString&)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onWinnerFound(QString &winner)
{
    ui->winner->setText(winner + " Wins");
    ui->winner->show();
}

