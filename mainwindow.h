#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "canvas.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class CAppOption;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public :
     MainWindow( QWidget *parent = nullptr );
    ~MainWindow();

protected :
    void setup_ui();
    void init_ui( CAppOption *pAppOption );
    void delete_ui();

private slots :
     void on_Save();
     void on_Load();
     void on_Exit();

     void on_Rectangle();
     void on_Ellipse();
     void on_Triangle();

     void on_Relation();
     void on_Delete();
     void on_Move();

 private :
    Ui::MainWindow *ui;

    QAction   *actionSave
            , *actionLoad
            , *actionExit;

    CCanvas   *m_pCanvas;
};
#endif // MAINWINDOW_H
