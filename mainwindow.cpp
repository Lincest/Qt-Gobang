#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 子窗口点击返回键时返回父窗口
    // https://blog.csdn.net/weixin_48424192/article/details/109195369?utm_medium=distribute.pc_relevant.none-task-blog-baidujs_baidulandingword-4&spm=1001.2101.3001.4242
}

MainWindow::~MainWindow()
{
    delete ui;
    delete game;
}

// AI模式
void MainWindow::on_pushButton_AI_clicked()
{
    game = new GameWidget();
    game->setStyleSheet("background-color: rgb(236, 188, 29);");
    connect(game, &GameWidget::return_to_main, this, &MainWindow::show);
    connect(game, &GameWidget::return_to_main, game, &GameWidget::deleteLater);
    this->hide();
    game->show();
    game->create_game();
}


//远程模式
void MainWindow::on_pushButton_Remote_clicked()
{
    game = new GameWidget();
    game->setStyleSheet("background-color: rgb(236, 188, 29);");
    connect(game, &GameWidget::return_to_main, this, &MainWindow::show);
    connect(game, &GameWidget::return_to_main, game, &GameWidget::deleteLater);
    this->hide();
    game->show();
    game->create_game();
    game->init_game(kOnline);
}
