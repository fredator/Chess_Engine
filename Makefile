CC = g++

#-g adds debugging info to executable
#-Wall turns on most compiler warnings
CFLAGS = -g -Wall
TARGET = chess.sh

all: $(TARGET)

$(TARGET): main.cpp
	$(CC) $(CFLAGS) -o $(TARGET) main.cpp

clean:
	rm $(TARGET)
