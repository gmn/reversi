/* reversi.c */

#include "reversi.h"
#include "minimax.h"

int main(int argc, char * argv[]) {

	checkArgs(argc, argv);

	int i = 0;
	SDL_Surface *screen = (SDL_Surface *)malloc(sizeof(SDL_Surface));
	SDL_Rect *board = (SDL_Rect *)malloc(sizeof(SDL_Rect));
	SDL_Event event;
	int exitkey = 0;

	// SDL_Rect board the size of the screen
	board->x = board->y = 0;
	board->w = _VID_X;
	board->h = _VID_Y;

	// the gameboard array. values: white, black, none
	unsigned char pieces[64];
	for (i = 0; i < 64; i++) pieces[i] = 0;

	// standard starting arrangement
	pieces[27] = _BADGUY;
	pieces[28] = _GOODGUY;
	pieces[35] = _GOODGUY;
	pieces[36] = _BADGUY;

	if( SDL_Init( SDL_INIT_VIDEO ) < 0)
	{
		fprintf(stderr, "Error: Couldn't set up SDL: %s", SDL_GetError());
		return -1;
	}

	/*when through executing, call SDL_Quit */
	atexit(SDL_Quit);

	screen = SDL_SetVideoMode( _VID_X, _VID_Y, _COLOR_DEPTH, SDL_SWSURFACE );
	if( screen == NULL ) {
		fprintf(stderr, "Couldn't set vid-mode %dx%dx%d: %s", _VID_X, _VID_Y, _COLOR_DEPTH, SDL_GetError() );
		return -1;
	}

	SDL_WM_SetCaption("SDL Reversi", "SDL Reversi");

	printf("\t BPP \t\t : %d\n", screen->format->BytesPerPixel*8);

	// playerone always starts in othello
	_TURN = _PLAYERONE;

	/* polling loop for user input */
	while (!exitkey) {
		// draws the lines on the board
		drawBoard(screen); 
		// re-checks piece information in case of changes
		updatePeices(screen, pieces);
		// re-draw the whole screen
		SDL_Flip(screen);

		// do AI move if any
		aiMove(pieces);
			
		while (SDL_PollEvent(&event)) {	
			if (event.type == SDL_QUIT) 
				exitkey = 1;
			else if (event.type == SDL_MOUSEBUTTONDOWN) {
				if (event.button.button == 1) 
					checkMouse(event, pieces);
			} else if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_c)
					if (board != NULL)
						changeColor(screen, board);
				if (event.key.keysym.sym == SDLK_s)
					skipTurn(pieces);
			}
		}
		checkGame(pieces);
	}
	return 0;      
} 


void setPixel(SDL_Surface *screen, Uint8 R, Uint8 G, Uint8 B, int x, int y)
{
	Uint32 color = SDL_MapRGB(screen->format, R,G,B);
	
	if (SDL_MUSTLOCK(screen) ) {
		if (SDL_LockSurface(screen) < 0) {
			return;
		}
	}
	switch (screen->format->BytesPerPixel) {
		case 1: { /* assuming 8-bpp */
			Uint8 *bufp;
			bufp = (Uint8 *)screen->pixels + y*screen->pitch + x;
			*bufp = color;
		}
		break;
		
		case 2: { /* Probably 15-bpp or 16-bpp */
			Uint16 *bufp;
			bufp = (Uint16 *)screen->pixels + y*screen->pitch/2 + x;
			*bufp = color;
		}
		break;

		case 3: { /* Slow 24-bpp mode, usually not used */
			Uint8 *bufp;
			bufp = (Uint8 *)screen->pixels + y*screen->pitch + x;
			*(bufp+screen->format->Rshift/8) = R;
			*(bufp+screen->format->Gshift/8) = G;
			*(bufp+screen->format->Bshift/8) = B;
		}
		break;

		case 4: { /* Probably 32-bpp */
			Uint32 *bufp;
			bufp = (Uint32 *)screen->pixels + y*screen->pitch/4 + x;
			*bufp = color;
		}
		break;
	}
	if ( SDL_MUSTLOCK(screen) ) {
		SDL_UnlockSurface(screen);
	}
}
 
