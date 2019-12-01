;;;; adventofcode-2018-01.lisp

(in-package #:adventofcode-2018-01)

(defparameter *input* (adventofcode-util:get-input 1))

(defun day-1 ()
  (let ((input *input*))
    (loop for ea in (uiop:split-string input :separator '(#\Newline))
          while (> (length ea) 0)
          for num = (parse-integer ea)
          sum num into sum
          finally (return sum))))

;; (day-1)

(defun day-1-2 ()
  (let* ((input *input*)
         (numbers (loop for ea in (uiop:split-string input :separator '(#\Newline))
                        while (> (length ea) 0)
                        collect (parse-integer ea)))
         (circular-numbers (let ((numbers (copy-seq numbers)))
                             (setf (cdr (last numbers)) numbers))))

    (loop with max fixnum = (expt 10 6)
          with seen = (make-hash-table :test 'eq)
          for i fixnum = 0 then (+ i 1)
          for num in circular-numbers
          sum num into sum
          when (> i max)
            do (return nil)
          if (gethash sum seen)
            do (return sum)
          else
            do (setf (gethash sum seen) t))))


;; (time (day-1-2))

