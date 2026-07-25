CC = clang

CFLAGS = -Wall -Wextra
INCLUDES = $(shell sdl2-config --cflags)
LIBS = $(shell sdl2-config --libs) -lSDL2_mixer

TARGET = fake_keyboard

all:
	$(CC) $(CFLAGS) fake_keyboard.c -o $(TARGET) $(INCLUDES) $(LIBS)

clean:
	rm -f $(TARGET)
