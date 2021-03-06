(*Dawid Szczyr*)

(*2*)

type 'a llist = LNil | LCons of 'a * (unit -> 'a llist);;
type 'a lBT = LEmpty | LNode of 'a * ('a lBT Lazy.t) * ('a lBT Lazy.t);;

let foo xs ll =
	let rec aux iter xs ll =
		match (xs,ll) with
		  ([],_)	->	ll
		| (_, LNil) ->	LNil
		| (y::ys, LCons(_, rest)) when y = iter -> aux (iter+1) ys (rest())
		| (xs, LCons(a, rest)) -> LCons(a,  function () -> aux (iter+1) xs (rest()))
	in aux 0 (List.sort compare xs) ll;; 


let y = ltake (5, foo [1;2] (LCons(1, function () -> LCons(2, function () -> LCons(3, function () -> LCons(4, function () -> LCons(5, function () -> LNil)))))));;
let z = ltake (5, foo [1;6] (LCons(1, function () -> LCons(2, function () -> LCons(3, function () -> LCons(4, function () -> LCons(5, function () -> LNil)))))));;
let z = ltake (5, foo [1;4;6] (LCons(1, function () -> LCons(2, function () -> LCons(3, function () -> LCons(4, function () -> LCons(5, function () -> LNil)))))));;

(*3*)
let splitLL xs =
	let rec aux (a1, a2) xs =
		match xs with
		  LNil -> (a1, a2)
		| LCons(y, ys) when y >= 0 -> aux (LCons(y, function () -> a1), a2) (ys())
		| LCons(y, ys) -> aux (a1, LCons(y, function () -> a2)) (ys())
	in aux (LNil, LNil) xs;;

let s = splitLL (LCons(1, function () -> LCons(-2, function () -> LCons(3, function () -> LCons(-4, function () -> LCons(5, function () -> LNil))))));;

(*4*)

type 'a ltree = LTNil | LNode of 'a * (unit -> 'a ltree) * (unit -> 'a ltree);;

let rec insert tree el =
	match tree with
	  LTNil -> LNode(el, (function () -> LTNil), (function () -> LTNil))
	| LNode(x, left, right) when el >= x -> LNode(x, left, (function () -> insert (right()) el))
	| LNode(x, left, right) -> LNode(x, (function () -> insert (left()) el), right);;

let toLBTS  list =
	let rec aux list tree = 
		match list with 
		  LNil -> tree
		| LCons(el, rest) -> aux (rest()) (insert tree el)
	in aux list LTNil;;

let rec flatten tree =
	match tree with
	  LTNil -> []
	| LNode(x, left, right) -> (flatten (left())) @ [x] @  (flatten (right()));;

let s = flatten (toLBTS (LCons(1, function () -> LCons(-2, function () -> LCons(3, function () -> LCons(-4, function () -> LCons(5, function () -> LNil)))))));;











