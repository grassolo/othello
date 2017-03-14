Othello Project
03/14/17

Gregory Vitalyevich Rassolov
David Ignacio Fager

On 03/03/17, we set up our github repository together and practiced committing, pushing, pulling, etc.

On 03/04/17, Greg wrote the code for an AI that randomly picks a valid move, but the AI segfaulted after 1 move.

On 03/05/17, David fixed the segfault and improved the AI heuristic so that it would automatically take a corner space when possible, avoid taking spaces around corners, and otherwise choose the move which flipped the most pieces.

On 03/06/17, Greg improved the heuristic further by changing the cost values of squares around the corner, implemented the minimax algorithm, and turned in the final submission for part 1.

On 03/13/17, Greg improved the AI by making it consider the time constraints and increasing the depth of the minimax tree.

On 03/14/17, David wrote this document.


Above are team member contributions and AI improvements.  Here's why we think our strategies will work:

By avoiding squares around the corners, our AI will force opponents to place in those squares, allowing us to take corners.  Our AI uses a minimax tree of depth 6 until it starts to run low on time.  This will make our AI especially effective early in a game.  Ideally, we will be able to get corner and side pieces early on, so that even though our AI may be short on time at the end of the game, it will be easier to find good moves because we will already be in a good position.