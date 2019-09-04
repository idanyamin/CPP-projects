#include "my_set.h"

/**
 * destructor of Node
 */
my_set::Node::~Node()
{
    _next = nullptr;
    delete _next;
}

/**
 * copy constructor
 * @param toCopy the object to copy
 */
my_set::my_set(const my_set &toCopy)
{
    // make head null
    if (toCopy._head == nullptr)
    {
        _sum = big_integer();
        _head = nullptr;
        return;
    }

    _sum = big_integer(toCopy._sum);
    Node *head = new Node(toCopy._head->_data);
    _head = head;
    Node *currThis = head, *currCpy = toCopy._head->_next;
    while (currCpy != nullptr)
    {
        currThis ->_next = new Node(currCpy->_data);
        currThis = currThis->_next;
        currCpy = currCpy ->_next;
    }
}


/**
 * return true if toFind is in set, false otherwise
 * @param toFind the number to search for
 * @return true if toFind is in set, false otherwise
 */
bool my_set::is_in_set(const big_integer &toFind) const
{
    if (_head == nullptr)
    {
        return false;
    }
    Node *cur = _head;
    while (cur != nullptr)
    {
        if (cur->_data == toFind)
        {
            return true;
        }
        cur = cur->_next;
    }
    return false;
}

/**
 * add the value to the list
 * @param toAdd
 * @return true if toAdd was added, false otherwise.
 */
bool my_set::add(const big_integer &toAdd)
{
    // can't add value if it's already in
    if (is_in_set(toAdd))
    {
        return false;
    }
    // if the list is empty
    if (_head == nullptr)
    {
        _head = new Node(toAdd);
        _sum = _sum + toAdd;
        return true;
    }
    // if toAdd suppose to first, make him the head
    if (_addToBeginning(toAdd))
    {
        _sum = _sum + toAdd;
        return true;
    }
    // make cur point to head
    Node *cur = _head;
    while (cur != nullptr)
    {
        // try to add the Node to the end
        if (_addToEnd(toAdd, cur))
        {
            _sum = _sum + toAdd;
            return true;
        }
        // try to add to the middle
        if (_addToTheMiddle(toAdd, cur))
        {
            _sum = _sum + toAdd;
            return true;
        }
        cur = cur->_next;
    }
    return false;
}

/**
 * try to add node to the middle of the linked List
 * @param toAdd the value to add
 * @param cur current node
 * @return true if toAdd was added, false otherwise
 */
bool my_set::_addToTheMiddle(const big_integer &toAdd, my_set::Node *cur) const
{// try to add to the middle
    if (toAdd > cur->_data && toAdd < cur->_next->_data)
    {
        Node *newNode = new Node(toAdd);
        newNode->_next = cur->_next;
        cur->_next = newNode;
        return true;
    }
    return false;
}

/**
 * try to add node to the end of the linked List
 * @param toAdd the value to add
 * @param cur current node
 * @return true if toAdd was added, false otherwise
 */
bool my_set::_addToEnd(const big_integer &toAdd, my_set::Node *cur) const
{
    if (toAdd > cur->_data && cur->_next == nullptr)
    {
        Node *newNode = new Node(toAdd);
        cur->_next = newNode;
        return true;
    }
    return false;
}

/**
 * try to add toAdd to the beginning of the linked list
 * @param toAdd the value to add
 * @return true if toAdd was added, false otherwise
 */
bool my_set::_addToBeginning(const big_integer &toAdd)
{
    // if toAdd suppose to first
    if (_head->_data > toAdd)
    {
        Node *newNode = new Node(toAdd);
        newNode->_next = _head;
        _head = newNode;
        return true;
    }
    return false;
}

/**
 * @param toDel value to delete
 * @return true if toDel was removed false otherwise
 */
bool my_set::remove(const big_integer &toDel)
{
    if (_head == nullptr)
    {
        return false;
    }
    // try to remove head
    if (_removeHead(toDel))
    {
        _sum = _sum - toDel;
        return true;
    }
    Node *cur = _head;
    while (cur != nullptr)
    {
        // if the next is null than toDel not found here
        if (cur->_next == nullptr)
        {
            return false;
        }
        // try to delete from the middle/end
        if (_removeFromMiddle(toDel, cur))
        {
            _sum = _sum - toDel;
            return true;
        }
        cur = cur->_next;
    }
    return false;

}

/**
 * try to remove from the middle of the linked list
 * @param toDel the value to delete
 * @param cur current node
 * @return true if value was deleted, false otherwise
 */
