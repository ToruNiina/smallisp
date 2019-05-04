(define (print-fizzbuzz n)
    (if (= 0 (% n 3)) (if (= 0 (% n 5)) (println "FizzBuzz") (println "Fizz")) (if (= 0 (% n 5)) (println "Buzz") (println n)))
    )

(define (fizzbuzz-aux n m tmp)
    (if (= n m) tmp (fizzbuzz-aux n (+ m 1) (print-fizzbuzz m)))
    )

(define (fizzbuzz n)
    (fizzbuzz-aux (+ n 1) 1 nil)
    )

(fizzbuzz 30)
