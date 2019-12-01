export ADVENT_OF_CODE_SESSION = $(shell cat ../session.txt)

day5: src/day5.rs
	cargo run 5

all: src/*
	for n in 1 2 3 4 5 9 ; do \
		cargo run $$n; \
	done
