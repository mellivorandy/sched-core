# SPDX-License-Identifier: MIT

CC       ?= gcc
CFLAGS   ?= -std=c11 -Wall -Wextra -O2 -Iinclude
BUILD    ?= build
TARGET   ?= sched-core

SRC_DIRS ?= src/core src/utils src/queue src/sched
SRCS     := main.c $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
OBJS     := $(SRCS:%.c=$(BUILD)/%.o)
DEPS     := $(OBJS:.o=.d)

all: $(BUILD)/$(TARGET)
	@echo
	@echo "Build completed: $(TARGET)"

$(BUILD)/$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)

debug: CFLAGS := -std=c11 -Wall -Wextra -g -O0 -Iinclude
debug: clean all

release: CFLAGS := -std=c11 -Wall -Wextra -O3 -Iinclude
release: clean all

run: all
	./$(BUILD)/$(TARGET) EDF input/test1.txt

clean:
	rm -rf $(BUILD)

rebuild: clean all

.PHONY: all clean debug release run rebuild
