#include "gamewidget.h"
#include "ui_gamewidget.h"

GameWidget::GameWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameWidget)
{
    ui->setupUi(this);
    game = new Game();
    // 棋盘左上角坐标为(20, 20)
    // 棋盘每一个正方形格子边长为30
    for (int i = 0; i < kBoardSize; ++i) {
        for (int j = 0; j < kBoardSize; ++j) {
            board_[i][j].setX(20 + 30 * i);
            board_[i][j].setY(20 + 30 * j);
        }
    }
    setMouseTracking(true);
}

GameWidget::~GameWidget()
{
    delete ui;
    delete tcp_socket;
}

void GameWidget::on_pushButton_Return_clicked() {
    this->close();
    emit return_to_main();
}

void GameWidget::init_game(GameType m) {
    game->start_game(m);
    if (m == kOnline) {
        set_tcp();
        first_action = false;
        setMouseTracking(false); // 不追踪鼠标
        can_action = false; // 按鼠标不反应
    }
}

// TODO: 指定服务器和端口, 显示连接信息
void GameWidget::set_tcp() {
    tcp_socket = new QTcpSocket(this);
    // 连接服务器
    quint16 port = 6667;
    QString ip = "127.0.0.1";
    QHostAddress* server_ip = new QHostAddress();
    server_ip->setAddress(ip);
    tcp_socket->connectToHost(*server_ip, port);
    // 绑定信号槽,当有数据发回来时调用data_received
    connect(tcp_socket, SIGNAL(readyRead()), this, SLOT(data_received()));
}

void GameWidget::data_received() {
    // 读buffer
    QByteArray buffer;
    // 返回可用数据大小
    buffer.resize(tcp_socket->bytesAvailable());
    tcp_socket->read(buffer.data(), buffer.size());
    QString msg = buffer.data();
    qDebug() << "recv: " << msg << endl;
    // 第一个开始的玩家标记为first_head
    if (msg.contains("start")) {
        first_action = true;
        return;
    }
    // 两个客户端都连上了
    if (msg.contains("ready")) {
        if (first_action == true) {
            setMouseTracking(true);
            can_action = true;
        }
        return;
    }
    int row = msg.section(',', 0, 0).toInt(), col = msg.section(',', 1, 1).toInt();
    setMouseTracking(false);
    if (row != -1 && col != -1 && game->game_map_[row][col] == kEmpty) {
        game->person_action(row, col);
        setMouseTracking(true);
        can_action = true;
    }
    update();
}

// 绘制棋盘和棋子
void GameWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    // 开启反走样(抗锯齿)
    painter.setRenderHint(QPainter::Antialiasing);
    // 画棋盘
    painter.setPen(Qt::black);
    for (int i = 0; i < kBoardSize; ++i) {
        painter.drawLine(this->board_[0][i], this->board_[kBoardSize - 1][i]);
        painter.drawLine(this->board_[i][0], this->board_[i][kBoardSize - 1]);
    }
    // 先/后手着黑色/白色
    game->flag_ ? painter.setBrush(Qt::black) : painter.setBrush(Qt::white);
    // 画光标
    if (can_action && cursor_col_ != -1 && cursor_row_ != -1) {
        painter.drawRect(board_[cursor_col_][cursor_row_].x() - 4, board_[cursor_col_][cursor_row_].y() - 4, 8, 8);
    }
    // 画棋子
    for (int i = 0; i < kBoardSize; ++i) {
        for (int j = 0; j < kBoardSize; ++j) {
            if (game->game_map_[i][j] != kEmpty) {
                game->game_map_[i][j] == kBlack ? painter.setBrush(Qt::black) : painter.setBrush(Qt::white);
                painter.drawEllipse(board_[j][i].x()-10, board_[j][i].y()-10, 20, 20);
            }
        }
    }
    // 判断输赢, 如果出现输赢或者死局重启游戏
    int status = game->evaluate();
    if (status != NO_WIN) {
        qDebug() << "game ends, should restart..." << endl;
        QString win_str;
        if (status == BLACK_WIN) win_str = "Black Win!";
        else if (status == WHITE_WIN) win_str = "White Win!";
        else if (status == DEAD_GAME) win_str = "Dead Game!";
        // MessageBox逻辑: https://doc.qt.io/qt-5/qmessagebox.html#StandardButton-enum
        QMessageBox msg;
        msg.setWindowTitle("Game Ends");
        msg.setText(win_str);
        msg.setStandardButtons(QMessageBox::Ok);
        // TODO: 输赢计数
        // 重启游戏
        if (msg.exec() == QMessageBox::Ok) {
            if (game->game_type_ == kOnline) {
                // 发送结束标志
                QString msg = "end";
                tcp_socket->write(msg.toUtf8(), msg.length());
                tcp_socket->close();
            }
            // 回到主界面
            // TODO: 继续保持在当前界面, 开始下一局
            this->close();
            emit return_to_main();
        }
    }
}

// 跟踪鼠标, 绘制鼠标小方格
void GameWidget::mouseMoveEvent(QMouseEvent *event) {
    // 获取鼠标的x和y坐标
    float x = event->x(), y = event->y();
    // 判断在边界内部
    if (x >= (20 - kBoardSize) && x <= (20 + kBoardSize * 30) && y >= (20 - kBoardSize) && y <= (20 + kBoardSize * 30)) {
        for (int i = 0; i < kBoardSize; ++i) {
            for (int j = 0; j < kBoardSize; ++j) {
                float bx = board_[i][j].x(), by = board_[i][j].y();
                // qDebug() << (x - bx) << ' ' << (y - by) << endl;
                if ((x >= bx - 15) && (x < bx + 15) && (y >= by - 15) && (y < by + 15)) {
                    cursor_row_ = j;
                    cursor_col_ = i;
                    // 显示坐标
                    QString cursor_str = "position: " + QString::number(cursor_row_) + ", " + QString::number(cursor_col_);
                    qDebug() << cursor_str << endl;
                    // TODO: 展示坐标
                    break;
                }
            }
        }
    }
    update();
}

// 鼠标释放, 下棋
void GameWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (can_action) {
        if (game->game_type_ == kOnline && game->game_map_[cursor_row_][cursor_col_] == kEmpty) {
            game->person_action(cursor_row_, cursor_col_);
            // 发送位置给Tcp服务器
            QString msg = QString::number(cursor_row_, 10) + ", " + QString::number(cursor_col_, 10);
            tcp_socket->write(msg.toUtf8(), msg.length());
            tcp_socket->flush();
            setMouseTracking(false);
            can_action = false;
        } else {
            game->ai_action();
        }
        update();
    }
}
