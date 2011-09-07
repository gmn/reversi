
CC=gcc
EXENAME=reversi
LINK=-lSDL


$(EXENAME): $(EXENAME).c
	$(CC) $(EXENAME).c -Wall -O3 -o $(EXENAME) $(LINK)

clean:
	rm -f $(EXENAME)

all : $(EXENAME)

