#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    game = new GameWidget();
    game->hide();
    // 子窗口点击返回键时返回父窗口
    // https://blog.csdn.net/weixin_48424192/article/details/109195369?utm_medium=distribute.pc_relevant.none-task-blog-baidujs_baidulandingword-4&spm=1001.2101.3001.4242
    connect(game, &GameWidget::return_to_main, this, &MainWindow::show);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete game;
}

// AI模式
void MainWindow::on_pushButton_AI_clicked()
{
    this->hide();
    game->show();
    game->init_game(kAI);
}


//远程模式
void MainWindow::on_pushButton_Remote_clicked()
{
    this->hide();
    game->show();
    game->init_game(kOnline);
}
