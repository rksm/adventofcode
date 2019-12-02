(ns aoc-2019.day02
  (:require [aoc-2019.util :refer [get-input]]
            [clojure.string :as string]))

(defn compute [instructions]
  (loop [instructions instructions n 0]
    (let [op (nth instructions n)]
      (if (= op 99)
        instructions
        (let [[_ b c d] (subvec instructions n)
              val-1 (nth instructions b)
              val-2 (nth instructions c)
              step 4
              op-fn (case op
                      1 +
                      2 *
                      (throw (Error. (format "unknown op %s at position %s" op n))))]
          (recur (assoc instructions d (op-fn val-1 val-2)) (+ n step)))))))

(defonce input (->> (string/split (get-input 2) #",")
                    (map string/trim)
                    (map #(Integer/parseInt %))
                    (into [])))

(defn fixup-input [input noun verb]
  (assoc input 1 noun 2 verb))

(defn solve-1 []
  (-> input
      (fixup-input 12 2)
      compute first))

(defn solve-2 []
  (let [[a b] (first
               (for [noun (range 0 100)
                     verb (range 0 100)
                     :let [result (-> input (fixup-input noun verb) compute first)]
                     :when (= 19690720 result)]
                 [noun verb]))]
    (+ (* 100 a) b)))

