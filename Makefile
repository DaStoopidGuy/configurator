BUILD_DIR := build/
TARGET := $(BUILD_DIR)/main
SRCS := main.c

CC := gcc
INCFLAGS :=
CFLAGS := -Wall -Wextra -ggdb $(INCFLAGS) -fsanitize=address
LDFLAGS :=

$(TARGET): $(SRCS)
	mkdir -p $(BUILD_DIR)
	$(CC) $^ $(CFLAGS) -o $@ $(LDFLAGS)

.PHONY: run debug clean
run: $(TARGET)
	./$<

debug: $(TARGET)
	gf2 ./$<

val: $(TARGET)
	valgrind ./$<

clean:
	rm $(BUILD_DIR) -rf
