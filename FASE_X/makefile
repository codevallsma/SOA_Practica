CC=gcc
CFLAGS=-Wall -Wextra -ggdb
LDFLAGS=
SOURCES=src/main.c src/utils/fileManagement.c src/utils/screenOutput.c src/utils/Utils.c src/EXT2/ext2.c src/FAT16/fat16.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=shooter

all: $(SOURCES) $(EXECUTABLE)
	rm -f src/*.o src/*~

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@  $(LDFLAGS)

.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f src/*.o shooter
