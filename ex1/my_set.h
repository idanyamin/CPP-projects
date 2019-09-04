#ifndef EX1_MY_SET_H
#define EX1_MY_SET_H

#include "big_integer.h"

/*-------------------5 point bonus-----------------------*/
/**
 * I chose to make a sorted linked list from the smallest
 * value to the largest value.
 * I did it by making sure that each value that is added to the list is added
 * in an ordered way.
 * the price I paid for that is add function takes
 * O(N), but the print takes O(N) as well.
 * also it made the operator | & - more efficient.
 *
 * when inserting a value first I ask if the value is already the smallest
 * by comparing him to the head, if it is than congratulations we found a new head!
 * if not I ask each iteration over the nodes two questions:
 *
 * 1. Is the current node is smaller than the value and the next
 * node is null, if it is than congratulations we found a new tail!
 *
 * 2. Is the value I want to add
 * is larger than the current one and smaller than the next, if it is so congratulations
 * we insert it to the middle of them.
 *
 * now that our linked list is sorted printing it in an ordered way is super easy
 * barely an inconvenience. we just iterate over the list and add each node to the
 * stream. and the price for printing goes down to O(N).
 */

/**
 * set of big_integers
 * @author Idan Yamin
 * @class my_set
 */
class my_set
{
    /**
     * nested class for a Node
     * @class Node represents a single node
     */
    class Node
    {
    public:
        /* _data: the data of the node*/
        big_integer _data;
        /* _next: the next node */
        Node *_next;

        /**
         * a constructor that gets num and next and build a single node
         * @param num the _data of the new node
         * @param next the next node
         */
        Node(const big_integer &num, Node *next) : _data(big_integer(num)), _next(next)
        {};

        /**
         * a constructor of a node, gets a num
         * @param num
         */
        explicit Node(const big_integer &num) : Node(num, nullptr)
        {};
        /**
        * destructor of Node
        */
        ~Node();
    };

public:
    /**
     * a default constructor, makes an empty set.
     */
    my_set() : _head(nullptr), _sum(big_integer("0"))
    {};

    /**
    * copy constructor
    * @param toCopy the object to copy
    */
    my_set(const my_set &toCopy);

    /**
     * destruct set
     */
    ~my_set();

    /**
     * deep copy of other to this
     * @param other other set
     * @return this set
     */
    my_set &operator=(const my_set &other);

    /**
     * return true if toFind is in set, false otherwise
     * @param toFind the number to search for
     * @return true if toFind is in set, false otherwise
     */
    bool is_in_set(const big_integer &toFind) const;

    /**
     * add the value to the list
     * @param toAdd
     * @return true if toAdd was added, false otherwise.
     */
    bool add(const big_integer &toAdd);

    /**
     * return sum of set
     * @return
     */
    big_integer sum_set() const;

    /**
     * @param toDel value to delete
     * @return true if toDel was removed false otherwise
     */
    bool remove(const big_integer &toDel);

    /**
     * return a set of all the numbers in this but not in other
     * @param other the other set
     * @return a new set of all the numbers in this but not in other
     */
    my_set operator-(const my_set &other) const;

    /**
     * return this union other
     * @param other set
     * @return a set of (this union other)
     */
    my_set operator|(const my_set &other) const;

    /**
     * return this (and) other
     * @param other the other set
     * @return a set of this (and) other
     */
    my_set operator&(const my_set &other) const;

    /**
     * overload operator <<
     * @param ostream the stream
     * @param set the set to put in the stream
     * @return the stream
     */
    friend std::ostream &operator<<(std::ostream &ostream, const my_set &set);

private:
    /* the sum of integers on the set */
    big_integer _sum;
    /* the head of the linked list */
    Node *_head;

    /**
     * try to add toAdd to the beginning of the linked list
     * @param toAdd the value to add
     * @return true if toAdd was added, false otherwise
     */
    bool _addToBeginning(const big_integer &toAdd);

    /**
    * try to add node to the end of the linked List
    * @param toAdd the value to add
    * @param cur current node
    * @return true if toAdd was added, false otherwise
    */
    bool _addToEnd(const big_integer &toAdd, Node *cur) const;

    /**
    * try to add node to the middle of the linked List
    * @param toAdd the value to add
    * @param cur current node
    * @return true if toAdd was added, false otherwise
    */
    bool _addToTheMiddle(const big_integer &toAdd, Node *cur) const;

    /**
    * try to remove the head
    * @param toDel the value to delete
    * @return true if value was deleted, false otherwise
    */
    bool _removeHead(const big_integer &toDel);


    /**
    * try to remove from the middle of the linked list
    * @param toDel the value to delete
    * @param cur current node
    * @return true if value was deleted, false otherwise
    */
    bool _removeFromMiddle(const big_integer &toDel, Node *cur) const;

    /**
     * add remaining nodes to set
     * @param curThis this node
     * @param newSet the set
     */
    void _addRemaining(Node *curThis, my_set &newSet) const;

    /**
     * free linked list
     */
    void _freeLinkedList();
};


#endif //EX1_MY_SET_H
