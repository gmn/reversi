/* reversi.h */

#define	_COLOR_BLACK	0x000000
#define	_COLOR_WHITE	0xFFFFFF
#define	_COLOR_GREY	0x777777
#define	_COLOR_RED	0x880000
#define _COLOR_GREEN	0x008800
#define _COLOR_BLUE	0x000088
#define _COLOR_LRED	0xcc0000
#define _COLOR_LGREEN	0x00cc00
#define	_COLOR_LBLUE	0x0000cc
#define _COLOR_YELLOW	0xe3e588
#define _COLOR_PURPLE	0x880088

#include "SDL/SDL.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <string.h>

#define _VID_X 		480
#define _VID_Y 		480

/*
#define _VID_X 		720
#define _VID_Y 		720
*/

#define	_SQR_W 		(_VID_X / 8)
#define _COLOR_DEPTH 	16
#define _CIRCLE_R 	((_SQR_W/2) - 8)
#define _GOODGUY 	1
#define _BADGUY 	2
#define _NOGUY 		0
#define _PLAYERONE 	_GOODGUY
#define _PLAYERTWO 	_BADGUY
#define _TRUE 		1
#define _FALSE 		0
#define _AIPLAYERONE	3
#define _AIPLAYERTWO	4
#define _AIBOTHPLAYERS  5
#define _NOAI		6

#define _LUP 	0x80
#define _UP 	0x40
#define _RUP 	0x20
#define _LFT 	0x10
#define _RT 	0x08
#define _LDN 	0x04
#define _DN 	0x02
#define _RDN 	0x01
#define _NONE	0x00

#define _STATE_GAMEOVER	0
#define _STATE_NEWGAME	1
#define _STATE_PLAY	2

#define USAGE "This is SDL Reversi.  It supports one or \
two players.\nYou can even have the computer play itself, although \
that\nmight not be too interesting.\nusage: reversi [-d 0-6][-c 0-2][--help][-p #]\n-d depth of AI recursion\n-c set computer player # [defaults to player 2]\n\
-p # of players [0 is computer plays itself]\n--help -- this message\n"


/* Global Variables and Initializations */
unsigned int 	_TURN, 
		_TURNSAVE,
		_COLOR_SCHEME = 0, 	//default
		_AIPLAYER = _AIPLAYERTWO,
		_MAXDEPTH = 4,
		_GAME_STATE = _STATE_PLAY,
	 	_BOARD_COLOR = (unsigned int) _COLOR_BLACK, 
		_PLAYER1_COLOR = (unsigned int) _COLOR_WHITE,
		_PLAYER2_COLOR = (unsigned int) _COLOR_GREY,
		_BORDER_COLOR = (unsigned int) _COLOR_WHITE; 

int pieces_placed = 4;

void setPixel(SDL_Surface*, Uint8 R, Uint8 G, Uint8 B, int, int);
void setHLine(SDL_Surface*, int, int, int, unsigned int);
void setVLine(SDL_Surface*, int, int, int, unsigned int);
void drawBoard(SDL_Surface *);
void drawCircle(SDL_Surface *screen, int, int);
void updatePeices(SDL_Surface *, unsigned char *);
void checkMouse(SDL_Event, unsigned char *);
void flip(int, unsigned char *, int); 
int isLegal(int, unsigned char *);
void changeColor(SDL_Surface *, SDL_Rect *);
int didntCross(int, int, int);
void checkArgs(int, char **);
void aiMove(unsigned char *);
void skipTurn(unsigned char *);
int countPlayer(unsigned char *);
void checkGame(unsigned char *);
