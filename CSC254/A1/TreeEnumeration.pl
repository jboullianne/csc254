/* Jean-Marc Boullianne */
/* jboullia@u.rochester.edu */
/* CSC 254: Fall 2015 */
/* A1: Tree Enumeration */


/* Checks how close the location is relative to the top of the tree (Keeps us from escaping outside of the root node ) */
depthCheck(NodesLeft, Depth, Tree) :- Depth > 1, moveUp(NodesLeft, Depth, Tree).
depthCheck(_, Depth, _) :- Depth =< 1.


/* moves the location down, adds an opening parenthesis, and subtracts remaining nodes by one */
addNode(NodesLeft, Depth, OldTree) :- append(OldTree, ['('], NewTree),
										 X is NodesLeft - 1,
										 Y is Depth + 1, 
										 buildTree(X, Y, NewTree).

/* moves the location up and adds a closing parenthesis */
moveUp(NodesLeft, Depth, OldTree) :- append(OldTree, [')'], NewTree),
										Y is Depth - 1,
										 buildTree(NodesLeft, Y, NewTree).

/* Decision Clauses ~ Decides whether or not we move up or down, or are finished */
buildTree(NodesLeft, Depth, Tree) :- NodesLeft is 0, Depth is 0, printTree(Tree), nl, !. 
buildTree(NodesLeft, Depth, Tree) :- NodesLeft is 0, Depth > 0, moveUp(NodesLeft, Depth, Tree), !.
buildTree(NodesLeft, Depth, Tree) :- addNode(NodesLeft, Depth, Tree), depthCheck(NodesLeft, Depth, Tree), !.

/* Prints out the tree List*/
printTree([]).
printTree([X|Y]) :- write(X), printTree(Y).

/* Main Clause Called to Enumerate all possible trees */
enumerateTrees(X) :- buildTree(X, 0, []).