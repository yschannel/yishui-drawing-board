CC = g++
CFLAGS = -O2 -Wall -std=c++11
LIBS = -lraylib -lopengl32 -lgdi32 -lwinmm
SOURCES = draw_board.cpp PAGE.cpp FANCTION.cpp
TARGET = drawing_board.exe

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES) $(LIBS)

clean:
	rm -f $(TARGET)

.PHONY: clean