#include "game.h"
#include <stdlib.h>
#include <time.h>
#include <utility>
#include <memory.h>


Game::Game(){}

// 初始化
void Game::start_game(GameType type) {
    game_type_ = type;
    // 初始化棋盘
    memset(game_map_, 0, sizeof(game_map_));

    // 初始化评分
    if (game_type_ == kAI) {
        memset(score_map_, 0, sizeof(score_map_));
    }
    // 初始化六元组
    init_tuple6type();

    // 先手
    flag_ = true;
}

void Game::init_tuple6type() {
    memset(tuple6type,0,sizeof (tuple6type));
    //白连5,ai赢
    tuple6type[2][2][2][2][2][2]=WHITE_WIN;
    tuple6type[2][2][2][2][2][0]=WHITE_WIN;
    tuple6type[0][2][2][2][2][2]=WHITE_WIN;
    tuple6type[2][2][2][2][2][1]=WHITE_WIN;
    tuple6type[1][2][2][2][2][2]=WHITE_WIN;
    tuple6type[3][2][2][2][2][2]=WHITE_WIN;
    tuple6type[2][2][2][2][2][3]=WHITE_WIN;
    //黑连5,ai输
    tuple6type[1][1][1][1][1][1]=BLACK_WIN;
    tuple6type[1][1][1][1][1][0]=BLACK_WIN;
    tuple6type[0][1][1][1][1][1]=BLACK_WIN;
    tuple6type[1][1][1][1][1][2]=BLACK_WIN;
    tuple6type[2][1][1][1][1][1]=BLACK_WIN;
    tuple6type[3][1][1][1][1][1]=BLACK_WIN;
    tuple6type[1][1][1][1][1][3]=BLACK_WIN;
}


// 棋盘更新
void Game::update_map(int row, int col) {
    // 先手黑棋
    if (flag_) {
        game_map_[row][col] = kBlack;
    } else {
        game_map_[row][col] = kWhite;
    }

    // 换手
    flag_ = !flag_;
}

// 人下棋
void Game::person_action(int row, int col) {
    if (game_map_[row][col] == kEmpty)
        update_map(row, col);
}

// 评分计算并找到最大值点
std::pair<int, int> Game::find_the_highest_point() {
    // 先计算评分
    // cal_score();

    // 找到分数最高的位置
    int max_score = 0;
    std::pair<int, int> point{0, 0};
    int row = 0, col = 0;
    for (row = 0; row < kBoardSize; ++row) {
        for (col = 0; col < kBoardSize; ++col) {
            // 空坐标
            if (game_map_[row][col] == kEmpty) {
                if (score_map_[row][col] >= max_score) {
                    if (max_score == score_map_[row][col]) {
                        std::pair<int, int> new_point{row, col};
                        // 如果评分相同, 随机选择
                        point = (rand() % 2 == 1) ? new_point : point;
                    }
                    max_score = score_map_[row][col];
                }
            }
        }
    }
    return point;
}

// ai下棋
void Game::ai_action() {
    std::pair<int, int> highest_point = find_the_highest_point();
    update_map(highest_point.first, highest_point.second);
}

// 是否获胜/死棋, 判断上下左右+两条斜线
// TODO: 实现评分矩阵, AI
int Game::evaluate() {
    // 包含两个边界的大棋盘, 边界用3标识 (https://github.com/livingsu/Gobang-ai/blob/master/Sources/chessai.cpp)
    int A[17][17], type;
    bool dead = true;
    for (int i = 0; i < 17; ++i) {
        A[i][0] = 3;
        A[i][16] = 3;
        A[0][i] = 3;
        A[16][i] = 3;
    }
    // 拷贝棋盘
    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 15; ++j) {
            // 只要有一个空就不死棋
            if (game_map_[i][j] == kEmpty) dead = false;
            A[i + 1][j + 1] = game_map_[i][j];
        }
    }
    // 横向
    for (int i = 1; i <= 15; ++i) {
        for (int j = 0; j < 12; ++j) {
            type = tuple6type[A[i][j]][A[i][j+1]][A[i][j+2]][A[i][j+3]][A[i][j+4]][A[i][j+5]];
            if (type == BLACK_WIN || type == WHITE_WIN) return type;
        }
    }
    // 纵向
    for (int j = 1; j <= 15; ++j) {
        for (int i = 0; i < 12; ++i) {
            type = tuple6type[A[i][j]][A[i+1][j]][A[i+2][j]][A[i+3][j]][A[i+4][j]][A[i+5][j]];
            if (type == BLACK_WIN || type == WHITE_WIN) return type;
        }
    }
    // 左上->右下斜线
    for(int i = 0; i < 12; ++i){
        for(int j = 0;j < 12; ++j){
            type = tuple6type[A[i][j]][A[i+1][j+1]][A[i+2][j+2]][A[i+3][j+3]][A[i+4][j+4]][A[i+5][j+5]];
            if (type == BLACK_WIN || type == WHITE_WIN) return type;
        }
    }
    // 右上->左下斜线
    for(int i = 0; i < 12; ++i){
        for(int j = 5; j < 17; ++j){
            type = tuple6type[A[i][j]][A[i+1][j-1]][A[i+2][j-2]][A[i+3][j-3]][A[i+4][j-4]][A[i+5][j-5]];
            if (type == BLACK_WIN || type == WHITE_WIN) return type;
        }
    }
    return dead ? DEAD_GAME : NO_WIN;
}



