;;;; adventofcode-2018.asd

;; (ql:quickload ':adventofcode-2018)

(asdf:defsystem #:adventofcode-2018
  :description "Describe adventofcode-2018 here"
  :author "Your Name <your.name@example.com>"
  :license  "Specify license here"
  :version "0.0.1"
  :serial t
  :depends-on ("drakma" "local-time" "cl-arrows")
  :components ((:file "package")
               (:file "adventofcode-util")
               (:file "adventofcode-2018-01")
               (:file "adventofcode-2018-05")))
