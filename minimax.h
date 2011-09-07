/* minimax.h */

/*

1. save global _TURN to turnSave 
2. create and initialize some states
3. call minimaxr(0, board, moves) return # pieces at _MAXDEPTH
4. restore _TURN = turnsv
5. make actual move corresponding to greatest # of pieces 
6. return nothing

*/

int countPieces(char *);
void byteCopy(char *, char *);
int minimaxr(int, char *, char *);
void minimaxm(unsigned char *);
void switchTurn(void);

// this is called from main
void minimaxm(unsigned char * pieces) {

	int i, j, k, themove, matchDir;
	char board[64], moves[64];
	int totals[64], max, temp;

	// save the turn and restore when done
	_TURNSAVE = _TURN;

	// pieces remains unchanged, do not restore
	byteCopy(pieces, board);

	// initialize totals & moves
	for (j = 0; j < 64; j++) 
		totals[j] = moves[j] = -1;

	// find all legal moves for the board passed in
	k = 0;
	for (j = 0; j < 64; j++) {
		if (isLegal(j, pieces) != 0) {
			moves[k++] = j;
		}
	}

	i = 0;
	// for each move get the total pieces at _MAXDEPTH
	while (moves[i] != -1) {
		//
		// must isolate the move to a different place,
		// because of the way that the minimaxr works
		//
		char move_tmp[2];
		move_tmp[0] = moves[i]; 
		move_tmp[1] = 0;
		totals[i] = minimaxr(0, board, &move_tmp[0]);
		i++;
	}

	//
	// sort the totals, find the biggest one
	// ** note: if there are one or more equal totals, in this
	// setup, we just take the first one we find.  this effects
	// game behavior.  
	//
	// ** note2:  end game logic can be put here.  
	//
	max = temp = themove = i = 0;
	while ( (temp = totals[i]) != -1 ) {
		if (temp > max) {
			max = temp;
			themove = moves[i];
		}
		++i;
	}	
	// restore the turn so that we can make the real move
	// ... just in case ...
	_TURN = _TURNSAVE;
	if ( (matchDir = isLegal(themove, pieces)) == _FALSE ) {
		//
		// ** extravagant error output
		//
		fprintf(stderr, "the AI made an illegal move = %d\n", themove);
		flip(themove, pieces, matchDir);
		k = 0;
		fprintf(stderr, "\nmoves: ");
		while (moves[k] != -1)
			fprintf(stderr, "%d ", moves[k++]);
		k = 0;
		fprintf(stderr, "\ntotals: ");
		while (totals[k] != -1)
			fprintf(stderr, "%d ", totals[k]);
		// wait until externally killed so I can read the output
//		sleep(60);
		exit(1);
	}
	flip(themove, pieces, matchDir);
	switchTurn();
}


// --- main ---
// depth: is the level we're on, each recursion is depth+1
// board: pointer to a previous board
// move: pointer to array of legal moves.  # of sqr, end in 0
// returns largest number of _PLAYERTWO pieces at level _MAXDEPTH
int minimaxr(int depth, char * board, char * move) {

	// variables
	register int i, j, k; 
	int matchDir, maxpieces = 0, temppieces = 0;

	// arrays of pointers to possible boards
	char *boards[64], *moves[64];
	for (i = 0; i < 64; i++) 
		boards[i] = moves[i] = NULL;

	if (depth == 0) {

		// make a copy of the board that was passed in
		boards[0] = (char *)malloc(64*sizeof(char));
		byteCopy(board, boards[0]);

		// allocate an array of moves
		moves[0] = (char *)malloc(64*sizeof(char));
		for (k = 0; k < 64; k++) {
			moves[0][k] = 0;
		}

		// a move was passed in, make it on the 
		// copy of the board
		matchDir = isLegal(move[0], boards[0]);
		flip(move[0], boards[0], matchDir);

		switchTurn();

		// set the legal moves in moves[i] for board
		k = 0;
		for (j = 0; j < 64; j++) {
			if (isLegal(j, boards[0]) != 0) {
				moves[0][k++] = j;
			}
		}

		// recurse down a level with the moves
		maxpieces = minimaxr(depth+1, boards[0], moves[0]);

		free(moves[0]);
		free(boards[0]);
		return maxpieces;
	}

	else if (depth < _MAXDEPTH) {

		// make all the move(s)

/* 
 * a small array of ints is passed in.  each possible move for "this" turn
 * is stored as a char square number from (0 - 63).  This loop creates a 
 * new recursed child scenario for each child, until the max-recursion depth
 * is reached.  
 */
		i = 0;
		while (move[i] != 0) {

			// setup a pointer to a new board
			boards[i] = (char *)malloc(64*sizeof(char));
			byteCopy(board, boards[i]);

			// setup pointer to new moves list
			moves[i] = (char *)malloc(64*sizeof(char));
			byteCopy(move, moves[i]);

			// actually make the move
			matchDir = isLegal(move[i], boards[i]);
			flip(move[i], boards[i], matchDir);
			
			switchTurn();	
			
			// set the legal moves in moves[i] for boards[i]
			k = 0;
			for (j = 0; j < 64; j++) {
				if (isLegal(j, boards[i]) != 0) {
					moves[i][k++] = j;
				}
			}

			// recurse down a level with this particular move
			temppieces = minimaxr(depth+1, boards[i], moves[i]);
			if (temppieces > maxpieces)
				maxpieces = temppieces;
			
			free(boards[i]);
			free(moves[i]);
			++i;
		}
		return maxpieces;
	} 

	else   {
		return (countPieces( board ));
	}
	// never reached 
	// tells compiler to shut the fuck up
	return 0;
}

int countPieces( char * board ) { 
	int total = 0; 
	register int i = 0;
	while (i < 64) {
		if (_AIPLAYER == _AIPLAYERONE) {
			if (board[i++] == _PLAYERONE)
				++total;
		} else if (_AIPLAYER == _AIPLAYERTWO) {
			if (board[i++] == _PLAYERTWO)
				++total;
		} else if (_AIPLAYER == _AIBOTHPLAYERS) {
			if (_TURN == _PLAYERTWO) {
				if (board[i++] == _PLAYERTWO)
					++total;
			} else { 
				if (board[i++] == _PLAYERONE)
					++total;
			}
		}
	}
	return total;
}

// copies left to right
void byteCopy( char * _old, char * _new) {
	register int i;
	for (i = 0; i < 64; i++) 
		_new[i] = _old[i];
}

void switchTurn(void) {
	_TURN = (_TURN == _PLAYERONE) ? _PLAYERTWO : _PLAYERONE;
}
