set shell := ["bash", "-cu"]

# create the build directory
prerequisites:
    mkdir -p build

# build the project with debug flags
dev: prerequisites
	cd build/ && cmake ".." -DCMAKE_BUILD_TYPE=Debug && make

# build the project
build: prerequisites
	cd build && cmake ".." && make

# run tests
test: build
	cd build && ctest --output-on-failure

# build the documentation using doxygen
docs:
	doxygen Doxyfile

# build and test
all: build test

# Clean the build and docs folders
clean:
	rm -R build

# Rebuild everything from scratch
rebuild: clean all

