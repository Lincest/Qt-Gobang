#ifndef GAME_H
#define GAME_H
#include<vector>
#include<map>

// 五子棋游戏模型

// 棋盘大小
const int kBoardSize = 15;

// 两种模式
enum GameType {
    kAI, kOnline
};

// 游戏状态
enum GameStatus {
    kWhiteFive = 0, kBlackFive = 1,
    kWhiteFour = 2, kBlackFour = 3, // 活四
    kWhiteThree = 4, kBlackThree = 5, // 活三
    kWhiteTwo = 6, kBlackTwo = 7, // 活二
    kWhiteBlockFour = 8, kBlackBlockFour = 9, // 冲四
    kWhiteBlockThree = 10, kBlackBlockThree = 11, // 眠三
    kWhiteBlockTwo = 12, kBlackBlockTwo = 13, // 眠二
    kWhiteOne = 14, kBlackOne = 15 // 活一
};

enum GameResult {
    kDeadGame, kWhiteWin, kBlackWin, kPlaying
};

// 棋子颜色
enum ChessPieces {
    kEmpty = 0, kBlack = 1, kWhite = 2
};

typedef struct AllScore {
    int score; // 局面总分
    std::map<GameStatus, int> stat; // stat[GameStatus] = 该status数量
    GameResult result; // 是否结束游戏
    AllScore() {
        score = 0; result = kPlaying;
    }
} AllScore;

class Game {
public:
    // 棋盘
    ChessPieces game_map_[kBoardSize][kBoardSize];
    // (ai)棋局评分
    int score_map_[kBoardSize][kBoardSize];
    //棋型辨识数组,0无子,1黑子,2白子,3边界
    // (https://blog.csdn.net/livingsu/article/details/104539741)
    GameStatus tuple_six_[4][4][4][4][4][4];
    // 棋形对应评分
    std::map<GameStatus, int> score_;
    // 下棋方
    bool flag_;
    // 模式
    GameType game_type_;

public:
    Game();
    // 游戏开始
    void start_game(GameType type);
    // 六元组初始化
    void init_tuple_six();
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
    AllScore evaluate();

};

#endif // GAME_H
