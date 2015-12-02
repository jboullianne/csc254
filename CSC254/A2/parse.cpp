/*
Jean-Marc Boullianne
jboullia@u.rochester.edu
CSC 254: Fall 2015
Assignment #2
*/

#include <iostream>
#include <cstdlib>
#include <exception>
#include "interpreter.h"

using namespace std;


const string names[] = {"read", "write", "id", "literal", ":=", "while", "if", "end", "<=", ">=", "==", "!=", "<", ">", "+", "-", "*", "/", "(", ")", "eof", "Statement Starter (id, read, write, if, while)", "Condition Starter (id, literal, \"(\")", "Expression Starter (id, literal, \"(\")", "Relational Operator (!=, ==, <, <=, ... )", "Addition Operator (+, -)", "Multiplication Operator (*, /)"};

static token input_token;
string old_image = ""; //Used to save ID's for after match has scanned for next token
bool outputTree = true; //Output trees if no errors
long int numOfToken = 0; //Number of tokens. Used for error reporting
long int numVariables = 0; //Number of declared variables (ID's). Used by interpreter 

//Throws an error with: Token expected, and where it was expected (numOfToken used)
void error (token expected) {
    outputTree = false;
    throw "Syntax Error: expected \""  + names[expected] + "\" at Token #: " + to_string(numOfToken);
    return;
}

//Matchs the current token to expected token, and scans for next token
void match (token expected, bool print) {
    if (input_token == expected) {
            if(input_token == t_id){
                numVariables++;
            }
            old_image = token_image;
            numOfToken++;
            if(input_token != t_eof)
                input_token = scan ();
    }else{
        error( expected );
    }
}


Node* program ();
Node* stmt_list ();
Node* stmt ();
Node* expr ();
Node* cond ();
Node* term_tail ();
Node* term ();
Node* factor_tail ();
Node* factor ();
string rel_op ();
string add_op ();
string mul_op ();
void printTree(Node*);
bool first(string);
bool follow(string);


/*
Parses the program using rule provided.
    - Error Handler is attached
    - Returns root node to start of syntax tree
*/
Node* program () {
    Node* tmp;
    try{
        switch (input_token) {
            case t_id:
            case t_read:
            case t_print:
            case t_write:
            case t_if:
            case t_while:
            case t_eof:
                tmp = stmt_list ();
                match (t_eof, false);
                return tmp;
            default: 
                error (t_starter);
        }
    } catch(const string msg){
        cout << msg << endl;
        while(1){
            numOfToken++;
            input_token = scan();
            if(first("program")){ //First set
                //trying again with next token
                return program();
            } else{ //No need to implement follow of this rule. We would end parsing if we did
                numOfToken++;
                input_token = scan(); //get next_token. "Eat up Tokens"
            }
        }
    }
    return NULL;
}

/*
Parses the stmt_list using rule provided.
    - Returns root node of a stmt_list
*/
Node* stmt_list () {
    Node* tmp;
    switch (input_token) {
        case t_id:
        case t_read:
        case t_write:
        case t_print:
        case t_if:
        case t_while:
            tmp = stmt ();
            tmp->child[3] = stmt_list ();
            return tmp;
        case t_eof:
        case t_end:
            return NULL;          /*  epsilon production */
        default: error (t_starter);
            return NULL;
    }
}

/*
Parses a stmt using rule provided.
    - Returns root node of a stmt
    - Error Handler is attached
*/
Node* stmt () {
    string id = "";
    Node* tmp; //node to be returned
    Node* rhs; //right hand side of statement
    try{
        switch (input_token) {
            case t_id:
                match (t_id, false);
                match (t_gets, false);
                id = old_image;
                rhs = expr();
                tmp = new Node(":=", 0, t_procedure);
                tmp->child[1] = rhs;
                tmp->child[0] = new Node(id, 0, t_id);
                return tmp;
            case t_read:
                match (t_read, true);
                match (t_id, true);
                tmp = new Node("read", 0, t_procedure);
                tmp->child[0] = new Node(old_image, 0, t_id);
                return tmp;
            case t_write:
                match (t_write, true);
                tmp = new Node("write", 0, t_procedure);
                tmp->child[0] = expr();
                return tmp;
            case t_print:
                match(t_print, true);
                tmp = new Node("#", 0, t_procedure);
                match(t_sLiteral, true);
                tmp->child[0] = new Node(old_image, 0, t_sLiteral);
                return tmp;
            case t_if:
                match(t_if, true);
                tmp = new Node("if", 0, t_procedure);
                tmp->child[0] = cond();
                tmp->child[1] = stmt_list();
                match(t_end, true);
                //tmp->child[2] = new Node("end", 0, t_procedure);
                return tmp;
            case t_while:
                match(t_while, true);
                tmp = new Node("while", 0, t_procedure);
                tmp->child[0] = cond();
                tmp->child[1] = stmt_list();
                match(t_end, true);
                //tmp->child[2] = new Node("end", 0, t_procedure);
                return tmp;
            default: error (t_starter);
                return NULL;
        }
    } catch (const string msg){
        cout << msg << endl;
        while(1){
            numOfToken++;
            input_token = scan();
            if(first("stmt")){
                //trying again with next token
                return stmt();
            } else if (follow("stmt")){
                return NULL; //meaning we've parsed all available stmt"s and can return
            } else{
                numOfToken++;
                input_token = scan(); //get next_token. "Eat up Tokens"
            }
        }
    }
    return NULL;
}

