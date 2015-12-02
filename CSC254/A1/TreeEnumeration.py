# Jean-Marc Boullianne
# jboullia@u.rochester.edu
# CSC 254: Fall 2015
# A1: Tree Enumeration

import sys

# Main tree building method
def buildTree(nodes):
	decide(nodes, 0, "")

# Decides whether to move up or down the tree
def decide(nodesLeft, depth, tree):
	if nodesLeft == 0 and depth == 0:
		print (tree)
	elif nodesLeft == 0 and depth != 0:
		moveUp(nodesLeft, depth, tree)
	else:
		addNode(nodesLeft, depth, tree)
		if depth > 1:
			moveUp(nodesLeft, depth, tree)

# Method for moving down tree
def addNode(nodesLeft, depth, tree):
	tree += "("
	decide(nodesLeft - 1, depth + 1, tree)

# Method for moving up the tree
def moveUp(nodesLeft, depth, tree):
	tree += ")"
	decide(nodesLeft, depth - 1, tree)

# Main method that is called on program start
buildTree(int(sys.argv[1])) # arg is # of nodes to start off with
