/* wen.h */

/* this is the king wen sequence from the i ching.  For our purposes,
 * we will store each hexagram in the low six bits of a char .   I ching
 * hexagrams are always read from the bottom upwards.  So we will store
 * bits, in LSB style: right-most bit = lowest most line */

char kingwen[] = {077, 000, 021, 042, 027, 072, 002, 020, 
		  067, 073, 007, 070, 075, 057, 004, 010,
		  031, 046, 003, 060, 051, 045, 040, 001,
		  071, 047, 041, 036, 022, 055, 034, 016,
		  074, 017, 050, 005, 065, 053, 024, 012,
		  043, 061, 037, 076, 030, 006, 032, 026,
		  035, 056, 011, 044, 064, 013, 015, 054,
		  066, 033, 062, 023, 063, 014, 025, 052};

static int index = 0;

char getwen(char * kingwen) {
	if (index > 63) {
		index = 0;
	} else
		++index;
	return kingwen[index];
}

