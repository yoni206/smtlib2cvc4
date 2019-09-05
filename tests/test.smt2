(define-fun A1 ((x Int) (y Int)) Int (+ x y))
(define-fun A2 ((a (_ BitVec 4))) Bool (= (bvadd a a) a))
(define-fun B1 ((a (_ BitVec 4))) Bool (= ((_ extract 1 1) a) ((_ extract 1 1) a) ))
(define-fun b2 ((x Int) (y Int)) Int (+ x 0))
