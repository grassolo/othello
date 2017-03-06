#ifndef __PLAYER_H__
#define __PLAYER_H__
#include <iostream>
// #include <vector>
#include <stdlib.h>
#include "common.hpp"
#include "board.hpp"
#define MAX_DEPTH 2

using namespace std;

class Player {
public:
    Player(Side side);
    ~Player();
    Move *doMove(Move *opponentsMove, int msLeft);
    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
    Board *board;
    Board *testboard;
    Side slf;
    Side opp;
    Side turn;
    int treedepth;
};

#endif

