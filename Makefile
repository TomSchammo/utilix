
all: configure

configure:
	cmake -S . -B build -D CMAKE_BUILD_TYPE=Release

install: configure
	cd build && sudo make install

clean:
	rm -rf build
