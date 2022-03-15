CC := g++
HPP_FILES := $(shell find . -type f -path "./OtterUtil/*.h*")

BUILD_DIR := ../build
OTTER_LIB_DIR := $(BUILD_DIR)/otterUtil
LIB_OTTERUTIL := $(OTTER_LIB_DIR)/libOtterUtil.a

INSTALL_LIB_PATH := /usr/local/lib/
INSTALL_INCLUDE_PATH := /usr/local/include/


all:
	xcodebuild -project OtterUtil.xcodeproj -quiet -scheme "OtterUtil" -destination arch="arm64"

install:
ifneq ($(shell id -u), 0)
	@echo "root needed to install -> try 'sudo make install' or simply 'sudo !!'"
else
	@rm -rf $(INSTALL_INCLUDE_PATH)/OtterUtil
	@rsync -a --relative --delete $(HPP_FILES) $(INSTALL_INCLUDE_PATH)
	@mkdir -p $(INSTALL_LIB_PATH)
	@cp $(LIB_OTTERUTIL) $(INSTALL_LIB_PATH)
	@echo "INSTALLED"
endif

clean:
	@$(RM) $(LIB_OTTERUTIL)