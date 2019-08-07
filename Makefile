DEFAULT_CFLAGS = --target=x86_64-w64-mingw -static-libgcc -std=c11 -O3
SDL_CFLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image  -lSDL2_ttf  
RELEASE_CFLAGS =  -mconsole -mwindows -Wl,-subsystem,windows 

cc = clang

debug: main.c  
	$(cc) main.c  -o main_d.exe $(SDL_CFLAGS) $(DEFAULT_CFLAGS)  && ./main_d

release: main.c  mine.o
	$(cc) main.c mine.o -o 扫雷v0.1.exe $(SDL_CFLAGS) $(DEFAULT_CFLAGS)  $(RELEASE_CFLAGS) && ./扫雷v0.1

single-release: single_edition.c mine.o
	$(cc) single_edition.c mine.o -o Minesweeper.exe $(SDL_CFLAGS) $(DEFAULT_CFLAGS)  $(RELEASE_CFLAGS) && ./Minesweeper

.PHONY : clean
clean:
	-rm   *.exe