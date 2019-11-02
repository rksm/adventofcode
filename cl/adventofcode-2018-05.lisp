(in-package #:adventofcode-2018-05)

(defparameter *input* (string-trim '(#\Newline) (adventofcode-util:get-input 5)))

(defun day-5 (input)
  (declare (optimize (speed 3) (safety 0)))
  (time
   (loop with right = input
         with left = nil
         with n-chars = (length right)
         for a = (first left)
         for b = (first right)
         count t into n
         if (and (not (equal a b))
                 (equalp a b))
           do (progn
                (pop left)
                (pop right))
         else
           do (progn
                (push b left)
                (pop right))
         when (null right)
           do (progn
                (setf right left)
                (setf left nil)
                (format t "~a vs ~a~%" n-chars (length right))
                (if (= n-chars (length right))
                    (return (format nil "~{~a~}" right))
                    (setf n-chars (length right)))))))

;; (setf *input* "dabAcCaCBAcCcaDA")
;; dabAcCaCBAcCcaDA  The first 'cC' is removed.
;; dabAaCBAcCcaDA    This creates 'Aa', which is removed.
;; dabCBAcCcaDA      Either 'cC' or 'Cc' are removed (the result is the same).
;; dabCBAcaDA        No further actions can be taken.
;; dabCBAcaDA

;; (setf x (list 1 2 3 4 5 6 7 8))

;; (time (day-5 (coerce *input* 'list)))
;; (length (day-5 (coerce *input* 'list)))
;; (setf t1 (get-universal-time))
;; (setf t2 (get-universal-time))


;; dabAcCaCBAcCcaDA
;; Removing all A/a units produces dbcCCBcCcD. Fully reacting this polymer produces dbCBcD, which has length 6.
;; Removing all B/b units produces daAcCaCAcCcaDA. Fully reacting this polymer produces daCAcaDA, which has length 8.
;; Removing all C/c units produces dabAaBAaDA. Fully reacting this polymer produces daDA, which has length 4.
;; Removing all D/d units produces abAcCaCBAcCcaA. Fully reacting this polymer produces abCBAc, which has length 6.

(defun day-5-2 (input)
  (declare (optimize (speed 3) (safety 0)))
  (let* ((inputs (loop for i from 65 to 90
                       for a = (code-char i)
                       for b = (char-downcase a)
                       collect (cons a (->> input
                                            (remove a)
                                            (remove b)))))
         (counts (loop for (char . input) in inputs
                       ;; do (print input)
                       collect (cons char (length (day-5 input)))))
         (min (loop for (char . count) in counts
                    minimize count)))
    (loop for (char . count) in counts
          when (= count min)
            do (return (cons char count)))))

;; (time (day-5-2 (coerce *input* 'list)))
