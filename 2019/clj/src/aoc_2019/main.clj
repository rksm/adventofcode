(ns aoc-2019.main
  (:require [aoc-2019.day02 :as day02]))

(defn -main []
  (time
   (println (day02/solve-1)))
  (time
   (println (day02/solve-2))))
