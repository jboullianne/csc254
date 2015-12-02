/*
Jean-Marc Boullianne
jboullia@u.rochester.edu
CSC 254: Fall 2015
Assignment #2
*/

#include <unordered_map>
#include <iostream>
#include <fstream>
#include <limits>
#include <cstdlib>
#include "node.h"


using namespace std;

/*
Variable Table
	-Used an unordered map for fast lookup of values
*/
unordered_map <string, int> variableTable; //Holds all variables being saved by code

void interpret(Node*, int);
Node* process_tree(Node*);
Node* run_procedure(Node*);
int eval(Node*);


//Starts the interpreting routine
void interpret(Node* tree, int numVariables){
	cout <<"\n~~~~~~~ RUN PROGRAM HERE (INTERPRETER) ~~~~~~~" << endl;
	
	//Continue processing the tree until no procedures are left
	while(tree != NULL){
		tree = process_tree(tree);
	}
}

//Runs the procedure at the root of the tree
Node* process_tree(Node* tree){
	switch(tree->type){
		case t_procedure:
			return run_procedure(tree);
		default: //Should Never reach this
			break;
	}
	return NULL;
}

//Runs instruction at root of tree (read, write, if, while, :=)
Node* run_procedure(Node* tree){
	if(tree->image == "read"){
		int tmp;
		cin >> tmp;
		variableTable[tree->child[0]->image] = tmp;
		return tree->child[3];
	}else if(tree->image == ":="){
		string varID = tree->child[0]->image;
		variableTable[varID] = eval(tree->child[1]);
		return tree->child[3];
	}else if(tree->image == "write"){
		cout << eval(tree->child[0]) << endl;
		return tree->child[3];
	}else if(tree->image == "#"){
		cout << tree->child[0]->image << endl;
		return tree->child[3];
	}else if(tree->image == "if"){
		if(eval(tree->child[0])){
			Node* tmp = tree->child[1];
			while(tmp != NULL){
				tmp = process_tree(tmp);
			}
		}
		return tree->child[3];
	}else if(tree->image == "while"){
		while(eval(tree->child[0])){
			Node* tmp = tree->child[1];
			while(tmp != NULL){
				tmp = process_tree(tmp);
			}
		}
		return tree->child[3];
	}else{
		cout << "This isn't supposed to happen...";
	}
	return NULL;
}

/*
	Evaluates the tree given to it
		- Evaluates truth of cond's
		- Evaluates numeric operations
		- Evaulates numeric values of variables
		- Returns numeric value of literal
*/
int eval(Node* tree){
	if(tree->type == t_lessThan	//Evaluate Conditional Expression
		|| tree->type == t_greaterThan
		|| tree->type == t_lessEqual
		|| tree->type == t_greaterEqual
		|| tree->type == t_notEqual
		|| tree->type == t_doubleEqual){

		//recursively evaluates down the tree, and returns up
		int lhs = eval(tree->child[0]);
		int rhs = eval(tree->child[1]);
		unordered_map<std::string,int>::const_iterator num;
		switch(tree->type){
			case t_lessThan:
				num = variableTable.find(tree->child[0]->image);
				if( num == variableTable.end()){
					cout << "Variable Not initialized..." << endl;
				}
				return (lhs < rhs);
			case t_greaterThan:
				return (lhs > rhs);
			case t_lessEqual:
				return (lhs <= rhs);
			case t_greaterEqual:
				return (lhs >= rhs);
			case t_notEqual:
				return (lhs != rhs);
			case t_doubleEqual:
				return (lhs == rhs);
			default:
				return 0;
		}
	}else if(tree->type == t_literal){ //Evaluate Literal
		return stoi(tree->image);
	}else if(tree->type == t_id){ //Evaluate Variable

		unordered_map<std::string,int>::const_iterator num = variableTable.find(tree->image);
		if( num == variableTable.end()){
			cout << "Variable Not initialized..." << endl;
		}else{
			return num->second;
		}
		
	}else if(tree->type == t_add //Evaluate Numeric Operation
		|| tree->type == t_sub
		|| tree->type == t_mul
		|| tree->type == t_div){

		//recursively evaluates down the tree, and returns up
		int lhs = eval(tree->child[0]);
		int rhs = eval(tree->child[1]);

		switch(tree->type){
			case t_add:
				return (lhs + rhs);
			case t_sub:
				return (lhs - rhs);
			case t_mul:
				return (lhs * rhs);
			case t_div:
				return (lhs / rhs);
			default:
				return 0;
		}
	}
	return 0;
}
