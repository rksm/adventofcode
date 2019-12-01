.PHONY: nrepl clean

# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
# repl / dev

export ADVENT_OF_CODE_SESSION = $(shell cat ../session.txt)

nrepl:
	clojure -R:dev:nrepl:test -C:nrepl:test -m advent-of-clj-2018.nrepl

# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

clean:
	rm -rf target .cpcache bin