bool my_set::_removeFromMiddle(const big_integer &toDel, my_set::Node *cur) const
{
    if (cur->_next->_data == toDel)
    {
        Node *temp = cur->_next;
        cur->_next = cur->_next->_next;
        delete temp;
        return true;
    }
    return false;
}

/**
 * try to remove the head
 * @param toDel the value to delete
 * @return true if value was deleted, false otherwise
 */
bool my_set::_removeHead(const big_integer &toDel)
{
    if (_head->_data == toDel)
    {
        Node *cur = _head;
        _head = _head->_next;
        delete cur;
        return true;
    }
    return false;
}

/**
 * return a set of all the numbers in this but not in other
 * @param other the other set
 * @return a new set of all the numbers in this but not in other
 */
my_set my_set::operator-(const my_set &other) const
{
    Node *curThis = _head, *curOther = other._head;
    my_set newSet = my_set();
    while (curThis != nullptr && curOther != nullptr)
    {
        // in this case curThis doesn't in other
        if (curThis->_data < curOther->_data)
        {
            newSet.add(curThis->_data);
            curThis = curThis->_next;
        }
            // the value on other is in this.
        else if (curThis->_data == curOther->_data)
        {
            curOther = curOther->_next;
            curThis = curThis->_next;
        }
            //curr this is bigger
        else
        {
            curOther = curOther->_next;
        }
    }
    // all the number in this are not in other.
    // so we should add them all
    _addRemaining(curThis, newSet);
    return newSet;
}

/**
 * add remaining nodes to set
 * @param curThis this node
 * @param newSet the set
 */
void my_set::_addRemaining(my_set::Node *curThis, my_set &newSet) const
{
    while (curThis != nullptr)
    {
        newSet.add(curThis->_data);
        curThis = curThis->_next;
    }
}

/**
 * return this union other
 * @param other set
 * @return this union other
 */
my_set my_set::operator|(const my_set &other) const
{
    my_set unionSet = my_set(*this);
    Node *curThis = _head;
    Node *curOther = other._head;
    while (curOther != nullptr && curThis != nullptr)
    {
        // in this case other is not this
        if (curThis->_data > curOther->_data)
        {
            unionSet.add(curOther->_data);
            curOther = curOther->_next;
        }
        else if (curThis->_data == curOther->_data)
        {
            curThis = curThis->_next;
            curOther = curOther->_next;
        }
        else
        {
            curThis = curThis->_next;
        }
    }
    _addRemaining(curOther, unionSet);
    return unionSet;
}

/**
 * return this (and) other
 * @param other the other set
 * @return this (and) other
 */
my_set my_set::operator&(const my_set &other) const
{
    Node *curThis = _head, *curOther = other._head;
    my_set newSet = my_set();
    while (curThis != nullptr && curOther != nullptr)
    {
        // in this case curThis doesn't in other
        if (curThis->_data < curOther->_data)
        {
            curThis = curThis->_next;
        }
            // the value on other is in this.
        else if (curThis->_data == curOther->_data)
        {
            newSet.add(curThis->_data);
            curOther = curOther->_next;
            curThis = curThis->_next;
        }
            //curr this is bigger
        else
        {
            curOther = curOther->_next;
        }
    }
    return newSet;
}

/**
 * overload operator <<
 * @param ostream the stream
 * @param set the set to put in the stream
 * @return the stream
 */
std::ostream &operator<<(std::ostream &ostream, const my_set &set)
{
    if (set._head == nullptr)
    {
        ostream << std::endl;
        return ostream;
    }
    my_set::Node *cur = set._head;
    while (cur != nullptr)
    {
        ostream << cur->_data << std::endl;
        cur = cur->_next;
    }
    return ostream;
}

/**
 * destruct set
 */
my_set::~my_set()
{
    _freeLinkedList();
}

/**
 * return sum of set
 * @return
 */
big_integer my_set::sum_set() const
{
    return _sum;
}

/**
 * free linked list
 */
void my_set::_freeLinkedList()
{
    while (_head != nullptr)
    {
        Node *temp = _head;
        _head = _head->_next;
        delete temp;
    }
}

/**
 * deep copy of other to this
 * @param other other set
 * @return this set
 */
my_set &my_set::operator=(const my_set &other)
{
    if (this == &other)
    {
        return *this;
    }
    _freeLinkedList();
    auto *deepCpy = new my_set(other);
    this->_head = deepCpy->_head;
    this->_sum = deepCpy->_sum;
    deepCpy->_head = nullptr;
    delete deepCpy;
    return *this;
}
