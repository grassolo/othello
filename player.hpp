#ifndef __PLAYER_H__
#define __PLAYER_H__
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "common.hpp"
#include "board.hpp"
#define MAX_DEPTH 6

using namespace std;

class Player {
public:
    Player(Side side);
    ~Player();
    Move *doMove(Move *opponentsMove, int msLeft);
    void tryMove(int depth);
    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
    Board *board;
    Board *testboard;
    Side slf;
    Side opp;
    Side turn;
};

#endif

