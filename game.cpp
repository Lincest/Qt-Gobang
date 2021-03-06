#include "game.h"
#include <stdlib.h>
#include <time.h>
#include <utility>
#include <memory.h>
#include <qdebug.h>

Game::Game(){
    // 初始化棋盘
    memset(game_map_, 0, sizeof(game_map_));
    // 初始化评分(AI模式)
    init_tuple_six();
    // 先手
    flag_ = true;
}

// 初始化
void Game::start_game(GameType type) {
    game_type_ = type;

}

void Game::init_tuple_six() {
    // 初始化棋形对应评分, 准确来说这里的白指的是「当前下棋方」,黑是「对手」
    score_[kWhiteFive] = 2000000;      score_[kBlackFive] = -10000000;
    score_[kWhiteFour] = 50000;        score_[kBlackFour] = -100000;
    score_[kWhiteBlockFour] = 400;     score_[kBlackBlockFour] = -100000;
    score_[kWhiteThree] = 400;         score_[kBlackThree] = -8000;
    score_[kWhiteBlockThree] = 20;     score_[kBlackBlockThree] = -60;
    score_[kWhiteTwo] = 20;            score_[kBlackTwo] = -60;
    score_[kWhiteBlockTwo] = 1;        score_[kBlackBlockTwo] = -3;
    score_[kWhiteOne] = 1;             score_[kBlackOne] = -3;

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
    qDebug() << "生成评分完成..." << endl;
}


// 棋盘更新
void Game::update_map(int row, int col) {
    qDebug() << "update中..." << endl;
    // 先手黑棋
    if (flag_) {
        game_map_[row][col] = kBlack;
    } else {
        game_map_[row][col] = kWhite;
    }

    // 换手
    flag_ = !flag_;
}

void Game::do_random_start() {
    srand(time(0));
    int tmp_row = rand() % 8 + 4, tmp_col = rand() % 8 + 4;
    while (game_map_[tmp_row][tmp_col] != kEmpty) {
        tmp_row = rand() % 8 + 4, tmp_col = rand() % 8 + 4;
    }
    history_ai_.push_back({tmp_row, tmp_col});
    update_map(tmp_row, tmp_col);
}

// 人下棋
void Game::person_action(int row, int col) {
    qDebug() << "human: " << row << "," << col << endl;
    if (game_map_[row][col] == kEmpty) {
        update_map(row, col);
        history_person_.push_back({row, col});
    }
}

// ai下棋
void Game::ai_action() {
    qDebug() << "AI思考中..." << endl;
    ChessPieces chess = flag_ ? kBlack : kWhite;
    alphabeta(maxDepth, -INT_MAX, INT_MAX, chess); // init: [-∞,+∞]
    qDebug() << "AI: " << step_.pos.first << "," << step_.pos.second << " ,score=" << step_.score << endl;
    if (game_map_[step_.pos.first][step_.pos.second] == kEmpty) {
        history_ai_.push_back({step_.pos.first, step_.pos.second});
        update_map(step_.pos.first, step_.pos.second);
    }
}

// 悔棋
void Game::do_back() {
    qDebug() << "悔棋...";
    if (!history_ai_.empty() && !history_person_.empty()) {
        game_map_[history_ai_.back().first][history_ai_.back().second] = kEmpty;
        game_map_[history_person_.back().first][history_person_.back().second] = kEmpty;
        history_ai_.pop_back();
        history_person_.pop_back();
    } else {
        qDebug() << "没有可悔的棋..." << endl;
    }
}

// 「静态评价启发」: 生成估值最大的十个点, 采用遍历所有空点选择最大点的方式
std::vector<std::vector<int>> Game::generate_points(ChessPieces board[][kBoardSize], ChessPieces chess_type) {
    std::vector<std::vector<int>> allans;
    if (chess_type == kBlack) reverse_map();
    for (int i = 0; i < kBoardSize; ++i) {
        for (int j = 0; j < kBoardSize; ++j) {
            if (board[i][j] == kEmpty) {
                board[i][j] = kWhite;
                allans.push_back({i, j, evaluate(board).score});
                board[i][j] = kEmpty; // 清除
            }
        }
    }
    if (chess_type == kBlack) reverse_map();
    std::sort(allans.begin(), allans.end(), [](const std::vector<int>& a, const std::vector<int>& b) {
        return a[2] > b[2];
    });
    while (allans.size() > 10) allans.pop_back();
    return allans;
}

