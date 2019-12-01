(ns aoc-2019.util
  (:require [clojure.java.io :as io]
            [clojure.string :as string])
  (:import [java.net.http HttpClient HttpRequest HttpResponse HttpResponse$BodyHandlers]))

(def session (string/trim (slurp (io/resource "session.txt"))))

(defn get-input [day]
  (let [session (format "session=%s; Path=/; Domain=adventofcode.com" session)
        url (format "https://adventofcode.com/2019/day/%s/input" day)
        uri (-> url io/as-url .toURI)
        req (-> (HttpRequest/newBuilder)
                (.uri uri)
                (.header "cookie" session)
                .build)
        client (HttpClient/newHttpClient)
        res (.send client req (HttpResponse$BodyHandlers/ofString))]
    (.body res)))
