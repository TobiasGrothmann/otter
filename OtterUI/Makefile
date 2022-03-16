CC := g++

BUILD_DIR := ../build
OTTER_LIB_DIR := $(BUILD_DIR)/otter
BIN_DIR = $(BUILD_DIR)/otterUI
EXE_NAME = otterUI
EXE = $(BUILD_DIR)/$(EXE_NAME)/$(EXE_NAME)
IMGUI_DIR = imgui
SOURCES = main_standalone.mm
SOURCES += $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_demo.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp
SOURCES += $(IMGUI_DIR)/backends/imgui_impl_sdl.cpp $(IMGUI_DIR)/backends/imgui_impl_metal.mm
CPP_FILES = $(shell find . -type f -path "./src/*.cpp")
SOURCES += $(CPP_FILES)
OBJS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))

LIBS = -framework Metal -framework MetalKit -framework Cocoa -framework IOKit -framework CoreVideo -framework QuartzCore
LIBS += `sdl2-config --libs`
LIBS += -L$(OTTER_LIB_DIR) -lotter

WARNINGS := -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
            -Wwrite-strings -Wmissing-declarations -Wredundant-decls \
            -Winline -Wno-long-long -Wconversion
OPTIMIZATION := -O3
CCFLAGS = -g -std=c++20 -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends -I../Otter/Otter $(WARNINGS) $(OPTIMIZATION)
CCFLAGS += `sdl2-config --cflags`
CCFLAGS += -Wall -Wformat
CFLAGS = $(CCFLAGS)

INSTALL_DIR := /usr/local/bin


all: $(EXE)
	@echo "DONE"

%.o: %.cpp
	@$(CC) $(CCFLAGS) -c -o $@ $<

%.o: $(IMGUI_DIR)/%.cpp
	@$(CC) $(CCFLAGS) -c -o $@ $<

%.o: $(IMGUI_DIR)/backends/%.cpp
	@$(CC) $(CCFLAGS) -c -o $@ $<

%.o: src/%.cpp
	@$(CC) $(CCFLAGS) -c -o $@ $<

%.o: %.mm
	@$(CC) $(CCFLAGS) -ObjC++ -fobjc-weak -fobjc-arc -c -o $@ $<

%.o: $(IMGUI_DIR)/backends/%.mm
	@$(CC) $(CCFLAGS) -ObjC++ -fobjc-weak -fobjc-arc -c -o $@ $<

$(EXE): $(OBJS)
	@mkdir -p $(BIN_DIR)
	@$(CC) -o $@ $^ $(CCFLAGS) $(LIBS)

clean:
	@rm -f $(EXE) $(OBJS)
	@rm -rf $(BIN_DIR)

install:
	xcodebuild install -project OtterUI.xcodeproj -quiet -scheme "OtterUI" -destination arch="arm64"
ifneq ($(shell id -u), 0)
	@echo "root needed to install -> try 'sudo make install' or simply 'sudo !!'"
else
	@mkdir -p $(INSTALL_DIR)
	@rm -f $(INSTALL_DIR)/$(EXE_NAME)
	@cp -f $(EXE) $(INSTALL_DIR)
	@echo "INSTALLED"
endif
