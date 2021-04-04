#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gamewidget.h"
#include <QColor>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_AI_clicked();

    void on_pushButton_Remote_clicked();


private:
    Ui::MainWindow *ui;
    GameWidget* game;
};

#endif // MAINWINDOW_H
