BUILD_DIR = build
SRC_DIR = src
SOURCES = $(shell find $(SRC_DIR)/ -name "*.c")
TARGET = keylogger
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
CLIB=-lpcap -lpthread
CC=gcc -g

default: $(TARGET)

$(OBJECTS) : $(patsubst $(BUILD_DIR)/%.o,$(SRC_DIR)/%.c,$@)
	mkdir -p $(@D)
	$(CC) -c -o $@ $(CFLAGS) $(patsubst $(BUILD_DIR)/%.o,$(SRC_DIR)/%.c,$@)

$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGET).elf $(CFLAGS) $^ $(CLIB)

clean:
	rm -rf build $(TARGET)

run: 
	./$(TARGET)

all: clean $(TARGET) run

.PHONY: default clean