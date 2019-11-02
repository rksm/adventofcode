export ADVENT_OF_CODE_SESSION = $(shell cat ../session.txt)

slime:
	sbcl \
		--eval '(push *default-pathname-defaults* asdf:*central-registry*)' \
		--eval '(ql:quickload :adventofcode-2018)' \
		--eval '(swank-loader:init)' \
		--eval '(swank:create-server :port 4005 :style :spawn :dont-close t)'
