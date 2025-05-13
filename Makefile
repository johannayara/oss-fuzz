CC = gcc
CFLAGS = -I./libpng-1.6.18/build/include
LDFLAGS = -L./libpng-1.6.18/build/lib -lpng -lz -lm
TARGET = poc
SRC = poc.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET)
