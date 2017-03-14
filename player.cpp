#include "player.hpp"


/*
* Constructor for the player; initialize everything here. The side your AI is
* on (BLACK or WHITE) is passed in as "side". The constructor must finish
* within 30 seconds.
*/
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;
    board = new Board();
    testboard = new Board();
    slf = side;
    if (slf == WHITE) opp = BLACK;
    else if (slf == BLACK) opp = WHITE;
    turn = slf;
}


/*
* Destructor for the player.
*/
Player::~Player() {
    delete board;
    delete testboard;
}


/*
* Compute the next move given the opponent's last move. Your AI is
* expected to keep track of the board on its own. If this is the first move,
* or if the opponent passed on the last move, then opponentsMove will be
* nullptr.
*
* msLeft represents the time your AI has left for the total game, in
* milliseconds. doMove() must take no longer than msLeft, or your AI will
* be disqualified! An msLeft value of -1 indicates no time limit.
*
* The move returned must be legal; if there are no valid moves for your side,
* return nullptr.
*/
Move *Player::doMove(Move *opponentsMove, int msLeft) {
    time_t start_timer, current_timer;
    time(&start_timer);
    // The opponent's move is added to the game board.
    if (opponentsMove != nullptr) {
        board->doMove(opponentsMove, opp);
    }
    // score_slf: current score of the AI
    // score_opp: current score of the opponent
    int score_slf = board->count(slf);
    int score_opp = board->count(opp);
    int score [8][8] = {{100,-2, 1, 1, 1, 1,-2, 100},
                         {-2,-7, 0, 0, 0, 0,-7,-2 },
                         { 1, 0, 0, 0, 0, 0, 0, 1 },
                         { 1, 0, 0, 0, 0, 0, 0, 1 },
                         { 1, 0, 0, 0, 0, 0, 0, 1 },
                         { 1, 0, 0, 0, 0, 0, 0, 1 },
                         {-2,-7, 0, 0, 0, 0,-7,-2 },
                        {100,-2, 1, 1, 1, 1,-2, 100}};
    if (testingMinimax) {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                score[i][j] = 0;
            }
        }
    }
    int new_score_slf, new_score_opp, num_moves = 0;
    int max_score = -256;
    Move best_move(-1, -1);
    // std::cerr << "Current self score: " << score_slf << std::endl;
    // std::cerr << "Current oppn score: " << score_opp << std::endl;
    // std::cerr << "Checking available moves..." << std::endl;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            // The program checks all moves which are available to the AI.
            Move move(i, j);
            if (board->checkMove(&move, slf)) {
                testboard = board->copy();
                num_moves++;
                // The candidate move is enacted on testboard.
                testboard->doMove(&move, slf);
                turn = opp;
                // Calling tryMove adds the opponent's response to testboard.
                tryMove(1);
                new_score_slf = testboard->count(slf) - score_slf;
                new_score_opp = testboard->count(opp) - score_opp;
                score[i][j] += new_score_slf - new_score_opp;
                /**
                 * The move which maximizes the AI's score, subject to the
                 * opponent's predicted response, is stored.
                 */
                if (score[i][j] > max_score) {
                    max_score = score[i][j];
                    best_move = move;
                }
                turn = slf;
            }
            time(&current_timer);
            if (msLeft > 0) {
                if (difftime(start_timer, current_timer) > msLeft/2000) break;
            }
        }
    }
    // The best move is made on the board and returned.
    if (num_moves > 0) {
        Move *mymove = new Move(best_move.getX(), best_move.getY());
        board->doMove(mymove, slf);
        return mymove;
    }
    return nullptr;
}


/*
* Attempts all moves to fill out a minimax tree. Note that all changes are made
* to testboard, and that moves are "returned" by making them on the board.
*/
void Player::tryMove(int depth) {
    // score_slf: current score for AI
    // score_opp: current score for opponent
    // copy: hard copy of the current board
    int score_slf = testboard->count(slf);
    int score_opp = testboard->count(opp);
    Board *copy = testboard->copy();
    int score [8][8] = {{100,-2, 1, 1, 1, 1,-2, 100},
                         {-2,-7, 0, 0, 0, 0,-7,-2 },
                         { 1, 0, 0, 0, 0, 0, 0, 1 },
                         { 1, 0, 0, 0, 0, 0, 0, 1 },
                         { 1, 0, 0, 0, 0, 0, 0, 1 },
                         { 1, 0, 0, 0, 0, 0, 0, 1 },
                         {-2,-7, 0, 0, 0, 0,-7,-2 },
                        {100,-2, 1, 1, 1, 1,-2, 100}};
    if (testingMinimax) {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                score[i][j] = 0;
            }
        }
    }
    int new_score_slf, new_score_opp, num_moves = 0;
    int max_score = -256, min_score = 256;
    Move best_move(-1, -1);
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            /**
             * All 64 spaces are checked for possible moves, after resetting
             * test board to its original state. If the opponent's moves are
             * being tested, it will try to find the move which minimizes the
             * AI's score. If the AI's moves are being tested, it will try to
             * find the move which maxmizes the AI's score.
             */
            Move move(i, j);
            testboard = copy;
            if (turn == opp) {
                if (testboard->checkMove(&move, opp)) {
                    num_moves++;
                    testboard->doMove(&move, opp);
                    // The program will not check the entire tree.
                    if (depth < MAX_DEPTH) {
                        turn = slf;
                        tryMove(depth+1);
                        turn = opp;
                    }
                    /**
                     * If the program did not attempt to determine the AI's
                     * response to the opponent's move, then the score is
                     * calculated from the board directly after the opponent's
                     * move. Otherwise, the score is calculated from the board
                     * which includes the AI's response to that move.
                     */
                    new_score_slf = testboard->count(slf) - score_slf;
                    new_score_opp = testboard->count(opp) - score_opp;
                    score[i][j] += new_score_slf - new_score_opp;
                    // The move minimizing the AI's score is stored.
                    if (score[i][j] < min_score) {
                        min_score = score[i][j];
                        best_move = move;
                    }
                }
            }
            else if (turn == slf) {
                if (testboard->checkMove(&move, slf)) {
                    num_moves++;
                    testboard->doMove(&move, slf);
                    // The program will not check the entire tree.
                    if (depth < MAX_DEPTH) {
                        turn = opp;
                        tryMove(depth+1);
                        turn = slf;
                    }
                    /**
                     * If the program did not attempt to determine the response
                     * of the opponent to the AI's move, then the score is
                     * calculated from the board directly after the AI's move.
                     * Otherwise, the score is calculated from the board which
                     * includes the opponent's response to that move.
                     */
                    new_score_slf = testboard->count(slf) - score_slf;
                    new_score_opp = testboard->count(opp) - score_opp;
                    score[i][j] += new_score_slf - new_score_opp;
                    // The move maximizing the AI's score is stored.
                    if (score[i][j] > max_score) {
                        max_score = score[i][j];
                        best_move = move;
                    }
                }
            }
        }
    }
    // The test board is reset, then the best move is made.
    testboard = copy;
    if ((num_moves > 0) && (turn == opp)) {
        Move *mymove = new Move(best_move.getX(), best_move.getY());
        testboard->doMove(mymove, opp);
    }
}