void drawBoard(SDL_Surface *screen) {
	int i;
	Uint32 color;
	if (_TURN == _PLAYERONE)
		color = _PLAYER1_COLOR;
	else
		color = _PLAYER2_COLOR;
	for (i = 0; i < 7; i++) {
		if (i < 4) {
			setHLine(screen, 0, _VID_X, i, _BORDER_COLOR); 	//top
			setHLine(screen, 0, _VID_X, _VID_X-i, _BORDER_COLOR); //bottom
			setVLine(screen, 0, _VID_Y, i, _BORDER_COLOR);	//left
			setVLine(screen, 0, _VID_Y, _VID_Y-i, _BORDER_COLOR); //right
		}
		setHLine(screen, 0, _VID_X, (i+1)*_SQR_W, _BORDER_COLOR); //middle-lines
		setVLine(screen, 0, _VID_Y, (i+1)*_SQR_W, _BORDER_COLOR);
		// small square that determines whos turn it is in top-left corner
		setHLine(screen, 4, 11, i+4, color);
	}
} //end drawBoard


void setHLine(SDL_Surface *screen, int x1, int x2, int y, unsigned int color) {
	int i;
	if ((x1 > _VID_X) || (x1 < 0) || (x2 > _VID_X) || (x2 < 0) || 
		(y > _VID_Y) || (y < 0)) {
		printf("line draw exception\n");
		exit(1);
	} else {
		for (i = x1; i < x2+1; i++) {
			setPixel(screen, ((color>>16)&0xff), ((color>>8)&0xff), (color&0xff), i, y);
		}
	}
}

void setVLine(SDL_Surface *screen, int y1, int y2, int x, unsigned int color) {
	int i;
	if ((y1 > _VID_Y) || (y1 < 0) || (y2 > _VID_Y) || (y2 < 0) || 
		(x > _VID_X) || (x < 0)) {
		printf("line draw exception\n");
		exit(1);
	} else {
		for (i = y1; i < y2+1; i++) {
	//		setPixel(screen, color, color, color, x, i);
			setPixel(screen, ((color>>16)&0xff), ((color>>8)&0xff), (color&0xff), x, i);
		}
	}
}


void drawCircle(SDL_Surface *screen, int guy, int sqr_num) {
	int x, y, i, j = 0;
	unsigned int color;
	switch (guy) {
		case _GOODGUY:
			color = _PLAYER1_COLOR;
			break;
		case _BADGUY:
			color = _PLAYER2_COLOR;
			break;
		case _NOGUY:
			color = _BOARD_COLOR;
			break;
		default:
			fprintf(stderr, "Error: status out of bounds error");
			exit(1);
	}
 
	if ((sqr_num < 1) || (sqr_num > 64) || (guy > 2) || (guy < 0)) {
		printf("circle out of bounds error\n");
		exit(1);
	} else {
		for (i = 1; i < 65; i++) {
			if (i == sqr_num) {
				j = i % 8; 		// j = col # (1-7, 0)
				if (j == 0) j = 8; 	// j = (1-8);
				x = 1;
				while (i > 8) { 
					i -= 8; 
					++x; 
				}
				i = x;			// i = row # (1-8)
				break;
			}
		}
		x = (j*_SQR_W) - (_SQR_W/2);
		y = (i*_SQR_W) - (_SQR_W/2);
		for (i = 0; i < _CIRCLE_R*2 ; i++) {
			j = (int)sqrt ((_CIRCLE_R*_CIRCLE_R) - ((_CIRCLE_R - i)*(_CIRCLE_R - i)));
			if (guy == _BADGUY ) {
				setPixel(screen, 0xFF, 0xFF, 0xFF, x-j, y-_CIRCLE_R+i);
				setHLine(screen, x-j+1, x+j-1, y-_CIRCLE_R+i, color);
				setPixel(screen, 0xFF, 0xFF, 0xFF, x+j, y-_CIRCLE_R+i);
			} else if (guy == _GOODGUY) {
				setPixel(screen, 0x77, 0x77, 0x77, x-j, y-_CIRCLE_R+i);
				setPixel(screen, 0x77, 0x77, 0x77, x-j+1, y-_CIRCLE_R+i);
				setHLine(screen, x-j+2, x+j-2, y-_CIRCLE_R+i, color);
				setPixel(screen, 0x77, 0x77, 0x77, x+j, y-_CIRCLE_R+i);
				setPixel(screen, 0x77, 0x77, 0x77, x+j-1, y-_CIRCLE_R+i);
			} 
		}
	}
}	

