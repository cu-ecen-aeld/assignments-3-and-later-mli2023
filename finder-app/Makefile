# PATH is updated in ~/.bashrc
# to use cross compiler, make CROSS_COMPILE=aarch64-none-linux-gnu-
CROSS_COMPILE=
CC:=$(CROSS_COMPILE)gcc
SRC := writer.c
TARGET = writer
OBJS := $(SRC:.c=.o)

all: $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) -o $(TARGET) $(LDFLAGS)

clean:
	-rm -f *.o $(TARGET) *.elf *.map
