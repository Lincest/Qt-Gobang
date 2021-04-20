#ifndef GAME_H
#define GAME_H
#include<vector>
#include<map>
#include<algorithm>
#include<stack>

// 五子棋游戏模型

// 棋盘大小
const int kBoardSize = 15;

// 两种模式
enum GameType {
    kAIBlack, kOnline, kAIWhite
};

// 游戏状态
enum GameStatus {
    kWhiteFive = 0,         kBlackFive = 1,
    kWhiteFour = 2,         kBlackFour = 3, // 活四
    kWhiteThree = 4,        kBlackThree = 5, // 活三
    kWhiteTwo = 6,          kBlackTwo = 7, // 活二
    kWhiteBlockFour = 8,    kBlackBlockFour = 9, // 冲四
    kWhiteBlockThree = 10,  kBlackBlockThree = 11, // 眠三
    kWhiteBlockTwo = 12,    kBlackBlockTwo = 13, // 眠二
    kWhiteOne = 14,         kBlackOne = 15 // 活一
};

// 游戏结果
enum GameResult {
    kDeadGame, kWhiteWin, kBlackWin, kPlaying, kNotStart
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

// 生成ai的最佳落子位置
typedef struct best_step {
    int score; // 该步数的评分
    std::pair<int, int> pos; // 位置
    best_step() {
        pos = {0, 0};
        score = -INT_MAX;
    }
} best_step;

class Game {
public:
    // 棋盘
    ChessPieces game_map_[kBoardSize][kBoardSize];
    //棋型辨识数组,0无子,1黑子,2白子,3边界
    // (https://blog.csdn.net/livingsu/article/details/104539741)
    GameStatus tuple_six_[4][4][4][4][4][4];
    // 棋形对应评分
    std::map<GameStatus, int> score_;
    // 下棋方
    bool flag_;
    // 模式
    GameType game_type_;
    // ai下棋位置(MinMax搜索得到)
    best_step step_;
    // 保存历史记录, 用于悔棋
    std::stack<std::pair<int, int>> history_person_, history_ai_;
    // 搜索最大深度
    int maxDepth = 4;

public:
    Game();
    // 游戏开始
    void start_game(GameType type);
    // 六元组初始化
    void init_tuple_six();
    // 下棋
    void person_action(int row, int col);
    void ai_action();
    // 更新棋盘
    void update_map(int row, int col);
    // 扫描棋盘判断局势(重载)
    AllScore evaluate(ChessPieces game_map_[][kBoardSize]);
    AllScore evaluate();
    // MinMax搜索 + alphabeta剪枝计算评分
    int alphabeta(int depth, int alpha, int beta, ChessPieces role);
    // 生成估值最大的十个点, 减少MinMax搜索的量(只用试最大的十个点, 而不用尝试所有点)
    std::vector<std::vector<int>> generate_points(ChessPieces board[][kBoardSize], ChessPieces chess_type);
    // 将棋盘反色
    void reverse_map();
    // 悔棋
    void do_back();
    // 随即开局
    void do_random_start();
};

#endif // GAME_H
