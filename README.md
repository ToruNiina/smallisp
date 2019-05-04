# smallisp

c++17 implementation of a small lisp

## example

```lisp
; fizzbuzz.sl
(define (print-fizzbuzz n)
    (if (= 0 (% n 15))
        (println "FizzBuzz")
        (if (= 0 (% n 3))
            (println "Fizz")
            (if (= 0 (% n 5))
                (println "Buzz")
                (println n)
                )
            )
        )
    )

(define (fizzbuzz iter last)
    (while (< iter last)
           (print-fizzbuzz (let iter (+ iter 1)))
        )
    )

(fizzbuzz 1 30)
```

```console
$ ./smallisp fizzbuzz.sl
(print-fizzbuzz n)
(fizzbuzz iter last)
2
Fizz
4
Buzz
Fizz
7
8
Fizz
Buzz
11
Fizz
13
14
FizzBuzz
16
17
Fizz
19
Buzz
Fizz
22
23
Fizz
Buzz
26
Fizz
28
29
FizzBuzz
nil
```

## spec

- comment
  - `; ... <newline>`
- symbol
  - `[a-zA-Z_-]+`
- integer
  - `(+|-)?[0-9]+`
- string
  - `"[^"]+"`
- list
  - `(<object> ...)`

## builtin-objects

- `+`
  - `(+ 1 2 3)`: sumup integers
- `-`
  - `(- 100)`: make integer negative
  - `(- 1 2 3)`: subtract tail (`2` and `3`) from head (`1`)
- `%`
  - `(% 10 3)`: calculate modulo 10 % 3
- `=`
  - `(= 1 1)`: return `T` if objects are the same. otherwise, returns `nil`
- `<`
  - `(< 1 2)`: return `T` if head < tail. otherwise, returns `nil`
- `let`
  - `(let a 1)`: bind object to symbol. returns the object bound.
- `define`
  - `(define (fn x) (+ x 1))`: define function
- `if`
  - `(if (cond) (then) (else))`: evaluates `then` if `cond`, otherwise evaluates `else`.
- `while`
  - `(while (cond) (body))`: evaluates `body` until `cond` becomes `nil`
- `println`
  - `(println expr)`: prints `expr`.
