.PHONY: run clean fresh rtags

CPP_SOURCE_FILES := $(wildcard *.cpp)
SOURCE_FILES := $(CPP_SOURCE_FILES) $(wildcard *.hpp)

CPP := clang
FLAGS := -lstdc++ -std=c++2a -Ofast
# FLAGS := -lstdc++ -std=c++2a -g

adventofcode: $(SOURCE_FILES) Makefile
	$(CPP) $(FLAGS) $(CPP_SOURCE_FILES) -o $@

run: adventofcode
	./adventofcode

debug: adventofcode
	gdb ./adventofcode

profile: adventofcode
	@-rm callgrind.out*
	valgrind --tool=callgrind ./adventofcode
	kcachegrind ./callgrind.out.*

clean:
	@-bash -c 'find . -type f -exec file {} \; | grep executable | cut -d':' -f1 | xargs rm'
	@-rm callgrind.out*

rtags: clean
	bear $(MAKE) run
	rc -J .

fresh: clean
	touch *.cpp
	touch *.hpp