/*
Parses a cond using rule provided.
    - Returns root node of a cond
    - Error Handler is Attached
*/
Node* cond () {
    Node* lhs; //left hand side of cond
    Node* rhs; //right hand side of cond
    Node* tmp; //Node to be returned
    string ro = ""; //Relational Operator
    try{
        switch(input_token) {
            case t_lparen:
            case t_id:
            case t_literal:
                lhs = expr();
                ro = rel_op();
                rhs = expr();
                if(ro == "<")
                    tmp = new Node(ro, 0, t_lessThan);
                if(ro == ">")
                    tmp = new Node(ro, 0, t_greaterThan);
                if(ro == "<=")
                    tmp = new Node(ro, 0, t_lessEqual);
                if(ro == ">=")
                    tmp = new Node(ro, 0, t_greaterEqual);
                if(ro == "==")
                    tmp = new Node(ro, 0, t_doubleEqual);
                if(ro == "!=")
                    tmp = new Node(ro, 0, t_notEqual);
                tmp->setChildren(lhs, rhs, 0, 0);
                return tmp;
            default: error (t_cond);
            return NULL;
        }
    } catch (const string msg){
        cout << msg << endl;
        while(1){
            numOfToken++;
            input_token = scan();
            if(first("cond")){
                //trying again with next token
                return cond();
            } else if (follow("cond")){
                return NULL; //meaning we've parsed all available cond"s and can return
            } else{
                // prevents an infinite loop...
                if (input_token == t_eof)
                {
                    exit(1);
                }
                numOfToken++;
                input_token = scan(); //get next_token. "Eat up Tokens"
            }
        }
    }
}

/*
Parses an expr using rule provided.
    - Returns root node of an expr
    - Error Handler is Attached
*/
Node* expr () {
    Node* lhs; //left hand side of expression
    Node* rhs; // right hand side of expression
    try{
        switch (input_token) {
            case t_id:
            case t_literal:
            case t_lparen:
                lhs = term ();
                rhs = term_tail ();
                if(rhs != NULL){ //In case rhs evaulated to epsilon
                    rhs->child[0] = lhs;
                    return rhs;
                }
                else
                    return lhs;
            default: error (t_expr);
            return NULL;
        }
    } catch (const string msg){
        cout << msg << endl;
        while(1){
            numOfToken++;
            input_token = scan();
            if(first("expr")){
                //trying again with next token
                return expr();
            } else if (follow("expr")){
                return NULL; //meaning we've parsed all available expr"s and can return
            } else{
                numOfToken++;
                input_token = scan(); //get next_token. "Eat up Tokens"
            }
        }
    }
}

/*
Parses the term_tail using rule provided.
    - Returns root node of a term_tail
*/
Node* term_tail () {
    string op; // add operator 
    Node* rhs; //right hand side of term_tail
    Node* tmp; //Node to be returned
    switch (input_token) {
        case t_add:
        case t_sub:
            op = add_op ();
            rhs = expr();
            if(op == "+")
                tmp = new Node(op,0, t_add);
            if(op == "-")
                tmp = new Node(op,0, t_sub);
            tmp->child[1] = rhs;
            return tmp;
        case t_rparen:
        case t_id:
        case t_read:
        case t_print:
        case t_write:
        case t_if:
        case t_while:
        case t_lessThan:
        case t_greaterThan:
        case t_lessEqual:
        case t_greaterEqual:
        case t_notEqual:
        case t_doubleEqual:
        case t_end:
        case t_eof:
            return NULL;         /*  epsilon production */
        default: error (t_add);
            return NULL;
    }
}

Node* term () {
    Node* lhs; //left hand side of term
    Node* rhs; //right hand side of term
    switch (input_token) {
        case t_id:
        case t_literal:
        case t_lparen:
            lhs = factor ();
            rhs = factor_tail ();
            if(rhs != NULL){ //In case right hand side evaluates to epsilon
                rhs->child[0] = lhs;
                return rhs;
            }
            else
                return lhs;
        default: error (t_id);
            return NULL;
    }
}

/*
Parses the factor_tail using rule provided.
    - Returns root node of a factor_tail
*/
Node* factor_tail () {
    string op = "";
    Node* rhs; //right hand side of factor_tail
    Node* tmp; // node to be returned
    switch (input_token) {
        case t_mul:
        case t_div:
            op += mul_op ();
            rhs = term();
            if(op == "*")
                tmp = new Node(op,0, t_mul);
            if(op == "/")
                tmp = new Node(op,0, t_div);
            tmp->child[1] = rhs;
            return tmp;
        case t_add:
        case t_sub:
        case t_rparen:
        case t_id:
        case t_read:
        case t_print:
        case t_write:
        case t_if:
        case t_while:
        case t_lessThan:
        case t_greaterThan:
        case t_lessEqual:
        case t_greaterEqual:
        case t_notEqual:
        case t_doubleEqual:
        case t_end:
        case t_eof:
            return NULL;         /*  epsilon production */
        default: error (t_mul);
            return NULL;
    }
}

