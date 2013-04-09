
all : main

main: main.c utils/GL_utilities.c utils/VectorUtils2.c utils/loadobj.c utils/LoadTGA2.c utils/generate_terrain2.c
	gcc -Wall -std=c99 -o main -DGL_GLEXT_PROTOTYPES main.c utils/GL_utilities.c utils/VectorUtils2.c utils/loadobj.c utils/LoadTGA2.c utils/generate_terrain2.c -lGL -lglut -lm
clean :
	rm main