// 「MinMax」搜索 + 「alphabeta」剪枝
//  role是ai的执方
int Game::alphabeta(int depth, int alpha, int beta, ChessPieces role) {
    // 达到搜索深度, 搜索深度最底层(叶子结点应该是ai方下棋, 故需要利用generator_point找到最大的叶子)
    ChessPieces other = (role == kWhite ? kBlack : kWhite); // 与role相对
    std::vector<std::vector<int>> pos;
    if (depth == 0) {
        return generate_points(this->game_map_, role)[0][2];
    } else if (depth % 2 == 0) { // 偶数层, max层
        pos = generate_points(this->game_map_, role);
        for (int i = 0; i < pos.size(); ++i) {
            std::vector<int> cur = pos[i];
            game_map_[cur[0]][cur[1]] = role; // 模拟落子
            int v = alphabeta(depth - 1, alpha, beta, role);
            game_map_[cur[0]][cur[1]] = kEmpty; // 回溯
            if (v > alpha) {
                alpha = v;
                if (depth == maxDepth) {
                    // 落点
                    qDebug() << alpha << endl;
                    step_.pos.first = cur[0];
                    step_.pos.second = cur[1];
                    step_.score = v;
                }
            }
            if (beta <= alpha) break; // 在max层发现beta<=alpha, 剪枝
        }
        return alpha;
    } else if (depth % 2 == 1) { // 奇数层, min层
        pos = generate_points(this->game_map_, other);
        for (int i = 0; i < pos.size(); ++i) {
            std::vector<int> cur = pos[i];
            game_map_[cur[0]][cur[1]] = other;
            int v = alphabeta(depth - 1, alpha, beta, role);
            game_map_[cur[0]][cur[1]] = kEmpty;
            if (v < beta) {
                beta = v;
            }
            if (beta <= alpha) break; // 在min层发现beta<=alpha, 剪枝
        }
        return beta;
    }
}

// 棋盘反色
void Game::reverse_map() {
    for (int i = 0; i < kBoardSize; ++i) {
        for (int j = 0; j < kBoardSize; ++j) {
            if (game_map_[i][j] == kWhite) game_map_[i][j] = kBlack;
            else if (game_map_[i][j] == kBlack) game_map_[i][j] = kWhite;
        }
    }
}

// 实现评分
AllScore Game::evaluate(ChessPieces game_map_[][kBoardSize]) {
    AllScore ascore;
    // 包含两个边界的大棋盘, 边界用3标识 (https://github.com/livingsu/Gobang-ai/blob/master/Sources/chessai.cpp)
    int A[kBoardSize + 2][kBoardSize + 2];
    GameStatus type;
    for (int i = 0; i < kBoardSize + 2; ++i) {
        A[i][0] = 3;
        A[i][kBoardSize + 1] = 3;
        A[0][i] = 3;
        A[kBoardSize + 1][i] = 3;
    }
    // 拷贝棋盘
    bool dead = true;
    bool empty = true;
    for (int i = 0; i < kBoardSize; ++i) {
        for (int j = 0; j < kBoardSize; ++j) {
            // 只要有一个空就不死棋
            if (game_map_[i][j] == kEmpty) dead = false;
            if (game_map_[i][j] != kEmpty) empty = false;
            A[i + 1][j + 1] = game_map_[i][j];
        }
    }
    if (dead) {
        ascore.result = kDeadGame;
        return ascore;
    }
    if (empty) {
        ascore.result = kNotStart;
        return ascore;
    }
    // 横
    for (int i = 1; i <= kBoardSize; ++i) {
        for (int j = 0; j < kBoardSize - 3; ++j) {
            type = tuple_six_[A[i][j]][A[i][j+1]][A[i][j+2]][A[i][j+3]][A[i][j+4]][A[i][j+5]];
            ++ascore.stat[type];
        }
    }
    // 纵
    for (int j = 1; j <= kBoardSize; ++j) {
        for (int i = 0; i < kBoardSize - 3; ++i) {
            type = tuple_six_[A[i][j]][A[i+1][j]][A[i+2][j]][A[i+3][j]][A[i+4][j]][A[i+5][j]];
            ++ascore.stat[type];
        }
    }
    // 左上->右下
    for(int i = 0; i < kBoardSize - 3; ++i) {
        for(int j = 0; j < kBoardSize - 3; ++j) {
            type = tuple_six_[A[i][j]][A[i+1][j+1]][A[i+2][j+2]][A[i+3][j+3]][A[i+4][j+4]][A[i+5][j+5]];
            ++ascore.stat[type];
        }
    }
    // 右上->左下
    for(int i = 0; i < kBoardSize - 3; ++i){
        for(int j = 5; j < kBoardSize + 2; ++j){
            type = tuple_six_[A[i][j]][A[i+1][j-1]][A[i+2][j-2]][A[i+3][j-3]][A[i+4][j-4]][A[i+5][j-5]];
            ++ascore.stat[type];
        }
    }
    // 计算评分
    for (int i = 0; i <= 15; ++i) {
        ascore.score += ascore.stat[(GameStatus)i] * score_[(GameStatus)i];
//        qDebug() << i << ": " << ascore.stat[(GameStatus)i] << endl;
    }
    // 判断是否输赢
    if (ascore.stat[kWhiteFive] > 0) ascore.result = kWhiteWin;
    else if (ascore.stat[kBlackFive] > 0) ascore.result = kBlackWin;
    else ascore.result = kPlaying;
    return ascore;
}

// 实现评分
AllScore Game::evaluate() {
    return evaluate(this->game_map_);
}




