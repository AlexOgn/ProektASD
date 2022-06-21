CC = gcc
FILES = terrain.c pathing.c libattopng.c main.c 
TARGET = generator

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(FILES) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f $(OBJFILES) $(TARGET) *~