.PHONY: run clean fresh rtags

SOURCE_FILES := $(wildcard *.cpp) $(wildcard *.hpp)
CPP := clang
# CPP := clang -lstdc++ -std=c++2a -Ofast
# FLAGS := -lstdc++ -std=c++2a -Ofast
FLAGS := -lstdc++ -std=c++2a -g
UTILS := util.cpp IntComputer.cpp

day_09: $(SOURCE_FILES)
	$(CPP) $(FLAGS) $@.cpp $(UTILS) -o $@

day_05: $(SOURCE_FILES)
	$(CPP) $(FLAGS) $@.cpp $(UTILS) -o $@

day_02: $(SOURCE_FILES)
	$(CPP) $(FLAGS) $@.cpp $(UTILS) -o $@

run: day_05
	./day_05

debug: day_05
	gdb ./day_05

profile: day_05
	@-rm callgrind.out*
	valgrind --tool=callgrind ./day_05
	kcachegrind ./callgrind.out.*

clean:
	@-bash -c 'find . -type f -exec file {} \; | grep executable | cut -d':' -f1 | xargs rm'

rtags: clean
	bear $(MAKE) run
	rc -J .

fresh: clean
	touch *.cpp
	touch *.hpp
