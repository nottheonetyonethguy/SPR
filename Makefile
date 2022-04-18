all:
	gcc -I src/include -L src/lib -o main main.c -lmingw32 -lSLD2main -lSDL2 -lSDL2_image -lSDL2_ttf -lm