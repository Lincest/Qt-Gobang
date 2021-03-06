#include "gamewidget.h"
#include "ui_gamewidget.h"
#include "iostream"
using namespace std;

GameWidget::GameWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameWidget)
{
    ui->setupUi(this);
}

GameWidget::~GameWidget()
{
    delete ui;
    delete tcp_socket;
}

void GameWidget::on_pushButton_Return_clicked() {
    can_action = false;
    emit return_to_main();
    this->close();
    if (tcp_socket) {
        tcp_socket->close();
    }
}

void GameWidget::create_game() {
    game = new Game();
}

void GameWidget::init_game(GameType m) {
    game->start_game(m);
    // 棋盘左上角坐标为(30, 30)
    // 棋盘每一个正方形格子边长为30
    for (int i = 0; i < kBoardSize; ++i) {
        for (int j = 0; j < kBoardSize; ++j) {
            board_[i][j].setX(30 + 30 * i);
            board_[i][j].setY(30 + 30 * j);
        }
    }
    can_action = false; // 按鼠标不反应
    setMouseTracking(false); // 不追踪鼠标
    // 如果是网络模式
    if (m == kOnline) {
        first_action = false;
        ui->label_connection->setText("not connected");
        // 隐藏按钮
        ui->check_ai_first->hide();
        ui->comboBox_Depth->hide();
        ui->label_3->hide();
        ui->label_4->hide();
        // 显示按钮
        ui->label_connection->show();
        ui->pushButton_Start->show();
        ui->label->show();
        qDebug() << "远程联机初始化完成..." << endl;
    } else if (m == kAIWhite) {
        setMouseTracking(true);
        can_action = true;
        first_action = true;
        // 隐藏按钮
        ui->label_connection->hide();
        ui->label->hide();
        qDebug() << "AI对战初始化完成..." << endl;
        update();
    } else if (m == kAIBlack) {
        // 隐藏按钮
        ui->label_connection->hide();
        ui->label->hide();
        game->do_random_start();
        qDebug() << "AI对战初始化完成..." << endl;
        setMouseTracking(true);
        can_action = true;
        first_action = true;
        update();
    }
    repaint();
}

void GameWidget::on_pushButton_Start_clicked() {
    if (game->game_type_ == kOnline) {
        qDebug() << tcp_socket << " " << endl;
        if (tcp_socket && tcp_socket->state() != 0) {
            qDebug() << tcp_socket->state() << endl;
            return;
        }
        if (tcp_socket == nullptr)
            tcp_socket = new QTcpSocket(this);
        set_tcp();
    } else {
        // 处理多选框
        if (check_state() != kNotStart) return;
        game->maxDepth = ui->comboBox_Depth->currentText().at(0).toLatin1() - '0';
        qDebug() << game->maxDepth << endl;
        if (ui->check_ai_first->isChecked()) {
            this->init_game(kAIBlack);
        } else {
            this->init_game(kAIWhite);
        }
    }
}

// TODO: 指定服务器和端口, 显示连接信息
void GameWidget::set_tcp() {
    // 连接服务器 TODO: 不写死ip和端口
    quint16 port = 6667;
    QString ip = "121.199.73.210";
    QHostAddress* server_ip = new QHostAddress();
    server_ip->setAddress(ip);
    tcp_socket->connectToHost(*server_ip, port);
    // 绑定信号槽,当有数据发回来时调用data_received
    connect(tcp_socket, SIGNAL(readyRead()), this, SLOT(data_received()));
    connect(tcp_socket, &QAbstractSocket::connected, this, [&](){ui->label_connection->setText("Ready (waiting for anthor)...");});
//    connect(tcp_socket, &QTcpSocket::disconnected, this, [&](){this->end_game_with_box("TCP error");});
    connect(tcp_socket, &QTcpSocket::disconnected, this, &GameWidget::on_pushButton_Return_clicked);
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
        QMessageBox::information(nullptr, "Game Start", "Game Start!");
        if (first_action == true) {
            ui->label_connection->setText("You are Black");
            setMouseTracking(true);
            can_action = true;
            return;
        }
        ui->label_connection->setText("You are White");
        return;
    }
    // 游戏中有一方退出, 则游戏结束
    if (msg.contains("end")) {
        if (check_state() != kWhiteWin && check_state() != kBlackWin)
            end_game_with_box("The other client aborted!");
    }
    if (msg != "") {
        int row = msg.section(',', 0, 0).toInt(), col = msg.section(',', 1, 1).toInt();
        setMouseTracking(false);
        if (row != -1 && col != -1 && game->game_map_[row][col] == kEmpty) {
            game->person_action(row, col);
            check_state();
            setMouseTracking(true);
            can_action = true;
        }
        repaint();
    }
}

void GameWidget::end_game_with_box(QString win_text) {
    // MessageBox: https://doc.qt.io/qt-5/qmessagebox.html#StandardButton-enum
    QMessageBox msg;
    msg.setWindowTitle("Game Ends");
    msg.setText(win_text);
    msg.setStandardButtons(QMessageBox::Ok);
    // TODO: 输赢计数
    // 重启游戏
    if (msg.exec() == QMessageBox::Ok) {
        if (game->game_type_ == kOnline) {
            tcp_socket->close();
        }
        // TODO: 继续保持在当前界面, 开始下一局
        this->on_pushButton_Return_clicked();
//        this->init_game(kOnline);
    }
}

