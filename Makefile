
all: configure

.PHONY: help
help:
	@echo "Available targets:"
	@echo "  clean       - Remove build artifacts"
	@echo "  install     - Install the library"
	@echo "  help        - Display this page"

configure:
	cmake -S . -B build -D CMAKE_BUILD_TYPE=Release

install: configure
	cd build && sudo make install

clean:
	rm -rf build
