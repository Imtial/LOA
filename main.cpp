#include "mainwindow.h"

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QVBoxLayout>
#include "board.h"
#include "human.h"
#include "ai.h"
#include "controller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QGraphicsScene * scene = new QGraphicsScene();
    w.setScene(scene);
    Player * p1 = new Human(W);
    Player * p2 = new AI(B);
    w.setLabel(p1->getType(), p2->getType());
    Controller * controller = new Controller(p1, p2);
    Board board = Board(*scene, controller);
    p1->setBoard(&board);
    p2->setBoard(&board);
//    QGraphicsView view(scene);
//    view.setRenderHints(QPainter::Antialiasing);
    w.show();

    if (p1->getType() == _AI_) controller->start();
//    view.show();
    return a.exec();
}
