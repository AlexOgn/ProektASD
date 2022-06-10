CC = gcc
FILES = terrain.c pathing.c main.c
TARGET = asdf

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(FILES) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f $(OBJFILES) $(TARGET) *~