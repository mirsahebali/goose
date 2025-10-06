.PHONY: all configure install build run

run: build start-cumin

all: install configure build start


install:
	conan install . --output-folder=build  --build=missing --settings=build_type=Debug

configure:
	cmake -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug

build:
	cmake --build build

start:
	./build/goose

tests:
	./build/goose-tests

start-cumin:
	./build/cumin


clean: 
	rm -rf build CMakeUserPresets.json deps


