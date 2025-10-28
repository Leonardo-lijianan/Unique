MAKEFLAGS += --no-print-directory
MAKE := mingw32-make
CD := cd

.PHONY: build
build:
	$(CD) UCM && $(MAKE) build
	$(CD) UVM && $(MAKE) build
	@echo " build done!"

.PHONY: test
test:
	@$(CD) TEST && $(MAKE) build

.PHONY: test-cls
test-cls:
	@$(CD) TEST && $(MAKE) clean
	@echo " clean done!"

.PHONY: clean
clean:
	@$(CD) UCM && $(MAKE) clean
	@$(CD) UVM && $(MAKE) clean
	@echo " clean done!"