void updatePeices(SDL_Surface *screen, unsigned char * piece) {
	int i;
	for (i = 0; i < 64; i++) {
		if (piece[i] == 0)
			drawCircle(screen, _NOGUY, i+1);
		else {
			if (piece[i] == 1)
				drawCircle(screen, _GOODGUY, i+1);
			else if (piece[i] == 2)
				drawCircle(screen, _BADGUY, i+1);
			else
				fprintf(stderr, "Error: piece out of bounds\n");
		}
	}
}

void checkMouse(SDL_Event event, unsigned char * pieces) {
	int matchDir;
	char i, j;	
	Uint16 X, Y;
	X = event.button.x;
	Y = event.button.y;

	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			if ( (X > ((i*_SQR_W)+8)) && (X < (((i+1)*_SQR_W)-8)) &&
	 		   (Y > ((j*_SQR_W)+8)) && (Y < (((j+1)*_SQR_W)-8)) ) {
				if ((matchDir = isLegal(j*8+i, pieces)) != _FALSE) {
					flip(j*8+i, pieces, matchDir);
					switchTurn();
				}
			}
		}
	}
}

/* activates a piece on the board.  assumes that all checks have been made */
void flip(int place, unsigned char * piece, int matchDir) {
	printf("match directions = %x\n", matchDir);

	extern int pieces_placed;
	int i, scout = place;
	Uint8 turnOpposite;

	turnOpposite = (_TURN == _PLAYERONE) ? _PLAYERTWO : _PLAYERONE;
	
	/* we know its legal.  isLegal took care of that */

	for (i = 1; i < 256; i<<=1) {
		switch (i & matchDir) {
			case _RDN:
				scout += 9;
				while (piece[scout] == turnOpposite) {
					piece[scout] = _TURN;
					scout += 9;
				}
				break;
			case _DN:
				scout += 8;
				while (piece[scout] == turnOpposite) {
					piece[scout] = _TURN;
					scout += 8;
				}
				break;
			case _LDN:
				scout += 7;
				while (piece[scout] == turnOpposite) {
					piece[scout] = _TURN;
					scout += 7;
				}
				break;
			case 0x08:
				scout += 1;
				while (piece[scout] == turnOpposite) {
					piece[scout] = _TURN;
					scout += 1;
				}
				break;
			case 0x10:
				scout -= 1;
				while (piece[scout] == turnOpposite) {
					piece[scout] = _TURN;
					scout -= 1;
				}
				break;
			case 0x20:
				scout -= 7;
				while (piece[scout] == turnOpposite) {
					piece[scout] = _TURN;
					scout -= 7;
				}
				break;
			case 0x40:
				scout -= 8;
				while (piece[scout] == turnOpposite) {
					piece[scout] = _TURN;
					scout -= 8;
				}
				break;
			case 0x80:
				scout -= 9;
				while (piece[scout] == turnOpposite) {
					piece[scout] = _TURN;
					scout -= 9;
				}
				break;
			default:
				break;
		}
		/* reset the scout */
		scout = place;
	}
			
	/* change square of origin of click */
	piece[place] = _TURN;
	
	++pieces_placed;
}

