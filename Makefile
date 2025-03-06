BUILD_DIR := build/
TARGET := $(BUILD_DIR)/main
SRCS := main.c

CC := gcc
INCFLAGS :=
CFLAGS := -Wall -Wextra -ggdb $(INCFLAGS) -fsanitize=address
LDFLAGS :=

$(TARGET): $(SRCS) configurator.h stinky.h
	mkdir -p $(BUILD_DIR)
	$(CC) $(SRCS) $(CFLAGS) -o $@ $(LDFLAGS)

.PHONY: run debug clean
run: $(TARGET)
	./$<

debug: $(TARGET)
	gf2 ./$<

val: $(TARGET)
	valgrind ./$<

clean:
	rm $(BUILD_DIR) -rf
