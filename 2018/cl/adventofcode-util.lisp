(in-package #:adventofcode-util)

(defun get-input (day)
  (let ((cookies (make-instance 'drakma:cookie-jar))
        (url (format nil "https://adventofcode.com/2018/day/~a/input" day))
        (session-cookie (make-instance 'drakma:cookie
                                       :name "session"
                                       :value (uiop:getenv "ADVENT_OF_CODE_SESSION")
                                       :domain "adventofcode.com"
                                       :expires (local-time:timestamp-to-universal (local-time:parse-timestring "2029-10-25T11:53:39.137Z")))))
    (rk-slime-scope:capture)
    (setf (drakma:cookie-jar-cookies cookies)
          (list session-cookie))
    (drakma:http-request url :COOKIE-JAR cookies)))

;; (get-input 5)

