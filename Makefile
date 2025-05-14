CC = gcc
TARGET = poc
SRC = poc.c

# Default flags
BASE_CFLAGS = -I./libpng-1.6.18/build/include
BASE_LDFLAGS = -L./libpng-1.6.18/build/lib -lpng -lz -lm

# Conditionally add -DLEAK_MEM if LEAK_MEM=1 is passed
ifeq ($(LEAK_MEM),1)
    CFLAGS = $(BASE_CFLAGS) -DLEAK_MEM 
	LDFLAGS = $(BASE_LDFLAGS)

else
    CFLAGS = $(BASE_CFLAGS) -fsanitize=address -O0
	LDFLAGS = $(BASE_LDFLAGS) -fsanitize=address

endif


all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET)