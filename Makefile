# SPDX-License-Identifier: MIT

MAKEFLAGS += -j$(shell nproc)

CC       ?= gcc
CFLAGS   ?= -std=c11 -Wall -Wextra -Iinclude
BUILD    ?= build
TARGET   ?= sched-core

SRC_DIRS ?= src/core src/utils src/queue src/sched
SRCS     := main.c $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
OBJS     := $(SRCS:%.c=$(BUILD)/%.o)
DEPS     := $(OBJS:.o=.d)

all: $(BUILD)/$(TARGET)
	@echo
	@echo -e "\033[1;32mBuild completed: \033[0m" $(TARGET)

$(BUILD)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD)/$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

-include $(DEPS)

debug:
	$(MAKE) clean
	$(MAKE) CFLAGS="$(CFLAGS) -O0" all

release:
	$(MAKE) clean
	$(MAKE) CFLAGS="$(CFLAGS) -O3" all

run: all
	./$(BUILD)/$(TARGET) EDF input/test1.txt

clean:
	$(RM) -r $(BUILD) $(TARGET)

rebuild:
	$(MAKE) clean
	$(MAKE) all

.PHONY: all clean debug release run rebuild
