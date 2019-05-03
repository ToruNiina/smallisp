# smallisp

c++17 implementation of a small lisp

## example

```lisp
; comment
(define (increment x) (+ x 1))
(let a 0)
(+ (increment a) (+ 2 4 6) 3 5)
(println a)
```

```console
$ ./smallisp sample.sl
(increment x)
a
21
0
nil
```
