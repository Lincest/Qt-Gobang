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

    // 初始化评分(AI模式)
    if (game_type_ == kAI) {
        memset(score_map_, 0, sizeof(score_map_));
    }
    // 初始化六元组
    init_tuple_six();

    // 先手
    flag_ = true;
}

void Game::init_tuple_six() {
    // 初始化棋形对应评分
    score_[kWhiteFive] = 100000; score_[kBlackFive] = -100000;
    score_[kWhiteFour] = 10000; score_[kBlackFour] = -10000;
    score_[kWhiteThree] = 1000; score_[kBlackThree] = -1000;
    score_[kWhiteTwo] = 100; score_[kBlackTwo] = -100;
    score_[kWhiteOne] = 10; score_[kBlackOne] = -10;
    score_[kWhiteBlockFour] = 1000; score_[kBlackBlockFour] = -1000;
    score_[kWhiteBlockThree] = 100; score_[kBlackBlockThree] = -100;
    score_[kWhiteBlockTwo] = 10; score_[kBlackBlockTwo] = -10;

    // 初始化六元组
    memset(tuple_six_,0x7f,sizeof (tuple_six_));
    //白连5,ai赢
    tuple_six_[2][2][2][2][2][2] = kWhiteFive;
    tuple_six_[2][2][2][2][2][0] = kWhiteFive;
    tuple_six_[0][2][2][2][2][2] = kWhiteFive;
    tuple_six_[2][2][2][2][2][1] = kWhiteFive;
    tuple_six_[1][2][2][2][2][2] = kWhiteFive;
    tuple_six_[3][2][2][2][2][2] = kWhiteFive;
    tuple_six_[2][2][2][2][2][3] = kWhiteFive;
    //黑连5,ai输
    tuple_six_[1][1][1][1][1][1] = kBlackFive;
    tuple_six_[1][1][1][1][1][0] = kBlackFive;
    tuple_six_[0][1][1][1][1][1] = kBlackFive;
    tuple_six_[1][1][1][1][1][2] = kBlackFive;
    tuple_six_[2][1][1][1][1][1] = kBlackFive;
    tuple_six_[3][1][1][1][1][1] = kBlackFive;
    tuple_six_[1][1][1][1][1][3] = kBlackFive;
    // 活四
    tuple_six_[0][2][2][2][2][0] = kWhiteFour;
    tuple_six_[0][1][1][1][1][0] = kBlackFour;
    // 活三
    tuple_six_[0][2][2][2][0][0] = kWhiteThree;
    tuple_six_[0][2][2][0][2][0] = kWhiteThree;
    tuple_six_[0][2][0][2][2][0] = kWhiteThree;
    tuple_six_[0][0][2][2][2][0] = kWhiteThree;
    tuple_six_[0][1][1][1][0][0] = kBlackThree;
    tuple_six_[0][1][1][0][1][0] = kBlackThree;
    tuple_six_[0][1][0][1][1][0] = kBlackThree;
    tuple_six_[0][0][1][1][1][0] = kBlackThree;
    // 活二
    tuple_six_[0][2][2][0][0][0] = kWhiteTwo;
    tuple_six_[0][2][0][2][0][0] = kWhiteTwo;
    tuple_six_[0][2][0][0][2][0] = kWhiteTwo;
    tuple_six_[0][0][2][0][2][0] = kWhiteTwo;
    tuple_six_[0][0][2][2][0][0] = kWhiteTwo;
    tuple_six_[0][0][0][2][2][0] = kWhiteTwo;
    tuple_six_[0][1][1][0][0][0] = kBlackTwo;
    tuple_six_[0][1][0][1][0][0] = kBlackTwo;
    tuple_six_[0][1][0][0][1][0] = kBlackTwo;
    tuple_six_[0][0][1][0][1][0] = kBlackTwo;
    tuple_six_[0][0][1][1][0][0] = kBlackTwo;
    tuple_six_[0][0][0][1][1][0] = kBlackTwo;
    // 活一
    tuple_six_[0][1][0][0][0][0] = kBlackOne;
    tuple_six_[0][0][1][0][0][0] = kBlackOne;
    tuple_six_[0][0][0][1][0][0] = kBlackOne;
    tuple_six_[0][0][0][0][1][0] = kBlackOne;
    tuple_six_[0][2][0][0][0][0] = kWhiteOne;
    tuple_six_[0][0][2][0][0][0] = kWhiteOne;
    tuple_six_[0][0][0][2][0][0] = kWhiteOne;
    tuple_six_[0][0][0][0][2][0] = kWhiteOne;
    // 冲四
    tuple_six_[0][0][1][1][1][1] = kBlackBlockFour;
    tuple_six_[0][1][0][1][1][1] = kBlackBlockFour;
    tuple_six_[0][1][1][0][1][1] = kBlackBlockFour;
    tuple_six_[0][1][1][1][0][1] = kBlackBlockFour;
    tuple_six_[0][1][1][1][1][2] = kBlackBlockFour;
    tuple_six_[1][0][1][1][1][0] = kBlackBlockFour;
    tuple_six_[1][0][1][1][1][1] = kBlackBlockFour;
    tuple_six_[1][0][1][1][1][2] = kBlackBlockFour;
    tuple_six_[1][1][0][1][1][0] = kBlackBlockFour;
    tuple_six_[1][1][0][1][1][1] = kBlackBlockFour;
    tuple_six_[1][1][0][1][1][2] = kBlackBlockFour;
    tuple_six_[1][1][1][0][1][0] = kBlackBlockFour;
    tuple_six_[1][1][1][0][1][1] = kBlackBlockFour;
    tuple_six_[1][1][1][0][1][2] = kBlackBlockFour;
    tuple_six_[1][1][1][1][0][0] = kBlackBlockFour;
    tuple_six_[1][1][1][1][0][1] = kBlackBlockFour;
    tuple_six_[1][1][1][1][0][2] = kBlackBlockFour;
    tuple_six_[2][0][1][1][1][1] = kBlackBlockFour;
    tuple_six_[2][1][0][1][1][1] = kBlackBlockFour;
    tuple_six_[2][1][1][0][1][1] = kBlackBlockFour;
    tuple_six_[2][1][1][1][0][1] = kBlackBlockFour;
    tuple_six_[2][1][1][1][1][0] = kBlackBlockFour;
    tuple_six_[3][0][1][1][1][1] = kBlackBlockFour;
    tuple_six_[3][1][0][1][1][1] = kBlackBlockFour;
    tuple_six_[3][1][1][0][1][1] = kBlackBlockFour;
    tuple_six_[3][1][1][1][0][1] = kBlackBlockFour;
    tuple_six_[3][1][1][1][1][0] = kBlackBlockFour;

    tuple_six_[0][0][2][2][2][2] = kWhiteBlockFour;
    tuple_six_[0][2][0][2][2][2] = kWhiteBlockFour;
    tuple_six_[0][2][2][0][2][2] = kWhiteBlockFour;
    tuple_six_[0][2][2][2][0][2] = kWhiteBlockFour;
    tuple_six_[0][2][2][2][2][1] = kWhiteBlockFour;
    tuple_six_[1][0][2][2][2][2] = kWhiteBlockFour;
    tuple_six_[1][2][0][2][2][2] = kWhiteBlockFour;
    tuple_six_[1][2][2][0][2][2] = kWhiteBlockFour;
    tuple_six_[1][2][2][2][0][2] = kWhiteBlockFour;
    tuple_six_[1][2][2][2][2][0] = kWhiteBlockFour;
    tuple_six_[2][0][2][2][2][0] = kWhiteBlockFour;
    tuple_six_[2][0][2][2][2][1] = kWhiteBlockFour;
    tuple_six_[2][0][2][2][2][2] = kWhiteBlockFour;
    tuple_six_[2][2][0][2][2][0] = kWhiteBlockFour;
    tuple_six_[2][2][0][2][2][1] = kWhiteBlockFour;
    tuple_six_[2][2][0][2][2][2] = kWhiteBlockFour;
    tuple_six_[2][2][2][0][2][0] = kWhiteBlockFour;
    tuple_six_[2][2][2][0][2][1] = kWhiteBlockFour;
    tuple_six_[2][2][2][0][2][2] = kWhiteBlockFour;
    tuple_six_[2][2][2][2][0][0] = kWhiteBlockFour;
    tuple_six_[2][2][2][2][0][1] = kWhiteBlockFour;
    tuple_six_[2][2][2][2][0][2] = kWhiteBlockFour;
    tuple_six_[3][0][2][2][2][2] = kWhiteBlockFour;
    tuple_six_[3][2][0][2][2][2] = kWhiteBlockFour;
    tuple_six_[3][2][2][0][2][2] = kWhiteBlockFour;
    tuple_six_[3][2][2][2][0][2] = kWhiteBlockFour;
    tuple_six_[3][2][2][2][2][0] = kWhiteBlockFour;
    // 眠三
    tuple_six_[0][0][0][2][2][2] = kWhiteBlockThree;
    tuple_six_[0][0][2][0][2][2] = kWhiteBlockThree;
    tuple_six_[0][0][2][2][0][2] = kWhiteBlockThree;
    tuple_six_[0][0][2][2][2][1] = kWhiteBlockThree;
    tuple_six_[0][2][0][0][2][2] = kWhiteBlockThree;
    tuple_six_[0][2][0][2][0][2] = kWhiteBlockThree;
    tuple_six_[0][2][0][2][2][1] = kWhiteBlockThree;
    tuple_six_[0][2][2][0][0][2] = kWhiteBlockThree;
    tuple_six_[0][2][2][0][2][1] = kWhiteBlockThree;
    tuple_six_[0][2][2][2][0][1] = kWhiteBlockThree;
    tuple_six_[1][0][0][2][2][2] = kWhiteBlockThree;
    tuple_six_[1][0][2][0][2][2] = kWhiteBlockThree;
    tuple_six_[1][0][2][2][0][2] = kWhiteBlockThree;
    tuple_six_[1][0][2][2][2][0] = kWhiteBlockThree;
    tuple_six_[1][2][0][0][2][2] = kWhiteBlockThree;
    tuple_six_[1][2][0][2][0][2] = kWhiteBlockThree;
    tuple_six_[1][2][0][2][2][0] = kWhiteBlockThree;
    tuple_six_[1][2][2][0][0][2] = kWhiteBlockThree;
    tuple_six_[1][2][2][0][2][0] = kWhiteBlockThree;
    tuple_six_[1][2][2][2][0][0] = kWhiteBlockThree;
    tuple_six_[2][0][0][2][2][0] = kWhiteBlockThree;
    tuple_six_[2][0][0][2][2][1] = kWhiteBlockThree;
    tuple_six_[2][0][0][2][2][2] = kWhiteBlockThree;
    tuple_six_[2][0][2][0][2][0] = kWhiteBlockThree;
    tuple_six_[2][0][2][0][2][1] = kWhiteBlockThree;
    tuple_six_[2][0][2][0][2][2] = kWhiteBlockThree;
    tuple_six_[2][0][2][2][0][0] = kWhiteBlockThree;
    tuple_six_[2][0][2][2][0][1] = kWhiteBlockThree;
    tuple_six_[2][0][2][2][0][2] = kWhiteBlockThree;
    tuple_six_[2][2][0][0][2][0] = kWhiteBlockThree;
    tuple_six_[2][2][0][0][2][1] = kWhiteBlockThree;
    tuple_six_[2][2][0][0][2][2] = kWhiteBlockThree;
    tuple_six_[2][2][0][2][0][0] = kWhiteBlockThree;
    tuple_six_[2][2][0][2][0][1] = kWhiteBlockThree;
    tuple_six_[2][2][0][2][0][2] = kWhiteBlockThree;
    tuple_six_[2][2][2][0][0][0] = kWhiteBlockThree;
    tuple_six_[2][2][2][0][0][1] = kWhiteBlockThree;
    tuple_six_[2][2][2][0][0][2] = kWhiteBlockThree;
    tuple_six_[3][0][0][2][2][2] = kWhiteBlockThree;
    tuple_six_[3][0][2][0][2][2] = kWhiteBlockThree;
    tuple_six_[3][0][2][2][0][2] = kWhiteBlockThree;
    tuple_six_[3][0][2][2][2][0] = kWhiteBlockThree;
    tuple_six_[3][2][0][0][2][2] = kWhiteBlockThree;
    tuple_six_[3][2][0][2][0][2] = kWhiteBlockThree;
    tuple_six_[3][2][0][2][2][0] = kWhiteBlockThree;
    tuple_six_[3][2][2][0][0][2] = kWhiteBlockThree;
    tuple_six_[3][2][2][0][2][0] = kWhiteBlockThree;
    tuple_six_[3][2][2][2][0][0] = kWhiteBlockThree;

    tuple_six_[0][0][0][1][1][1] = kBlackBlockThree;
    tuple_six_[0][0][1][0][1][1] = kBlackBlockThree;
    tuple_six_[0][0][1][1][0][1] = kBlackBlockThree;
    tuple_six_[0][0][1][1][1][2] = kBlackBlockThree;
    tuple_six_[0][1][0][0][1][1] = kBlackBlockThree;
    tuple_six_[0][1][0][1][0][1] = kBlackBlockThree;
    tuple_six_[0][1][0][1][1][2] = kBlackBlockThree;
    tuple_six_[0][1][1][0][0][1] = kBlackBlockThree;
    tuple_six_[0][1][1][0][1][2] = kBlackBlockThree;
    tuple_six_[0][1][1][1][0][2] = kBlackBlockThree;
    tuple_six_[1][0][0][1][1][0] = kBlackBlockThree;
    tuple_six_[1][0][0][1][1][1] = kBlackBlockThree;
    tuple_six_[1][0][0][1][1][2] = kBlackBlockThree;
    tuple_six_[1][0][1][0][1][0] = kBlackBlockThree;
    tuple_six_[1][0][1][0][1][1] = kBlackBlockThree;
    tuple_six_[1][0][1][0][1][2] = kBlackBlockThree;
    tuple_six_[1][0][1][1][0][0] = kBlackBlockThree;
    tuple_six_[1][0][1][1][0][1] = kBlackBlockThree;
    tuple_six_[1][0][1][1][0][2] = kBlackBlockThree;
    tuple_six_[1][1][0][0][1][0] = kBlackBlockThree;
    tuple_six_[1][1][0][0][1][1] = kBlackBlockThree;
    tuple_six_[1][1][0][0][1][2] = kBlackBlockThree;
    tuple_six_[1][1][0][1][0][0] = kBlackBlockThree;
    tuple_six_[1][1][0][1][0][1] = kBlackBlockThree;
    tuple_six_[1][1][0][1][0][2] = kBlackBlockThree;
    tuple_six_[1][1][1][0][0][0] = kBlackBlockThree;
    tuple_six_[1][1][1][0][0][1] = kBlackBlockThree;
    tuple_six_[1][1][1][0][0][2] = kBlackBlockThree;
    tuple_six_[2][0][0][1][1][1] = kBlackBlockThree;
    tuple_six_[2][0][1][0][1][1] = kBlackBlockThree;
    tuple_six_[2][0][1][1][0][1] = kBlackBlockThree;
    tuple_six_[2][0][1][1][1][0] = kBlackBlockThree;
    tuple_six_[2][1][0][0][1][1] = kBlackBlockThree;
    tuple_six_[2][1][0][1][0][1] = kBlackBlockThree;
    tuple_six_[2][1][0][1][1][0] = kBlackBlockThree;
    tuple_six_[2][1][1][0][0][1] = kBlackBlockThree;
    tuple_six_[2][1][1][0][1][0] = kBlackBlockThree;
    tuple_six_[2][1][1][1][0][0] = kBlackBlockThree;
    tuple_six_[3][0][0][1][1][1] = kBlackBlockThree;
    tuple_six_[3][0][1][0][1][1] = kBlackBlockThree;
    tuple_six_[3][0][1][1][0][1] = kBlackBlockThree;
    tuple_six_[3][0][1][1][1][0] = kBlackBlockThree;
    tuple_six_[3][1][0][0][1][1] = kBlackBlockThree;
    tuple_six_[3][1][0][1][0][1] = kBlackBlockThree;
    tuple_six_[3][1][0][1][1][0] = kBlackBlockThree;
    tuple_six_[3][1][1][0][0][1] = kBlackBlockThree;
    tuple_six_[3][1][1][0][1][0] = kBlackBlockThree;
    tuple_six_[3][1][1][1][0][0] = kBlackBlockThree;
    // 眠二
    tuple_six_[0][0][0][0][1][1] = kBlackBlockTwo;
    tuple_six_[0][0][0][1][0][1] = kBlackBlockTwo;
    tuple_six_[0][0][0][1][1][2] = kBlackBlockTwo;
    tuple_six_[0][0][1][0][0][1] = kBlackBlockTwo;
    tuple_six_[0][0][1][0][1][2] = kBlackBlockTwo;
    tuple_six_[0][0][1][1][0][2] = kBlackBlockTwo;
    tuple_six_[0][1][0][0][0][1] = kBlackBlockTwo;
    tuple_six_[0][1][0][0][1][2] = kBlackBlockTwo;
    tuple_six_[0][1][0][1][0][2] = kBlackBlockTwo;
    tuple_six_[0][1][1][0][0][2] = kBlackBlockTwo;
    tuple_six_[1][0][0][0][1][0] = kBlackBlockTwo;
    tuple_six_[1][0][0][0][1][1] = kBlackBlockTwo;
    tuple_six_[1][0][0][0][1][2] = kBlackBlockTwo;
    tuple_six_[1][0][0][1][0][0] = kBlackBlockTwo;
    tuple_six_[1][0][0][1][0][1] = kBlackBlockTwo;
    tuple_six_[1][0][0][1][0][2] = kBlackBlockTwo;
    tuple_six_[1][0][1][0][0][0] = kBlackBlockTwo;
    tuple_six_[1][0][1][0][0][1] = kBlackBlockTwo;
    tuple_six_[1][0][1][0][0][2] = kBlackBlockTwo;
    tuple_six_[1][1][0][0][0][0] = kBlackBlockTwo;
    tuple_six_[1][1][0][0][0][1] = kBlackBlockTwo;
    tuple_six_[1][1][0][0][0][2] = kBlackBlockTwo;
    tuple_six_[2][0][0][0][1][1] = kBlackBlockTwo;
    tuple_six_[2][0][0][1][0][1] = kBlackBlockTwo;
    tuple_six_[2][0][0][1][1][0] = kBlackBlockTwo;
    tuple_six_[2][0][1][0][0][1] = kBlackBlockTwo;
    tuple_six_[2][0][1][0][1][0] = kBlackBlockTwo;
    tuple_six_[2][0][1][1][0][0] = kBlackBlockTwo;
    tuple_six_[2][1][0][0][0][1] = kBlackBlockTwo;
    tuple_six_[2][1][0][0][1][0] = kBlackBlockTwo;
    tuple_six_[2][1][0][1][0][0] = kBlackBlockTwo;
    tuple_six_[2][1][1][0][0][0] = kBlackBlockTwo;
    tuple_six_[3][0][0][0][1][1] = kBlackBlockTwo;
    tuple_six_[3][0][0][1][0][1] = kBlackBlockTwo;
    tuple_six_[3][0][0][1][1][0] = kBlackBlockTwo;
    tuple_six_[3][0][1][0][0][1] = kBlackBlockTwo;
    tuple_six_[3][0][1][0][1][0] = kBlackBlockTwo;
    tuple_six_[3][0][1][1][0][0] = kBlackBlockTwo;
    tuple_six_[3][1][0][0][0][1] = kBlackBlockTwo;
    tuple_six_[3][1][0][0][1][0] = kBlackBlockTwo;
    tuple_six_[3][1][0][1][0][0] = kBlackBlockTwo;
    tuple_six_[3][1][1][0][0][0] = kBlackBlockTwo;

    tuple_six_[0][0][0][0][2][2] = kWhiteBlockTwo;
    tuple_six_[0][0][0][2][0][2] = kWhiteBlockTwo;
    tuple_six_[0][0][0][2][2][1] = kWhiteBlockTwo;
    tuple_six_[0][0][2][0][0][2] = kWhiteBlockTwo;
    tuple_six_[0][0][2][0][2][1] = kWhiteBlockTwo;
    tuple_six_[0][0][2][2][0][1] = kWhiteBlockTwo;
    tuple_six_[0][2][0][0][0][2] = kWhiteBlockTwo;
    tuple_six_[0][2][0][0][2][1] = kWhiteBlockTwo;
    tuple_six_[0][2][0][2][0][1] = kWhiteBlockTwo;
    tuple_six_[0][2][2][0][0][1] = kWhiteBlockTwo;
    tuple_six_[1][0][0][0][2][2] = kWhiteBlockTwo;
    tuple_six_[1][0][0][2][0][2] = kWhiteBlockTwo;
    tuple_six_[1][0][0][2][2][0] = kWhiteBlockTwo;
    tuple_six_[1][0][2][0][0][2] = kWhiteBlockTwo;
    tuple_six_[1][0][2][0][2][0] = kWhiteBlockTwo;
    tuple_six_[1][0][2][2][0][0] = kWhiteBlockTwo;
    tuple_six_[1][2][0][0][0][2] = kWhiteBlockTwo;
    tuple_six_[1][2][0][0][2][0] = kWhiteBlockTwo;
    tuple_six_[1][2][0][2][0][0] = kWhiteBlockTwo;
    tuple_six_[1][2][2][0][0][0] = kWhiteBlockTwo;
    tuple_six_[2][0][0][0][2][0] = kWhiteBlockTwo;
    tuple_six_[2][0][0][0][2][1] = kWhiteBlockTwo;
    tuple_six_[2][0][0][0][2][2] = kWhiteBlockTwo;
    tuple_six_[2][0][0][2][0][0] = kWhiteBlockTwo;
    tuple_six_[2][0][0][2][0][1] = kWhiteBlockTwo;
    tuple_six_[2][0][0][2][0][2] = kWhiteBlockTwo;
    tuple_six_[2][0][2][0][0][0] = kWhiteBlockTwo;
    tuple_six_[2][0][2][0][0][1] = kWhiteBlockTwo;
    tuple_six_[2][0][2][0][0][2] = kWhiteBlockTwo;
    tuple_six_[2][2][0][0][0][0] = kWhiteBlockTwo;
    tuple_six_[2][2][0][0][0][1] = kWhiteBlockTwo;
    tuple_six_[2][2][0][0][0][2] = kWhiteBlockTwo;
    tuple_six_[3][0][0][0][2][2] = kWhiteBlockTwo;
    tuple_six_[3][0][0][2][0][2] = kWhiteBlockTwo;
    tuple_six_[3][0][0][2][2][0] = kWhiteBlockTwo;
    tuple_six_[3][0][2][0][0][2] = kWhiteBlockTwo;
    tuple_six_[3][0][2][0][2][0] = kWhiteBlockTwo;
    tuple_six_[3][0][2][2][0][0] = kWhiteBlockTwo;
    tuple_six_[3][2][0][0][0][2] = kWhiteBlockTwo;
    tuple_six_[3][2][0][0][2][0] = kWhiteBlockTwo;
    tuple_six_[3][2][0][2][0][0] = kWhiteBlockTwo;
    tuple_six_[3][2][2][0][0][0] = kWhiteBlockTwo;
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
    int row = 0, col = 0, cnt = 1;
    for (row = 0; row < kBoardSize; ++row) {
        for (col = 0; col < kBoardSize; ++col) {
            // 空坐标
            if (game_map_[row][col] == kEmpty) {
                if (score_map_[row][col] >= max_score) {
                    if (max_score == score_map_[row][col]) {
                        // 如果评分相同, 随机选择
                        point = (rand() % cnt == 0) ? std::make_pair(row, col) : point;
                        ++cnt;
                    } else {
                        point = std::make_pair(row, col);
                        cnt = 0;
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
AllScore Game::evaluate() {
    AllScore ascore;
    // 包含两个边界的大棋盘, 边界用3标识 (https://github.com/livingsu/Gobang-ai/blob/master/Sources/chessai.cpp)
    int A[17][17];
    GameStatus type;
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
    if (dead) {
        ascore.result = kDeadGame;
        return ascore;
    }
    // 横向
    for (int i = 1; i <= 15; ++i) {
        for (int j = 0; j < 12; ++j) {
            type = tuple_six_[A[i][j]][A[i][j+1]][A[i][j+2]][A[i][j+3]][A[i][j+4]][A[i][j+5]];
            ++ascore.stat[type];
        }
    }
    // 纵向
    for (int j = 1; j <= 15; ++j) {
        for (int i = 0; i < 12; ++i) {
            type = tuple_six_[A[i][j]][A[i+1][j]][A[i+2][j]][A[i+3][j]][A[i+4][j]][A[i+5][j]];
            ++ascore.stat[type];
        }
    }
    // 左上->右下斜线
    for(int i = 0; i < 12; ++i){
        for(int j = 0;j < 12; ++j){
            type = tuple_six_[A[i][j]][A[i+1][j+1]][A[i+2][j+2]][A[i+3][j+3]][A[i+4][j+4]][A[i+5][j+5]];
            ++ascore.stat[type];
        }
    }
    // 右上->左下斜线
    for(int i = 0; i < 12; ++i){
        for(int j = 5; j < 17; ++j){
            type = tuple_six_[A[i][j]][A[i+1][j-1]][A[i+2][j-2]][A[i+3][j-3]][A[i+4][j-4]][A[i+5][j-5]];
            ++ascore.stat[type];
        }
    }
    // 计算评分
    for (int i = 0; i <= 15; ++i) {
        ascore.score += ascore.stat[(GameStatus)i] * score_[(GameStatus)i];
    }
    // 判断是否输赢
    if (ascore.stat[kWhiteFive] > 0) ascore.result = kWhiteWin;
    else if (ascore.stat[kBlackFive] > 0) ascore.result = kBlackWin;
    else ascore.result = kPlaying;
    return ascore;
}



