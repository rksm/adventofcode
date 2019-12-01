(ns aoc-2019.day01
  (:require [aoc-2019.util :refer [get-input]]
            [clojure.string :as string]))

(defn fuel-for-module [module-mass]
  (-> module-mass
      (/ 3.0)
      Math/floor
      (- 2)))

(comment
  (fuel-for-module 14))

(defn fuel-for-module-and-fuel [module-mass]
  (->> module-mass
       (iterate fuel-for-module)
       (drop 1)
       (take-while pos?)
       (reduce +)))

(comment
 (fuel-for-module-and-fuel 14)
 (fuel-for-module-and-fuel 1969))

(defn part-1 []
  (->> (string/split-lines (get-input 1))
       (map #(Float/parseFloat %))
       (map fuel-for-module)
       (reduce +)))

(defn part-2 []
  (->> (string/split-lines (get-input 1))
       (map #(Float/parseFloat %))
       (map fuel-for-module-and-fuel)
       (reduce +)))
