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
    slf = side;
    if (slf == WHITE) opp = BLACK;
    else if (slf == BLACK) opp = WHITE;
    /*
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */
}

/*
 * Destructor for the player.
 */
Player::~Player() {
    delete board;
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
    /*
     * TODO: Implement how moves your AI should play here. You should first
     * process the opponent's move before calculating your own move.
     */
    std::cerr << "MOVE: " << msLeft << std::endl;
    if (opponentsMove != nullptr) {
        std::cerr << opponentsMove->getX() << std::endl;
        std::cerr << opponentsMove->getY() << std::endl;
        board->doMove(opponentsMove, opp);
    }
    int score_slf = board->count(slf);
    int score_opp = board->count(opp);
    Board *copy = board->copy();
    std::vector<Move> valid_moves;
    int score [8][8] = { };
    int new_score_slf, new_score_opp, max_score = 0;
    Move best_move(0, 0);
    std::cerr << "Current self score: " << score_slf << std::endl;
    std::cerr << "Current oppn score: " << score_opp << std::endl;
    std::cerr << "Checking available moves..." << std::endl;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Move move(i, j);
            if (board->checkMove(&move, slf)) {
                std::cerr << "x: " << i << " y: " << j << std::endl;
                copy = board->copy();
                valid_moves.push_back(move);
                copy->doMove(&move, slf);
                new_score_slf = copy->count(slf) - score_slf;
                new_score_opp = copy->count(opp) - score_opp;
                std::cerr << "Self token gain: " << new_score_slf << std::endl;
                std::cerr << "Oppn token gain: " << new_score_opp << std::endl;
                score[i][j] = new_score_slf - new_score_opp;
                std::cerr << "Move score: " << score[i][j] <<  std::endl;
                if (score[i][j] > max_score) {
                    max_score = score[i][j];
                    best_move = move;
                }
            }
        }
    }
    std::cerr << "Best possible score: " << max_score << std::endl;
    int num_moves = valid_moves.size();
    if (num_moves > 0) {
        int random_index = rand() % num_moves;
        Move* mymove = &(valid_moves[random_index]);
        board->doMove(mymove, slf);
        std::cerr << mymove->getX() << std::endl;
        std::cerr << mymove->getY() << std::endl;
        std::cerr << "New self score: " << board->count(slf) << std::endl;
        std::cerr << "New oppn score: " << board->count(opp) << std::endl;
        return mymove;
    }
    return nullptr;
}
