all:
	gcc -I /usr/include/SDL2 -D_REENTRANT -L /usr/lib -Wl,-rpath,/usr/lib -o main.out main.c -lSDL2 -lSDL2_image -lSDL2_ttf -lm -g -Wall -std=c11