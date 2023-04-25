SHELL:=/bin/bash -O extglob
PROJECT=block-memory-allocator
MAKEFLAGS=--no-print-directory

.PHONY: test
test: build/test
	./build/tests/$(PROJECT)-tests

.PHONY: test/valgrind
test/valgrind: build/test
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all build/tests/$(PROJECT)-tests

.PHONY: build
build:
	mkdir -p build && \
	cd build && \
	cmake .. && \
	$(MAKE)

.PHONY: build/test
build/test: clean
	mkdir -p build && \
	cd build && \
	cmake -DBMA_BUILD_TESTS=ON .. && \
	$(MAKE)

.PHONY: clean
clean:
	rm -rf build/!(_deps)

.PHONY: clean/full
clean/full:
	rm -rf build
