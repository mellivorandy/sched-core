# SPDX-License-Identifier: MIT

NPROC := $(shell \
	command -v nproc >/dev/null 2>&1 && nproc || sysctl -n hw.ncpu)

MAKEFLAGS += -j$(NPROC)

CC       ?= gcc
CPPFLAGS += -Iinclude
CFLAGS   ?= -std=c11 -Wall -Wextra
LDLIBS   := -lm
BUILD    ?= build
TARGET   ?= sched-core

ifdef DEBUG
CPPFLAGS += -DRTS_DEBUG
endif

ifdef NOCOLOR
CPPFLAGS += -DRTS_NO_COLOR
endif

SRC_DIRS ?= src src/core src/utils src/queue src/sched
SRCS     := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
OBJS     := $(SRCS:%.c=$(BUILD)/%.o)
DEPS     := $(OBJS:.o=.d)

all: $(BUILD)/$(TARGET)
	@echo
	@echo -e "\033[1;32mBuild completed: \033[0m" $(TARGET)

$(BUILD)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD)/$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

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
