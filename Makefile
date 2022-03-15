INSTALL = /usr/bin/install
INSTALL_PROGRAM = $(INSTALL)
INSTALL_DATA = $(INSTALL) -m 644

UNAME := $(shell uname)
ifeq ($(UNAME),Darwin)
	# macOS
	BUILD_DIRS = 	Otter-build		OtterUtil-build		OtterServer-build 						OtterUI-build
	INSTALL_DIRS = 	Otter-install	OtterUtil-install						OtterCLI-install	OtterUI-install
	CLEAN_DIRS = 	Otter-clean		OtterUtil-clean		OtterServer-clean						OtterUI-clean
else ifeq ($(UNAME),Linux)
	# Linux / Raspberry Pi
	BUILD_DIRS = 	Otter-build							OtterServer-build
	INSTALL_DIRS = 										OtterServer-install
	CLEAN_DIRS = 	Otter-clean							OtterServer-clean
endif

MKFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
CURRENT_DIR := $(dir $(MKFILE_PATH))
PROFILE_FILE := /etc/profile
ZPROFILE_FILE := ~/.zprofile


all: $(BUILD_DIRS)
$(BUILD_DIRS):
	@echo "--- BUILD: $(@:%-build=%) ---"
	@-$(MAKE) -C $(@:%-build=%)
	@echo "\n"

install: $(INSTALL_DIRS) setOtterHome
$(INSTALL_DIRS):
ifneq ($(shell id -u), 0)
	@echo "root needed to install -> try 'sudo make install' or simply 'sudo !!'"
else
	@echo "--- INSTALL: $(@:%-install=%) ---"
	@-$(MAKE) -C $(@:%-install=%) install
	@echo "\n"
endif

setOtterHome:
ifneq ($(shell id -u), 0)
	@echo "root needed to set OTTER_HOME -> try 'sudo make install' or simply 'sudo !!'"
else
	@echo "--- SETTING OTTER HOME ---"
ifeq ($(UNAME),Darwin)
	@# macOS
	@sed -i '' '/export OTTER_HOME/d' $(ZPROFILE_FILE)
	@sudo sh -c "echo 'export OTTER_HOME=$(CURRENT_DIR)' >> $(ZPROFILE_FILE)"
else ifeq ($(UNAME),Linux)
	@# Linux / Raspberry Pi
	@sed -i '/export OTTER_HOME/d' $(PROFILE_FILE)
	@sudo sh -c "echo 'export OTTER_HOME=$(CURRENT_DIR)' >> $(PROFILE_FILE)"
endif
	@echo "DONE\n"
endif


clean: $(CLEAN_DIRS)
$(CLEAN_DIRS):
	@echo "--- CLEAN: $(@:%-clean=%) ---"
	@-$(MAKE) -C $(@:%-clean=%) clean
	@echo "\n"


.PHONY: subdirs $(DIRS)
.PHONY: subdirs $(BUILD_DIRS)
.PHONY: subdirs $(INSTALL_DIRS)
.PHONY: subdirs $(CLEAN_DIRS)
.PHONY: all install clean setOtterHome
