/*
Jean-Marc Boullianne
jboullia@u.rochester.edu
CSC 254: Fall 2015
Assignment 05
*/

#include <string>
using namespace std;

using std::cout;
using std::endl;
using std::flush;

//Declare Template class
template <class T> class oset;


template <class T> class oset{
    class node {
     public:
        T val;  //Val is of Type T so it matches the type that an oset was constructed with
        node *next;
        node(T v) : val(v), next(NULL) { }
    };
    node head;
    node beyond;
    

    //--------------------------------------
    // Iterator support

    public:
        class iter {
            node *pos;          // node _before_ the one with this->operator*
            // constructor is private:
            iter(node* n) : pos(n) { }

            friend class oset;      // so oset can call the (private) constructor
            public:   
                T & operator*() {
                    return pos->next->val;
                }
                // support forward iteration.  This is prefix version (++p).
                iter& operator++() {
                    if (pos != NULL) pos = pos->next;
                    return *this;
                }
                // and this is the postfix version (p++).
                // Note that it returns a copy, _not_ a reference.
                iter operator++(int) {
                    iter rtn = *this;
                    if (pos != NULL) pos = pos->next;
                    return rtn;
                }
                // gets current pos of iterator
                node* getPos(){
                    return pos;
                }
                bool operator==(iter other) {return pos->next == other.pos->next;}
                bool operator!=(iter other) {return pos->next != other.pos->next;}
        };

        iter begin() {
            return start;
        }
        iter end() {
            return finish;
        }

        // Holds compare function passed in by constructor
        bool (* cmp)(T,T); //Takes to Type T's

    private:
        iter start;         // initialized in the constructors below
        iter finish;        // initialized in the constructors below

        void clear() {
            node *n = head.next;
            while (n) {
                node* t = n->next;
                delete n;
                n = t;
            }
            head.next = NULL;
        }

        // Return pointer to last node with val < v
        node* find_prev(T v) {
            node* p = &head;
            while (true) {
                if (p->next == NULL) return p;
                if ((*cmp)(p->next->val, v)) return p;
                p = p->next;
            }
        }

     public:
        //--------------------------------------
        // Constructors and destructor

        // new empty set:
        oset<T>(bool (*f)(T,T)) : head(T()), beyond(T()), start(&head), finish(&beyond), cmp(f) {
            head.next = NULL;
        }

        // new singleton set:
        oset<T>(T v, bool (*f)(T,T)) : head(T()), beyond(T()), start(&head), finish(&beyond), cmp(f) {
            head.next = new node(v);
        }

        // copy constructor:
        oset<T>(oset<T>& other, bool (*f)(T,T)) : head(T()), beyond(T()), start(&head), finish(&beyond), cmp(f) {
            node *o = other.head.next;
            node *n = &head;
            while (o) {
                n->next = new node(o->val);
                o = o->next;
                n = n->next;
            }
            n->next = NULL;
        }

        // destructor -- clean up nodes:
        ~oset<T>() {
            clear();
        }
        // assignment
        oset<T>& operator=(oset<T>& other) {
            clear();
            operator+=(other); 
            return *this;     // union (see below)
        }

        // find -- return true iff present:
        bool operator[]( T v) {
            node* p = find_prev(v);
            return (p->next != NULL && p->next->val == v);
        }

        // insert v if not already present; return ref to self
        oset<T>& operator+=( T v) {
            node* p = find_prev(v);
            if (p->next == NULL || p->next->val != v) {
                node* n = new node(v);
                n->next = p->next;
                p->next = n;
            }
            return *this;
        }

        // remove v if present; return ref to self
        oset<T>& operator-=( T v) {
            node* p = find_prev(v);
            node* t;
            if ((t = p->next) != NULL && p->next->val == v) {
                // already present
                p->next = t->next;
                delete t;
            }
            return *this;
        }

        //Splices in new node, at current node iter points at, with value val in this oset
        oset<T>& insertVal(node* p, T v){
            node* n = new node(v);
            n->next = p->next;
            p->next = n;

            return *this;
        }

        //Deletes node, at next node iter points at, in this oset
        oset<T>& deleteVal(node* p){
            node* temp = p->next->next;
            delete p->next;
            p->next = temp;

            return *this;
        }

        //--------------------------------------
        // Union, intersection and difference.
        // These are operator assignments, to simplify management of space:
        // programmer is forced to manage temporaries explicitly
        // (as illustrated in the code for intersection below).

        // Union.
        //
        // 
        //
        oset<T>& operator+=(oset<T>& other) {
            
            //O(N^2) Time Algorithm
            if(cmp != other.cmp){
                for (iter i = other.begin(); i != other.end(); ++i) {
                    operator+=(*i);
                }
                return *this;
            }
            

            //Linear Time Algorithm
            iter i = begin();
            iter j = other.begin();

            //Iterate over other set, and splice in vals to this set. So it is O(N).
            while(j != other.end()){
                if(i == end()){
                    j++;
                    insertVal(i.getPos(), j.getPos()->val);
                    i++;
                }
                else if(*i == *j){   //Same value do nothing
                    i++;
                    j++;
                }
                else if(*i < *j){
                    i++;
                }else if(*j < *i){
                    j++;
                    insertVal(i.getPos(), j.getPos()->val);
                    i++;

                }else{
                    //Shouldn't ever execute. Used to make above cases clearer to read.
                }
            }
            return *this;
        }

        // Set difference.
        //
        // 
        //
        oset<T>& operator-=(oset<T>& other) {
            
            //O(N^2) Time Algorithm
            if(cmp != other.cmp){
                for (iter i = other.begin(); i != other.end(); ++i) {
                    operator-=(*i);
                }
                return *this;
            }

            //Linear Time Algorithm
            iter i = begin();
            iter j = other.begin();

            //Iterate over this set, and delete vals that other set has. So it is O(N).
            while(i != end()){
                if(*i == *j){
                    deleteVal(i.getPos());
                    j++;
                }else if(*i < *j){
                    i++;
                }else if(*j < *i){
                    j++;
                }else{
                    //Shouldn't ever execute. Used to make above cases clearer to read.
                }
            }
            
            return *this;
        }

        // Intersection.
        //
        // 
        //
        oset<T>& operator*=(oset<T>& other) {
            
            //O(N^2) Time Algorithm
            if(cmp != other.cmp){
                oset temp(cmp);      // empty
                for (iter i = begin(); i != end(); ++i) { // O(n) becaus of loop
                    if (other[*i]) temp+=(*i);
                }
                clear();
                operator+=(temp);   // union
                // NB: temp is destructed as we leave this scope
                return *this;
            }

            //Linear Time Algorithm
            iter i = begin();
            iter j = other.begin();

            //Iterate over this set and other set at the same time. Delete nodes in this set that aren't contained in other. So it is O(N).
            while(i != end() && j != other.end()){
                if(*i ==*j){
                    i++;
                    j++;
                }else if(*i < *j){
                    deleteVal(i.getPos());
                }else if(*j < *i){
                    j++;
                }else{
                    //Shouldn't ever execute. Used to make above cases clearer to read.
                }
            }
            
            return *this;
        }
};

//Not associated with classes. Helper Function
template <typename T>
void print(oset<T>& OS) {
    for (typename oset<T>::iter i = OS.begin(); i != OS.end(); ++i) {
        cout << *i << " ";
    }
    cout << endl;
}