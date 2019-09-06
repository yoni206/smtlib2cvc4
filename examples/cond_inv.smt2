(define-fun inv_bvsge_bvadd ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvsub t s))
(define-fun inv_bvsge_bvand ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) #b0111)
(define-fun inv_bvsge_bvashr0 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) #b0111)
(define-fun inv_bvsge_bvashr1 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvand s #b1000))
(define-fun inv_bvsge_bvlshr0 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvshl t s))
(define-fun inv_bvsge_bvlshr1 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvlshr s (bvlshr #b0111 (bvsub #b1000 #b0111))))
(define-fun inv_bvsge_bvneg ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvneg t))
(define-fun inv_bvsge_bvnot ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) #b1000)
(define-fun inv_bvsge_bvor ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) t)
(define-fun inv_bvsge_bvshl0 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvlshr #b0111 s))
(define-fun inv_bvsge_bvudiv0 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvnot (bvshl s (bvor s (bvadd s s)))))
(define-fun inv_bvsge_bvurem0 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) t)
(define-fun inv_bvsge_bvurem1 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvsub (bvor s #b1000) (bvand t #b0111)))
(define-fun inv_bvsgt_bvadd ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvsub #b0111 s))
(define-fun inv_bvsgt_bvand ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) #b0111)
(define-fun inv_bvsgt_bvashr0 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) #b0111)
(define-fun inv_bvsgt_bvashr1 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvand s #b1000))
(define-fun inv_bvsgt_bvlshr0 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvshl #b0111 s))
(define-fun inv_bvsgt_bvlshr1 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvlshr s (bvlshr #b0111 (bvsub #b1000 #b0111))))
(define-fun inv_bvsgt_bvneg ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvnot t))
(define-fun inv_bvsgt_bvnot ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) #b1000)
(define-fun inv_bvsgt_bvor ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) #b0111)
(define-fun inv_bvsgt_bvshl0 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvlshr #b0111 s))
(define-fun inv_bvsgt_bvudiv0 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvnot (bvshl s (bvor s (bvadd s s)))))
(define-fun inv_bvsgt_bvurem0 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvneg (bvnot t)))
(define-fun inv_bvsle_bvadd ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvsub t s))
(define-fun inv_bvsle_bvand ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) t)
(define-fun inv_bvsle_bvashr0 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) #b1000)
(define-fun inv_bvsle_bvashr1 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvnot (bvor s #b0111)))
(define-fun inv_bvsle_bvlshr0 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) t)
(define-fun inv_bvsle_bvlshr1 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvnot (bvor s #b0111)))
(define-fun inv_bvsle_bvneg ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) #b1000)
(define-fun inv_bvsle_bvnot ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) #b0111)
(define-fun inv_bvsle_bvor ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) #b1000)
(define-fun inv_bvsle_bvshl0 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvlshr t s))
(define-fun inv_bvsle_bvudiv0 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) t)
(define-fun inv_bvsle_bvudiv1 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvlshr s (bvlshr #b0111 (bvsub #b1000 #b0111))))
(define-fun inv_bvsle_bvurem0 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvand t #b1000))
(define-fun inv_bvsle_bvurem1 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvsub s t))
(define-fun inv_bvslt_bvadd ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvsub #b1000 s))
(define-fun inv_bvslt_bvand ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) #b1000)
(define-fun inv_bvslt_bvashr0 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) #b1000)
(define-fun inv_bvslt_bvashr1 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvnot (bvor s #b0111)))
(define-fun inv_bvslt_bvlshr0 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvshl #b1000 s))
(define-fun inv_bvslt_bvlshr1 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvnot (bvor s #b0111)))
(define-fun inv_bvslt_bvneg ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) #b1000)
(define-fun inv_bvslt_bvnot ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) #b0111)
(define-fun inv_bvslt_bvor ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) #b1000)
(define-fun inv_bvslt_bvshl0 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvlshr #b1000 s))
(define-fun inv_bvslt_bvudiv0 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvnot (bvneg t)))
(define-fun inv_bvslt_bvudiv1 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvlshr s (bvlshr #b0111 (bvsub #b1000 #b0111))))
(define-fun inv_bvslt_bvurem0 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvshl #b1000 (bvshl t t)))
(define-fun inv_bvslt_bvurem1 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) t)
(define-fun inv_bvuge_bvadd ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvnot s))
(define-fun inv_bvuge_bvand ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) s)
(define-fun inv_bvuge_bvashr0 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvnot #b0000))
(define-fun inv_bvuge_bvashr1 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvand s #b1000))
(define-fun inv_bvuge_bvlshr0 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvnot s))
(define-fun inv_bvuge_bvlshr1 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) #b0000)
(define-fun inv_bvuge_bvneg ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvneg t))
(define-fun inv_bvuge_bvnot ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) #b0000)
(define-fun inv_bvuge_bvor ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) t)
(define-fun inv_bvuge_bvshl0 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvnot #b0000))
(define-fun inv_bvuge_bvudiv0 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvnot #b0000))
(define-fun inv_bvuge_bvudiv1 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) #b0000)
(define-fun inv_bvuge_bvurem0 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) t)
(define-fun inv_bvuge_bvurem1 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) #b0000)
(define-fun inv_bvugt_bvadd ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvnot s))
(define-fun inv_bvugt_bvand ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) s)
(define-fun inv_bvugt_bvashr0 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvnot #b0000))
(define-fun inv_bvugt_bvashr1 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvand s #b1000))
(define-fun inv_bvugt_bvlshr0 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvnot s))
(define-fun inv_bvugt_bvlshr1 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) #b0000)
(define-fun inv_bvugt_bvneg ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvnot t))
(define-fun inv_bvugt_bvnot ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) #b0000)
(define-fun inv_bvugt_bvor ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvnot s))
(define-fun inv_bvugt_bvshl0 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvnot #b0000))
(define-fun inv_bvugt_bvudiv0 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvnot #b0000))
(define-fun inv_bvugt_bvudiv1 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) #b0000)
(define-fun inv_bvugt_bvurem0 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvnot (bvneg s)))
(define-fun inv_bvugt_bvurem1 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) #b0000)
(define-fun inv_bvule_bvadd ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvneg s))
(define-fun inv_bvule_bvand ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) t)
(define-fun inv_bvule_bvashr0 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) #b0000)
(define-fun inv_bvule_bvashr1 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvnot (bvor s #b0111)))
(define-fun inv_bvule_bvlshr0 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) s)
(define-fun inv_bvule_bvlshr1 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) s)
(define-fun inv_bvule_bvmul ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) #b0000)
(define-fun inv_bvule_bvneg ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) #b0000)
(define-fun inv_bvule_bvnot ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvnot t))
(define-fun inv_bvule_bvor ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) s)
(define-fun inv_bvule_bvshl0 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) #b0000)
(define-fun inv_bvule_bvshl1 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) #b1000)
(define-fun inv_bvule_bvudiv0 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) t)
(define-fun inv_bvule_bvudiv1 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvnot #b0000))
(define-fun inv_bvule_bvurem0 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) s)
(define-fun inv_bvule_bvurem1 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) s)
(define-fun inv_bvult_bvadd ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvneg s))
(define-fun inv_bvult_bvand ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) #b0000)
(define-fun inv_bvult_bvashr0 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) #b0000)
(define-fun inv_bvult_bvashr1 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvnot (bvor s #b0111)))
(define-fun inv_bvult_bvlshr0 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) s)
(define-fun inv_bvult_bvlshr1 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) s)
(define-fun inv_bvult_bvmul ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) #b0000)
(define-fun inv_bvult_bvneg ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) #b0000)
(define-fun inv_bvult_bvnot ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvneg t))
(define-fun inv_bvult_bvor ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) s)
(define-fun inv_bvult_bvshl0 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) #b0000)
(define-fun inv_bvult_bvshl1 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) #b1000)
(define-fun inv_bvult_bvudiv0 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) #b0000)
(define-fun inv_bvult_bvudiv1 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvnot #b0000))
(define-fun inv_bvult_bvurem0 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) s)
(define-fun inv_bvult_bvurem1 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) s)
(define-fun inv_eq_bvadd ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvsub t s))
(define-fun inv_eq_bvand ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) t)
(define-fun inv_eq_bvashr0 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvshl t (bvlshr (bvshl s s) s)))
(define-fun inv_eq_bvlshr0 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvshl t s))
(define-fun inv_eq_bvneg ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvneg t))
(define-fun inv_eq_bvnot ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvnot t))
(define-fun inv_eq_bvor ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) t)
(define-fun inv_eq_bvshl0 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvlshr t s))
(define-fun inv_eq_bvurem0 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) t)
(define-fun inv_eq_bvurem1 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvsub s t))
(define-fun inv_ne_bvadd ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvnot (bvadd s t)))
(define-fun inv_ne_bvand ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvnot t))
(define-fun inv_ne_bvashr0 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvnot t))
(define-fun inv_ne_bvashr1 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvlshr t (bvsub s t)))
(define-fun inv_ne_bvlshr0 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvshl #b1000 t))
(define-fun inv_ne_bvlshr1 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvneg t))
(define-fun inv_ne_bvmul ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvshl #b0111 t))
(define-fun inv_ne_bvneg ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvnot t))
(define-fun inv_ne_bvnot ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) t)
(define-fun inv_ne_bvor ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvnot t))
(define-fun inv_ne_bvshl0 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvshl #b0111 t))
(define-fun inv_ne_bvshl1 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) t)
(define-fun inv_ne_bvudiv0 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvlshr s t))
(define-fun inv_ne_bvudiv1 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvand t #b1000))
(define-fun inv_ne_bvurem0 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) (bvneg (bvnot t)))
(define-fun inv_ne_bvurem1 ((s (_ BitVec 4)) (t (_ BitVec 4))) (_ BitVec 4) t)
