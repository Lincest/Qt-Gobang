#ifndef GAME_H
#define GAME_H

#define WHITE_WIN 1
#define BLACK_WIN 2
#define NO_WIN 3
#define DEAD_GAME 4
#include<vector>

// 五子棋游戏模型

// 棋盘大小
const int kBoardSize = 15;

// 两种模式
enum GameType {
    kAI, kOnline
};

// 游戏状态
enum GameStatus {
    kPlaying, kWin, kDead
};

// 棋子颜色
enum ChessPieces {
    kEmpty = 0, kBlack = 1, kWhite = 2
};

class Game {
public:
    // 棋盘
    ChessPieces game_map_[kBoardSize][kBoardSize];
    // (ai)棋局评分
    int score_map_[kBoardSize][kBoardSize];
    //棋型辨识数组,0无子,1黑子,2白子,3边界
    // (https://blog.csdn.net/livingsu/article/details/104539741)
    int tuple6type[4][4][4][4][4][4];
    // 下棋方
    bool flag_;
    // 模式
    GameType game_type_;
    // 状态
    GameStatus game_status_;

public:
    Game();
    // 游戏开始
    void start_game(GameType type);
    // 六元组初始化
    void init_tuple6type();
    // 计算评分
    void cal_score();
    // 找到最高的评分点
    std::pair<int, int> find_the_highest_point();
    // 下棋
    void person_action(int row, int col);
    void ai_action();
    // 更新棋盘
    void update_map(int row, int col);
    // 扫描棋盘判断局势
    int evaluate();

};

#endif // GAME_H
