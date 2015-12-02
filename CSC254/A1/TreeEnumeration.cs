/*
Jean-Marc Boullianne
jboullia@u.rochester.edu
CSC 254: Fall 2015
A1: Tree Enumeration
*/

using System;
public class TreeEnumeration
{
	public static void Main(string [] args)
	{

		// Parses command line argument (Number of Nodes).
		if (args.Length == 0)
		{
		    Console.WriteLine("Please enter the amount of nodes as a command line argument...");
		}
		else{
			buildTree(Int32.Parse(args[0]), 0, ""); //Starts the tree finding function
		}
	}

	/* ~~~~~ Recursively Enumerates each tree ~~~~~

		Takes three arguments:
			Number of nodes left to place in the tree
			Current Depth within the tree (Location)
			Current Tree's Structure (Paranthetical Tree Structure)

		Decides which action to take in tree. (Move up or Add Node).

	*/
	public static void buildTree(int nodesLeft, int depth, string tree){

		if(nodesLeft == 0 && depth == 0){
			Console.Write(tree);
			Console.Write("\n");
		}
		else if(nodesLeft == 0 && depth != 0){
			moveUp(nodesLeft, depth, tree);
		}
		else{
			addNode(nodesLeft, depth, tree);
			if(depth > 1){
				moveUp(nodesLeft, depth, tree);
			}
		}
	}

	//Method to add node to tree
	public static void addNode(int nodesLeft, int depth, string tree){
		tree += "(";
		buildTree(nodesLeft - 1, depth + 1, tree);
	}
	
	//Method to move up in tree
	public static void moveUp(int nodesLeft, int depth, string tree){
		tree += ")";
		buildTree(nodesLeft, depth - 1, tree);
	}
}