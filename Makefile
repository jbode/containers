CC=gcc
CFLAGS=-std=c17 -Wall -Werror -Wextra -g -DSIZE=$(size) -MP -MD
LDLIBS=-lm

SRCS=node.c list.c main.c
OBJS=$(SRCS:%.c=%.o)

TARGET=listtest

.PHONY: $(TARGET) clean all

#-include $(SRCS:%.c=%.d)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(CFLAGS) $(LDFLAGS) $(LDLIBS) $?

clean:
	rm -rf $(TARGET) $(OBJS) *.d

all:
	$(TARGET)