// place is the square # that the player clicks
// returns either (_FALSE = 0), or the directions that
// matches occur in. 
int isLegal(int place, unsigned char *piece) {
	int i, scout = place, matchDir = 0;
	Uint8 hasNeighbors = _FALSE;
	Uint8 neighborDir = _NONE;
	Uint8 available = 255;
	Uint8 neighborExists = _FALSE;

	Uint8 turnOpposite;
	turnOpposite = (_TURN == _PLAYERONE) ? _PLAYERTWO : _PLAYERONE;

	/* make sure square is empty */
	if (piece[place] != _NOGUY) 
		return _FALSE;

	/* this first set determines where the click ocurred */
	/* there are nine quadrants */
	/* set mask (available) accordingly */
	if (place == 0)  			// top-left
		available &= 0x0b;
	else if ((place > 0) && (place < 7)) 	// top-row
		available &= 0x1f;
	else if (place == 7) 			// top-right
		available &= 0x16;
	else if ((place > 7) && (place < 55)) {
		for (i = 8; i < 49; i+=8) {		// left-edge
			if (place == i) {
				available &= 0x6b;
			}
		}
		for (i = 15; i < 56; i+=8) {		// right-edge
			if (place == i) {
				available &= 0xd6;
			}
		}
		if ((place < 55) && (available == 0))	// center-somewhere
			available &= 0xff;
	}
	else if (place == 56) 			// bottom-left
		available &= 0x68;
	else if ((place > 56) && (place < 63)) 	// bottom-row
		available &= 0xf8;
	else if (place == 63) 			// bottom-right
		available &= 0xd0;

	/* now we know which directions we CAN check */
	/* check for an adjacent piece of opposite type */
	if (available & _RDN) {
		if (piece[place+9] == turnOpposite) {
			neighborDir |= _RDN;
			neighborExists = hasNeighbors = _TRUE;
		}
	} 
	if (available & _DN) {
		if (piece[place+8] == turnOpposite) {
			neighborDir |= _DN;
			neighborExists = hasNeighbors = _TRUE;
		}
	} 
	if (available & _LDN) {
		if (piece[place+7] == turnOpposite) {
			neighborDir |= _LDN;
			neighborExists = hasNeighbors = _TRUE;
		}
	} 
	if (available & _RT) {
		if (piece[place+1] == turnOpposite) {
			neighborDir |= _RT;
			neighborExists = hasNeighbors = _TRUE;
		}
	} 
	if (available & _LFT) {
		if (piece[place-1] == turnOpposite) {
			neighborDir |= _LFT;
			neighborExists = hasNeighbors = _TRUE;
		}
	} 
	if (available & _RUP) {
		if (piece[place-7] == turnOpposite) {
			neighborDir |= _RUP;
			neighborExists = hasNeighbors = _TRUE;
		}
	} 
	if (available & _UP) {
		if (piece[place-8] == turnOpposite) {
			neighborDir |= _UP;
			neighborExists = hasNeighbors = _TRUE;
		}
	} 
	if (available & _LUP) {
		if (piece[place-9] == turnOpposite) {
			neighborDir |= _LUP;
			neighborExists = hasNeighbors = _TRUE;
		}
	}

	/* at this point hasNeighbors is either true or false.  
	* and neighborDir will have a bit set for each direction
	* that has a neighbor of opposite type */

	if (hasNeighbors) {	

		// i is mask.  do one set of checks for each bit
		for (i = 1;  i < 256; i<<=1) {

			/* go 2 the first time */
			switch (neighborDir & i) {
				case _RDN:
					scout += 2*9;
					break;
				case _DN:
					scout += 2*8;
					break;
				case _LDN:
					scout += 2*7;
					break;
				case _RT:
					scout += 2;
					break;
				case _LFT:
					scout -= 2;
					break;
				case _RUP:
					scout -= 2*7;
					break;
				case _UP:
					scout -= 2*8;
					break;
				case _LUP:
					scout -= 2*9;
					break;
				default:
					neighborExists = _FALSE;
					break;
			}

			// continue going in a certain direction one at a time
			while ( (hasNeighbors > 0) && (neighborExists == _TRUE) && (scout >= 0) && (scout < 64) )  {	
				// we've found one of our own again.  in that case turn on
				// the mask bit in matchDir for later returning and leave the
				// while loop, check remaining directions
				if (piece[scout] == _TURN) {
					if (didntCross(place, scout, (neighborDir & i))) 
						matchDir |= i;
					break;
				} else if (piece[scout] == _NONE) {
					neighborExists = _NONE;
					break;
				} else if (piece[scout] == turnOpposite) {
					switch (neighborDir & i) {
						case _RDN:
							scout += 9;
							break;
						case _DN:
							scout += 8;
							break;
						case _LDN:
							scout += 7; 
							break;
						case _RT:
							scout += 1;
							break;
						case _LFT:
							scout -= 1;
							break;
						case _RUP:
							scout -= 7;
							break;
						case _UP:
							scout -= 8;
							break;
						case _LUP:
							scout -= 9;
							break;
						default:
							neighborExists = _FALSE;
							break;
					}//switch
				} else {
					fprintf(stderr, "error: board array exception\n");
					for (i = 0; i<64; i++)
						fprintf(stderr, "%d ", piece[i]);
					fprintf(stderr, "\n");
					exit(1);
				}
			}  //end while
			// reset scout for next go round
			scout = place;
			neighborExists = _TRUE;
		}  //end for
	} else 	//end if
		return _FALSE;
	if (matchDir != 0)
		// we've found a match!!
		return matchDir;
	else
		return _FALSE;
}

