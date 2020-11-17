#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QGraphicsScene;
class Controller;
class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    Controller * controller;
public:
    MainWindow(QWidget *parent = nullptr);
    void setScene(QGraphicsScene * scene);
    void setLabel(QString p1, QString p2);
    void setController(Controller * con);
    ~MainWindow();
public slots:
    void onWinnerFound(QString &winner);


private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
