(*Dawid Szczyrk*)
(*1*)

module type QUEUE_FUN =
sig
(* This module implements queues (FIFOs)in a functional way. *)
type 'a t
(* The type of queues containing elements of type ['a]. *)
exception Empty of string
(* Raised when [first] is applied to an empty queue. *)
val create: unit -> 'a t
val enqueue: 'a * 'a t -> 'a t
(* [enqueue (x,q)] adds the element [x] at the end of queue [q]. *)
val dequeue: 'a t -> 'a t
(* [dequeue q] removes the first element in queue [q] *)        
val first: 'a t -> 'a
(* [first q] returns the first element in queue [q] without removing  
it from the queue, or raises [Empty] if the queue is empty.*) 
val isEmpty: 'a t -> bool
(* [isEmpty q] returns [true] if queue [q] is empty, 
otherwise returns [false]. 
*)
end;;


module Queue : QUEUE_FUN= 
  struct
    type 'a t = EmptyQueue | Enqueue of 'a * 'a t
    exception Empty of string 
    let create() = EmptyQueue
    let enqueue(e, q) = Enqueue(e, q)
    let dequeue =  function x -> 
			    let rec aux = function
				Enqueue(_, EmptyQueue) -> EmptyQueue
			      | Enqueue(e, q) -> Enqueue(e, aux q)
			      | EmptyQueue -> EmptyQueue
			    in aux x
    let first = function x ->
			 let rec aux = function
			     Enqueue(e, EmptyQueue) -> e
			   | Enqueue(_, q) -> aux q
			   | EmptyQueue -> raise(Empty "I see what you did there")
			 in aux x  
    let isEmpty = function EmptyQueue -> true   | Enqueue _ -> false
  end;;


module QueueList :QUEUE_FUN= 
  struct
    type 'a t = { mutable l : 'a list }
    exception Empty of string 
    let create() = { l = [] }
    let enqueue(e, q) = q.l <- e:: q.l; q
    let dequeue q = (q.l <-
		      let rec aux = function
			  [] -> []
			| [e] -> []
			| (e::r) -> e::(aux r)
		      in aux q.l); q
    let first q = let rec aux = function
		      [] -> raise(Empty "I see what you did there")
		    | [e] -> e
		    | (e::r) -> aux r
		  in aux q.l
    let isEmpty q = q.l = []
  end;;

module QueueDoubleList : QUEUE_FUN= 
  struct
    type 'a t = { mutable f : 'a list; mutable s : 'a list }
    exception Empty of string 
    let create() = { f = []; s = [] }
    let enqueue(e, q) = (if q.f = [] then
			  q.f <- [e]
			else
			  q.s <- e :: q.s); q
    let dequeue q = 
      match q.f with
	[] -> q
      | [e] -> q.f <- List.rev q.s; q.s <- [];q
      | x::xs -> q.f <- xs; q
    let first q = let rec aux = function
		      [] -> raise(Empty "I see what you did there")
		    | x::xs -> x
		  in aux q.f
    let isEmpty q = q.f = []
  end;;

module Test (Que : QUEUE_FUN) = 
struct 
  type 'a t = Que
  let test1 = Que.isEmpty (Que.enqueue (1,(Que.create()))) = false
  let test2 = Que.isEmpty (Que.create()) = true
  let test3 = Que.dequeue (Que.enqueue(1,Que.enqueue(2,(Que.create())))) = 
		Que.enqueue(1,Que.dequeue(Que.enqueue(2,Que.create())))
  let test4 = Que.dequeue(Que.enqueue(1,Que.create()))         = Que.create()
  let test5 = Que.dequeue (Que.create())                     = Que.create()
  let test6 = Que.first (Que.enqueue(1,Que.enqueue(2,Que.create())))   = Que.first(Que.enqueue(2,Que.create()))
  let test7 = Que.first (Que.enqueue(1,Que.create()))           = 1                     
end;;

module Q1 = Test(Queue);;

Q1.test1;;
Q1.test2;;
Q1.test3;;
Q1.test4;;
Q1.test5;;
Q1.test6;;
Q1.test7;;

module Q2 = Test(QueueList);;

Q2.test1;;
Q2.test2;;
Q2.test3;;
Q2.test4;;
Q2.test5;;
Q2.test6;;
Q2.test7;;

module Q3 = Test(QueueDoubleList);;

Q3.test1;;
Q3.test2;;
Q3.test3;;
Q3.test4;;
Q3.test5;;
Q3.test6;;
Q3.test7;;