void changeColor(SDL_Surface *screen, SDL_Rect *board) {
	int random = (int) (16777000.0*rand()/(RAND_MAX));

	switch (++_COLOR_SCHEME) {
		case 1:
			_PLAYER1_COLOR = (unsigned int) _COLOR_RED;
			_PLAYER2_COLOR = (unsigned int) _COLOR_LBLUE;
			_BORDER_COLOR = (unsigned int) _COLOR_YELLOW;
			break;
		case 2:
			_PLAYER1_COLOR = (unsigned int) _COLOR_GREEN;
			_PLAYER2_COLOR = (unsigned int) _COLOR_YELLOW;
			_BORDER_COLOR = (unsigned int) _COLOR_LGREEN;
			SDL_FillRect(screen, board, (Uint32) _COLOR_LBLUE);
			break;
/* randomish */	case 3:
			_PLAYER1_COLOR = (unsigned int) random;
			printf("p1 = %x\n", random);
			random = (int) (16777000.0*rand()/(RAND_MAX));
			_PLAYER2_COLOR = (unsigned int) random;
			printf("p2 = %x\n", random);
			random = (int) (16777000.0*rand()/(RAND_MAX));
			_BORDER_COLOR = (unsigned int) random;
			random = (int) (16777000.0*rand()/(RAND_MAX));
			SDL_FillRect(screen, board, (Uint32) random);
			printf("B = %x\n", random);
			break;
			
		default:
			_PLAYER1_COLOR = (unsigned int) _COLOR_WHITE;
			_PLAYER2_COLOR = (unsigned int) _COLOR_GREY;
			_BORDER_COLOR = (unsigned int) _COLOR_WHITE;
			SDL_FillRect(screen, board, (Uint32) _COLOR_BLACK);
			_COLOR_SCHEME = 0;
			break;
	}
}

int didntCross(int mclick, int match, int dir) {
	switch (dir) {
		case _RDN:
		case _RT:
		case _RUP:
			if ((match%8) <= (mclick%8))
				return _FALSE;
			break;
		case _LDN:
		case _LFT:
		case _LUP:
			if ((match%8) >= (mclick%8))
				return _FALSE;
			break;
		case _DN:
		case _UP:
		default:
			break;
	}
	return _TRUE;
}

/* args 
-d (0 - 6)	// set depth
-c (1 || 2)	// set computer player number
-p (0, 1, 2)	// set number of players
--help		// gnu style help message
if its 1|2|4|6 ignore it; 3|5|7, check
*/
void checkArgs(int argc, char * argv[]) {
	int i;
	unsigned int getsleep = _FALSE;
	for (i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			if (argv[i][1] == 'd') { 
				++i;
				if ((argv[i][0] > 47) && 
				   (argv[i][0] < 55) && 
				   (argv[i][1] == '\0') ) {
					_MAXDEPTH = argv[i][0] - '0';
					printf("setting depth to %d\n", _MAXDEPTH);
					getsleep = _TRUE; 
				}
				else {
					fprintf(stderr, "not a valid depth\n");
					exit(1);
				}
			}
			if (argv[i][1] == 'c') {
				++i;
				if ((argv[i][0] == 49) && (argv[i][1] == '\0')) {
					_AIPLAYER = _AIPLAYERONE;
					printf("setting computer player to player 1\n");
					getsleep = _TRUE; 
				}
			}
			if (argv[i][1] == '-') {
				if (strcmp(argv[i], "--help") == 0) {
					fprintf(stderr, USAGE);
					exit(0);
				}
			}
			if (argv[i][1] == 'p') {
				if (strcmp(argv[++i], "0") == 0) {
					_AIPLAYER = _AIBOTHPLAYERS;
					printf("Computer plays itself\n");
					getsleep = _TRUE; 
				} else if (strcmp(argv[i], "1") == 0) {
		// dont need anything here, this is already initialized
					printf("Computer is player 2\n");
					getsleep = _TRUE; 
				} else if (strcmp(argv[i], "2") == 0) {
					_AIPLAYER = _NOAI;
					printf("2 player mode\n");
					getsleep = _TRUE; 
				}
			}
		}
	}
	if (getsleep == _TRUE)
		sleep(2);
}

