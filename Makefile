.PHONY: all configure install build run

run: build start

all: install configure build start

configure:
	cmake -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug

install:
	mkdir -p deps/imgui_backends
	conan install . --output-folder=build  --build=missing --settings=build_type=Debug

build:
	cmake --build build

start:
	./build/goose

tests:
	./build/goose-tests

clean: 
	rm -rf build CMakeUserPresets.json deps


