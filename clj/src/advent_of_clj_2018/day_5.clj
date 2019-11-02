(ns advent-of-clj-2018.day-5
  (:require [clojure.java.io :as io]
            [clojure.string :as string])
  (:import [java.net.http HttpClient HttpRequest HttpResponse HttpResponse$BodyHandlers]))

(defn get-input [day]
  (let [session (format "session=%s; Path=/; Domain=adventofcode.com" (System/getenv "ADVENT_OF_CODE_SESSION"))
        url (format "https://adventofcode.com/2018/day/%s/input" day)
        uri (-> url io/as-url .toURI)
        req (-> (HttpRequest/newBuilder)
                (.uri uri)
                (.header "cookie" session)
                .build)
        client (HttpClient/newHttpClient)
        res (.send client req (HttpResponse$BodyHandlers/ofString))]
    (.body res)))

(defn reactive? [a b]
  (and (not (.equals a b))
       (.equals (java.lang.Character/toLowerCase a)
                (java.lang.Character/toLowerCase b))))

(defn solve [^String input]
  (loop [left nil right (seq input)]
    (let [[^Character a & rest-l] left
          [^Character b & rest-r] right]
      (cond
        ;; when no elements are in the right list we are done
        (nil? b) (->> left reverse (string/join ""))
        ;; empty left list: just put a single element from the right in it
        (nil? a) (recur (conj rest-l b) rest-r)
        ;; if both chars "react" remove them
        (reactive? a b) (recur rest-l rest-r)
        ;; put char from left to right
        :else (recur (conj left b) rest-r)))))

(defn solve-2 [^String input]
  (let [right (transient (into [] (reverse input)))
        last (nth right (dec (count right)))
        left (transient [last])]
    (while (> (count right) 0)
      (let [a (nth left (dec (count left)))
            b (nth right (dec (count right)))]
        (pop! right)
        (if (reactive? a b)
          (pop! left)
          (conj! left b))))
    (string/join "" (drop 1 (persistent! left)))))

(defn -main []
  (let [input (string/trim (get-input 5))
        ;; [input "dabAcCaCBAcCcaDA"]
        ]
    (println "solve")
    (dotimes [_ 10] (time (count (solve input))))
    (println "solve 2")
    (dotimes [_ 10] (time (count (solve-2 input))))))
