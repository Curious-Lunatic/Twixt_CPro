CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2
TARGET = twixt
SRCS = main.c board.c
OBJS = $(SRCS:.c=.o)
HEADERS = board.h defination.h

all: $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) 

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	 ./$(TARGET)

clean: 
	rm -f $(OBJS) $(TARGET)