// 绘制棋盘和棋子
void GameWidget::paintEvent(QPaintEvent *event) {
//    qDebug() << "绘制棋盘..." << endl;
    QPainter painter(this);
    // 开启反走样(抗锯齿)
    painter.setRenderHint(QPainter::Antialiasing);
    // 画棋盘
    painter.setPen(Qt::black);
    for (int i = 0; i < kBoardSize; ++i) {
        painter.drawLine(this->board_[0][i], this->board_[kBoardSize - 1][i]);
        painter.drawLine(this->board_[i][0], this->board_[i][kBoardSize - 1]);
        painter.setBrush(Qt::black);
        painter.drawEllipse(this->board_[kBoardSize / 2][kBoardSize / 2].x() - 3, this->board_[kBoardSize / 2][kBoardSize / 2].y() - 3, 6, 6);
        int tmp_dis = kBoardSize / 4 + 1;
        vector<int> dis{tmp_dis, tmp_dis, -tmp_dis, -tmp_dis, tmp_dis};
        for (int k = 0; k < 4; ++k) {
            painter.drawEllipse(this->board_[kBoardSize / 2 + dis[k]][kBoardSize / 2 + dis[k + 1]].x() - 3, this->board_[kBoardSize / 2 + dis[k]][kBoardSize / 2 + dis[k + 1]].y() - 3, 6, 6);
        }
    }
    // 画坐标
    QFont font1("consolas", 10, QFont::Bold, true);
    painter.setFont(font1);
    for (int i = 0; i <= 14; ++i) {
        painter.drawText(this->board_[0][i].x() - 20, this->board_[0][i].y() + 4, QString('A' + i));
        painter.drawText(this->board_[i][0].x() - 4, this->board_[i][0].y() - 10, QString(QString::number(i + 1)));
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
}

// 跟踪鼠标, 获取下棋位置
void GameWidget::mouseMoveEvent(QMouseEvent *event) {
    // 获取鼠标的x和y坐标
    float x = event->x(), y = event->y();
    // 判断在边界内部
    if (x >= (20 - kBoardSize) && x <= (20 + kBoardSize * 30) && y >= (20 - kBoardSize) && y <= (20 + kBoardSize * 30)) {
        for (int i = 0; i < kBoardSize; ++i) {
            for (int j = 0; j < kBoardSize; ++j) {
                float bx = board_[i][j].x(), by = board_[i][j].y();
                if ((x >= bx - 15) && (x < bx + 15) && (y >= by - 15) && (y < by + 15)) {
                    cursor_row_ = j;
                    cursor_col_ = i;
                    // 显示坐标
                    QString cursor_str = QString('A' + cursor_row_) + ' ' + QString::number(cursor_col_ + 1);
                    ui->label_pos->setText(cursor_str);
                    break;
                }
            }
        }
    }
    update();
}

// 鼠标点击, 下棋
void GameWidget::mousePressEvent(QMouseEvent *event) {
    // 判断是否能下棋(棋方为自己并且落点为空)
    if(can_action && game->game_map_[cursor_row_][cursor_col_] == kEmpty) {
        can_action = false;
        do_action();
    }
}


void GameWidget::do_action() {
    // 强制事件循环, 解决鼠标点击多次事件阻塞的问题 ->   https://blog.csdn.net/simonforfuture/article/details/78977426
    QCoreApplication::processEvents();
    if (cursor_row_ != -1 && cursor_col_ != -1) {
        if (game->game_type_ == kOnline) {
            game->person_action(cursor_row_, cursor_col_);
            // 发送位置给Tcp服务器
            QString msg = QString::number(cursor_row_, 10) + ", " + QString::number(cursor_col_, 10);
            tcp_socket->write(msg.toUtf8(), msg.length());
            tcp_socket->flush();
            setMouseTracking(false);
            repaint();
            check_state();
        } else if (game->game_type_ == kAIWhite || game->game_type_ == kAIBlack){
            game->person_action(cursor_row_, cursor_col_);
            repaint();
            if (check_state() == kPlaying) {
                game->ai_action();
                repaint();
                check_state();
                can_action = true; // 电脑完才能动
            }
        }
    }
}

// 判断输赢, 如果出现输赢或者死局重启游戏
GameResult GameWidget::check_state() {
    GameResult status = game->evaluate().result;
    if (status == kDeadGame || status == kWhiteWin || status == kBlackWin) {
        QString win_str;
        qDebug() << status << endl;
        if (status == kBlackWin) win_str = "Black Win!";
        else if (status == kWhiteWin) win_str = "White Win!";
        else if (status == kDeadGame) win_str = "Dead Game!";
        end_game_with_box(win_str);
    }
    return status;
}

// 悔棋
void GameWidget::on_pushButton_Return_Back_clicked() {
    if (game->game_type_ == kAIWhite || game->game_type_ == kAIBlack) {
        // 只有电脑下完了才能悔棋
        if (can_action) {
            game->do_back();
            repaint();
        }
    }
}

