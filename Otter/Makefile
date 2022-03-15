CC := g++
INCLUDES := -isystem/opt/homebrew/include -IOtter
CPP_FILES := $(shell find . -type f -path './Otter/*.cpp')
HPP_FILES := $(shell find . -type f -path "./Otter/*.hpp")
O_FILES := $(patsubst %.cpp,%.o, $(CPP_FILES))
DEP_FILES := $(patsubst %.cpp,%.d, $(CPP_FILES))
GCH_FILES := $(shell find . -type f -path "./Otter/*.gch")

WARNINGS := -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
            -Wwrite-strings -Wmissing-declarations -Wredundant-decls \
            -Winline -Wno-long-long -Wconversion
OPTIMIZATION := -O3
CFLAGS := -g -std=c++20 $(WARNINGS) $(OPTIMIZATION)

PCH_HPP := Otter/stl.hpp
PCH_OUT := Otter/stl.hpp.gch

OTTER_HPP := Otter/Otter.hpp
OTTER_OUT := Otter/Otter.hpp.gch

BUILD_DIR := ../build
OTTER_LIB_DIR := $(BUILD_DIR)/otter
LIB_OTTER := $(OTTER_LIB_DIR)/libotter.a

INSTALL_LIB_PATH := /usr/local/lib/
INSTALL_INCLUDE_PATH := /usr/local/include/


all: $(LIB_OTTER)
	@echo "DONE"

$(LIB_OTTER): $(O_FILES) $(PCH_OUT)
	@mkdir -p $(OTTER_LIB_DIR)
	@ar -rs $(LIB_OTTER) $(O_FILES)

%.o: %.cpp Makefile $(PCH_OUT)
	@$(CC) $(CFLAGS) -MD -MP -c $< -o $@ $(INCLUDES) -include $(PCH_HPP)

$(PCH_OUT): $(PCH_HPP) Makefile
	@$(CC) $(CFLAGS) -o $(PCH_OUT) $<

clean:
	@$(RM) $(wildcard $(O_FILES) $(DEP_FILES) $(GCH_FILES) $(LIB_OTTER))

install:
ifneq ($(shell id -u), 0)
	@echo "root needed to install -> try 'sudo make install' or simply 'sudo !!'"
else
	@rsync -a --relative --delete $(HPP_FILES) $(INSTALL_INCLUDE_PATH)
	@mkdir -p $(INSTALL_LIB_PATH)
	@cp $(LIB_OTTER) $(INSTALL_LIB_PATH)
	@echo "INSTALLED"
endif
