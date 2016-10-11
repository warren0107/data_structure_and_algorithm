ifeq ("$(origin T)", "command line")
    TARGET = $(T)
endif

TARGET?=avl

CROSS_COMPILE=
CC:=$(CROSS_COMPILE)gcc
CFLAGS+=-g -O0
LDFLAGS:=
objs:=avl.o rotate.o


all:$(TARGET)

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(objs)
	$(CC) $(LDFLAGS) $^ -o $@

clean:
	rm -f *.o
	rm $(TARGET)
.PHONY: all
