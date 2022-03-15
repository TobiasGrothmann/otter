SCRIPT := "otter.py"
SCRIPT_TARGET_NAME := "otter"
SCRIPT_TARGET_DIR := "/usr/local/bin"

all:

.PHONY: install

install:
ifneq ($(shell id -u), 0)
	@echo "root needed to install -> try 'sudo make install' or simply 'sudo !!'"
else
	@cp -f $(SCRIPT) $(SCRIPT_TARGET_DIR)/$(SCRIPT_TARGET_NAME)
	@echo "INSTALLED"
endif