void aiMove(unsigned char * pieces) {
	if ((_AIPLAYER != _NOAI) && (_GAME_STATE == _STATE_PLAY)) {
		if (_AIPLAYER == _AIPLAYERONE) {
			if (_TURN == _PLAYERONE)
				minimaxm(pieces);
		} else if (_AIPLAYER == _AIPLAYERTWO) {
			if (_TURN == _PLAYERTWO)
				minimaxm(pieces);
		} else if (_AIPLAYER == _AIBOTHPLAYERS)
			minimaxm(pieces);
	}
}

void skipTurn(unsigned char * pieces) {
	unsigned char board[64];
	byteCopy(pieces, board);
	
	// see what AI player could do on next turn
	// by playing it against a copy of the board
	switchTurn();
	// minimaxm switch's turn back implicitly
	minimaxm(board);

	if (countPlayer(board) == 0) {
		printf("illegal move: cant skip !\n");
	} else
		switchTurn();
}

int countPlayer(unsigned char * pieces) {
	register int i;
	int total = 0;
	for (i = 0; i < 64; i++) {
		if (pieces[i] == _TURN)
			++total;
	}
	return total;
}

void checkGame(unsigned char * pieces) {
	register int i;
	int p1Pieces, p2Pieces, temp;
	unsigned int 	p1HasLegalMove = _TRUE, 
			p2HasLegalMove = _TRUE;

	for (i = 0; i < 64; i++) {
		if (isLegal(i, pieces) != 0) {
			if (_TURN == _PLAYERONE)
				p1HasLegalMove = _FALSE;
			else
				p2HasLegalMove = _FALSE;
		}
	}
	p1Pieces = countPlayer(pieces);
	switchTurn();
	p2Pieces = countPlayer(pieces);
	switchTurn();
	if (_TURN == _PLAYERTWO) {
		temp = p1Pieces;
		p1Pieces = p2Pieces;
		p2Pieces = temp;
	} 
	// so far we have a count of both players pieces and an 
	// indication of whether the current player has a legal move
	if (p1HasLegalMove == _FALSE)  {
		fprintf(stderr, "Player 1 has no further legal moves.\n");
			if (0) {
				fprintf(stderr, "Player 2 Wins!!\n"); 
				sleep(120);
				exit(0);
			}
	}
	if (p2HasLegalMove == _FALSE) {
		fprintf(stderr, "Player 2 has no further legal moves.\n\nPlayer 1 Wins!!\n"); 
			if (0) {
				fprintf(stderr, "Player 2 Wins!!\n"); 
				sleep(120);
				exit(0);
			}
	}
	if (p1Pieces == 0) {
		fprintf(stderr, "Player 1 has no pieces.\n\nPlayer 2 Wins!!\n"); 
		sleep(120);
		exit(0);
	}
	if (p2Pieces == 0) {
		fprintf(stderr, "Player 2 has no pieces.\n\nPlayer 1 Wins!!\n"); 
		sleep(120);
		exit(0);
	}
}
/*
how do you know when a game is over?

- when a player X has no further moves, he/she can skip until:
	a - they get a move
	b - they lose all their pieces
- a game is decisive if one or the other player has zero pieces
- when there are no further moves for either player, the pieces
	are counted.  he with the most pieces wins.
*/
