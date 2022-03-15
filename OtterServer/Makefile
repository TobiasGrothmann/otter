CC := g++
WARNINGS := -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
            -Wwrite-strings -Wmissing-declarations -Wredundant-decls \
            -Winline -Wno-long-long -Wconversion
OPTIMIZATION := -O3
CFLAGS := -g -std=c++20 $(WARNINGS) $(OPTIMIZATION)

BUILD_DIR := ../build
OTTER_LIB_DIR := $(BUILD_DIR)/otter
BUILD_BIN_DIR := $(BUILD_DIR)/otterServer
OTTER_SERVER := $(BUILD_BIN_DIR)/otterServer

INCLUDES := -I../Otter/Otter -Isrc
LFLAGS := -L$(OTTER_LIB_DIR) -lotter -L/usr/local/lib/ -lboost_system -lpthread -lboost_filesystem

UNAME := $(shell uname)
ifeq ($(UNAME),Darwin)
	# macOS
	INCLUDES := $(INCLUDES) -isystem/opt/homebrew/include -isystem/opt/homebrew/Cellar/boost/1.76.0/include
	CFLAGS := $(CFLAGS) -DWITH_GPIO=0
	LFLAGS := $(LFLAGS) -L/opt/homebrew/lib -lboost_thread-mt
else ifeq ($(UNAME),Linux)
	# Linux / Raspberry Pi
	INCLUDES := $(INCLUDES)
	CFLAGS := $(CFLAGS) -DWITH_GPIO=1
	LFLAGS := $(LFLAGS) -lboost_thread
endif

MAIN_FILE := src/main.cpp
CPP_FILES := $(shell find . -type f -path "./src/*.cpp")
HPP_FILES := $(shell find . -type f -path "./src/*.hpp")
O_FILES := $(patsubst %.cpp,%.o,$(CPP_FILES))
DEP_FILES := $(patsubst %.cpp,%.d,$(CPP_FILES))
GCH_FILES := $(shell find . -type f -path "./src/*.gch")

PCH_HPP := src/stl.hpp
PCH_OUT := src/stl.hpp.gch

MKFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
INSTALL_PATH := /usr/local/bin/otterServer

SYSTEMD_SOURCE := installResource/systemd
SYSTEMD_TARGET := /etc/systemd/system/otterServer.service


all: $(OTTER_SERVER)
	@echo "DONE"

$(OTTER_SERVER): $(PCH_OUT) $(O_FILES)
	@mkdir -p $(BUILD_BIN_DIR)
	@$(CC) $(CFLAGS) -o $(OTTER_SERVER) $(INCLUDES) $(O_FILES) $(LFLAGS)

%.o: %.cpp $(PCH_OUT)
	@$(CC) $(CFLAGS) $(INCLUDES) -MD -MP -c $< -o $@ -include $(PCH_HPP)

$(PCH_OUT): $(PCH_HPP) Makefile
	@$(CC) $(CFLAGS) -o $(PCH_OUT) $< $(INCLUDES)

clean:
	@$(RM) $(wildcard $(O_FILES) $(DEP_FILES) $(GCH_FILES) $(OTTER_SERVER))

install:
ifneq ($(UNAME),Linux)
	@echo "Only installable on Raspberry-Pi"
else
ifneq ($(shell id -u), 0)
	@echo "root needed to install -> try 'sudo make install' or simply 'sudo !!'"
else
	@#install otterServer bin
	@rm -f $(INSTALL_PATH)
	@cp -f $(OTTER_SERVER) $(INSTALL_PATH)
	@#install systemd autostart
	@cp -f $(SYSTEMD_SOURCE) $(SYSTEMD_TARGET)
	@systemctl daemon-reload
	@systemctl enable otterServer
	@echo "INSTALLED"
endif
endif
