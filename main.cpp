#include "mainwindow.h"

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "board.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
    QGraphicsScene scene;
    Board board = Board(scene);
    QGraphicsView view(&scene);
    view.setRenderHints(QPainter::Antialiasing);
    view.show();
    return a.exec();
}
