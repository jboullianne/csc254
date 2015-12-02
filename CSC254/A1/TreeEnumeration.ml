(*
Jean-Marc Boullianne
jboullia@u.rochester.edu
CSC 254 Fall 2015
Assignment 01 : Tree Enumeration 
*)


(*  Recursive Tree Building Function *)
let rec build_tree nLeft depth tree =
	if nLeft == 0 && depth == 0 then print_string (tree ^ "\n")
	else if nLeft == 0 && depth != 0 then build_tree nLeft (depth - 1) (tree ^ ")")
	else begin build_tree (nLeft - 1) (depth + 1) (tree ^ "(");
		if depth > 1 then build_tree nLeft (depth - 1) (tree ^ ")");
	end;;

(* Starts the tree building routine with command line args *)
let main () =
	print_string "\n";
	build_tree (int_of_string Sys.argv.(1)) 0 "";
	print_string "\n";;

main ();;