#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <game.h>
#include <QMouseEvent>
#include <QMessageBox>
#include <QMap>
#include <QPainter>
#include "QTcpSocket"
#include <QDebug>
#include <QHostAddress>

namespace Ui {
class GameWidget;
}

class GameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameWidget(QWidget *parent = nullptr);
    ~GameWidget();
    // 设置游戏模式
    void init_game(GameType m);
    void set_tcp();

protected:
    // 绘制棋盘和棋子
    void paintEvent(QPaintEvent* event) override;
    // 鼠标移动事件(获取鼠标实时位置以及最近的点的坐标)
    void mouseMoveEvent(QMouseEvent* event) override;
    // 鼠标释放事件(下棋)
    void mouseReleaseEvent(QMouseEvent* event) override;

signals:
    void return_to_main();

private:
    Ui::GameWidget *ui;
    Game* game;
    QPoint board_[kBoardSize][kBoardSize];
    // 鼠标行列
    int cursor_row_;
    int cursor_col_;
    // tcp_socket
    QTcpSocket* tcp_socket;
    // 网络对战先手
    bool first_action;
    bool can_action; // 能否动
private slots:
    void data_received(); // tcp收到数据
    void on_pushButton_Return_clicked();
};

#endif // GAMEWIDGET_H
