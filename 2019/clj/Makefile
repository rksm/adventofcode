.PHONY: nrepl clean run

# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
# repl / dev

nrepl:
	clojure -R:dev:nrepl:test -C:nrepl:test -m aoc-2019.nrepl

run:
	clojure -m aoc-2019.main

# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

clean:
	rm -rf target .cpcache bin