/*
Parses the factor using rule provided.
    - Returns root node of a factor
*/
Node* factor () {
    Node* tmp; //Node to e returned
    switch (input_token) {
        case t_id :
            match (t_id, false);
            return new Node(old_image, 0, t_id);
        case t_literal:
            match (t_literal, false);
            return new Node(old_image, 0, t_literal);
        case t_lparen:
            match (t_lparen, false);
            tmp = expr();
            match (t_rparen, false);
            return tmp;
        default: error (t_id);
            return NULL;
    }
}

/*
Parses the rel_op using rule provided.
    - Returns string of a rel_op
*/
string rel_op(){
    switch(input_token) {
        case t_doubleEqual:
            match(t_doubleEqual, false);
            return "==";
        case t_notEqual:
            match(t_notEqual, false);
            return "!=";
        case t_lessThan:
            match(t_lessThan, false);
            return "<";
        case t_greaterThan:
            match(t_greaterThan, false);
            return ">";
        case t_lessEqual:
            match(t_lessEqual, false);
            return "<=";
        case t_greaterEqual:
            match(t_greaterEqual, false);
            return ">=";
        default: error (t_relOp);
        return NULL;
    }
}

/*
Parses the add_op using rule provided.
    - Returns string of an add_op
*/
string add_op () {
    switch (input_token) {
        case t_add:
            match (t_add, false);
            return "+";
        case t_sub:
            match (t_sub, false);
            return "-";
        default: error (t_addOp);
        return NULL;
    }
}

/*
Parses the mul_op using rule provided.
    - Returns string of a mul_op
*/
string mul_op () {
    switch (input_token) {
        case t_mul:
            match (t_mul, false);
            return "*";
        case t_div:
            match (t_div, false);
            return "/";
        default: error (t_mulOp);
        return NULL;
    }
}

int main () {

    cout << "~~~~~~~ Syntactic Analysis ~~~~~~~" << endl;
    input_token = scan (); //Get first token
    numOfToken++;

    //Build parse tree
    Node* tree = program();

    //If no errors, then print tree
    if(outputTree)
        printTree(tree);
    cout << endl;

    //If user has specified they would like to run code, we do so here
    if(runAfter && outputTree){
        interpret(tree, numVariables);
    }
    cout << endl;
}


/*
Self-implemented Functions
*/

//Outputs tree in pre-order
void printTree(Node* root){
    if(root == NULL){
        return;
    }else{
        if(root->child[0] != NULL
            || root->child[1] != NULL
            || root->child[2] != NULL
            || root->child[3] != NULL){

            cout << "( " << root->image << " ";
            printTree(root->child[0]);
            printTree(root->child[1]);
            printTree(root->child[2]);
            printTree(root->child[3]);
            cout << ") ";
        }
        else{
            cout << " " << root->image << " ";
        }
    }
}

//Checks if current token is apart of the first set of given procedure
bool first(string procedure){
        if(procedure == "program"){
            if( input_token == t_eof
                || input_token == t_id
                || input_token == t_read
                || input_token == t_print
                || input_token == t_write
                || input_token == t_if
                || input_token == t_while)
                return true;
            else
                return false;
        }
        if(procedure == "stmt"){
            if( input_token == t_id
                || input_token == t_read
                || input_token == t_print
                || input_token == t_write
                || input_token == t_if
                || input_token == t_while){
                return true;
            }
            else{
                return false;
            }
        }
        if(procedure == "cond"){
            if( input_token == t_id
                || input_token == t_literal
                || input_token == t_lparen)
                return true;
            else
                return false;
        }
        if(procedure == "expr"){
            if( input_token == t_id
                || input_token == t_literal
                || input_token == t_lparen)
                return true;
            else
                return false;
        }
        return false;
}

//Checks if current token is apart of the follow set of given procedure
bool follow(string procedure){
        if(procedure == "stmt"){
            if( input_token == t_id
                || input_token == t_read
                || input_token == t_print
                || input_token == t_write
                || input_token == t_if
                || input_token == t_while
                || input_token == t_end
                || input_token == t_eof)
                return true;
            else
                return false;
        }
        if(procedure == "cond"){
            if( input_token == t_id
                || input_token == t_read
                || input_token == t_print
                || input_token == t_write
                || input_token == t_if
                || input_token == t_while
                || input_token == t_end)
                return true;
            else
                return false;
        }
        if(procedure == "expr"){
            if( input_token == t_doubleEqual
                || input_token == t_notEqual
                || input_token == t_lessThan
                || input_token == t_greaterThan
                || input_token == t_lessEqual
                || input_token == t_greaterEqual
                || input_token == t_rparen
                || input_token == t_eof
                || input_token == t_end)
                return true;
            else
                return false;
        }
        return false;
}




