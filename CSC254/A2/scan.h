/*
Jean-Marc Boullianne
jboullia@u.rochester.edu
CSC 254: Fall 2015
Assignment #2
*/

/* Simple ad-hoc scanner for the calculator language.
*/

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <stdio.h>

//#include "scan.h"

using namespace std;

enum token {t_read, t_write, t_id, t_literal, t_gets, t_while, t_if, t_end, t_lessEqual, t_greaterEqual, t_doubleEqual, t_notEqual, t_lessThan, t_greaterThan, t_add, t_sub, t_mul, t_div, t_lparen, t_rparen, t_eof, t_starter, t_cond, t_expr, t_relOp, t_addOp, t_mulOp, t_procedure, t_sLiteral, t_print};

extern char token_image[100]; //Actual Input parsed into one token
extern bool runAfter; // Will we interpret the code after parsing

extern token scan();

char token_image[100];
bool runAfter;

token scan() {
    runAfter = false;
    static int c = ' ';
        /* next available char; extra (int) width accommodates EOF */
    int i = 0;              /* index into token_image */

    /* skip white space */
    while (isspace(c)) {
        c = getchar();
    }
    if(c == '~'){
        runAfter = true;
        return t_eof;
    }
    else if(c == EOF){
        return t_eof;
    }else if(c == '#'){
        token_image[i] = c;
        c = getchar();
        if(isspace(c)){
            return t_print;
        }else{
            cout << stderr << "error\n";
            exit(1);
        }
    }else if(c == '"'){
        bool first = true;
        do {
            if(!first)
                token_image[i++] = c;
            first = false;
            c = getchar();
        } while (c != '"' && !isspace(c));
        token_image[i] = '\0';
        c = getchar();
        return t_sLiteral;
    }
        
    if (isalpha(c)) {
        do {
            token_image[i++] = c;
            c = getchar();
        } while (isalpha(c) || isdigit(c) || c == '_');
        token_image[i] = '\0';
        if (!strcmp(token_image, "read")) return t_read;
        else if (!strcmp(token_image, "write")) return t_write;
        else if (!strcmp(token_image, "while")) return t_while;
        else if (!strcmp(token_image, "if")) return t_if;
        else if (!strcmp(token_image, "end")) return t_end;
        else return t_id;
    }
    else if (isdigit(c)) {
        do {
            token_image[i++] = c;
            c = getchar();
        } while (isdigit(c));
        token_image[i] = '\0';
        return t_literal;
    } else switch (c) {
        case ':':
            if ((c = getchar()) != '=') {
                cout << stderr << "error\n";
                exit(1);
            } else {
                c = getchar();
                return t_gets;
            }
            break;
        case '+': c = getchar(); return t_add;
        case '-': c = getchar(); return t_sub;
        case '*': c = getchar(); return t_mul;
        case '/': c = getchar(); return t_div;
        case '(': c = getchar(); return t_lparen;
        case ')': c = getchar(); return t_rparen;
        case '<':
            c = getchar();
            if (c == '=') {
                c = getchar();
                if(isspace(c))
                    return t_lessEqual;
                else{
                    cout << stderr << "Error: not a valid operator\n";
                    exit(1);
                }
            } else if(isspace(c)){
                return t_lessThan;
            } else {
                cout << stderr << "Error: not a valid operator\n";
                exit(1);
            }
            break;
        case '>':
            c = getchar();
            if (c == '=') {
                c = getchar();
                if(isspace(c))
                    return t_greaterEqual;
                else{
                    cout << stderr << "Error: not a valid operator\n";
                    exit(1);
                }
            } else if(isspace(c)){
                return t_greaterThan;
            } else {
                cout << stderr << "Error: not a valid operator\n";
                exit(1);
            }
            break;
        case '=':
            if ((c = getchar()) != '=') {
                cout << stderr << "error\n";
                exit(1);
            } else {
                c = getchar();
                if(isspace(c))
                    return t_doubleEqual;
                else{
                    cout << stderr << "Error: not a valid operator\n";
                    exit(1);
                }
            }
            break;
        case '!':
            if ((c = getchar()) != '=') {
                cout << stderr << "error\n";
                exit(1);
            } else {
                c = getchar();
                if(isspace(c))
                    return t_notEqual;
                else{
                    cout << stderr << "Error: not a valid operator\n";
                    exit(1);
                }
            }
            break;
        default:
            cout << "error\n";
            exit(1);
    }
}
