/**
*	author:     roccoshi
*	created:    2021-04-17 14:11:37
*   description: 极小化极大搜索 + ab剪枝
*/
#include<bits/stdc++.h>
using namespace std;

vector<int> 

int negamax(int depth, int alpha, int beta) {
    if (depth == 0) {
        return 0;
    } else {
        int t; // evaluate值模拟
        for (int i = 0; i < 3; ++i) {
            cin >> t;
            int value = -negamax(depth - 1, -beta, -alpha);
            if (value > alpha) {
                // 剪枝
                if (value >= beta) {
                    cout << "cut the " << depth << "-depth " << i << "th node\n";
                    return beta;
                }
            }
            alpha = value;
        }
    }
    return alpha;
}

int main () {
    negamax(3, -INT_MAX, INT_MAX);
}