#ifndef EX3_MY_SET_HPP
#define EX3_MY_SET_HPP

#include "my_set.h"

/**
 * copy constructor, deep copy x into this
 * @param x the other my_set
 */
template<typename T>
my_set<T>::my_set(const my_set<T> &x) : my_set()
{
    auto start = x.begin();
    while (start != end())
    {
        insert(*start);
        ++start;
    }
}

/**
 * constructor from iterator
 * @tparam InputIterator an iterator
 * @param start the start of the iterator
 * @param end the end of the iterator
 */
template<class T>
template<class InputIterator>
my_set<T>::my_set(InputIterator start, InputIterator end):my_set()
{
    while (start != end)
    {
        insert(*start);
        ++start;
    }
}

/**
 * move x values into this
 * @param x the obj to move from
 */
template<typename T>
my_set<T>::my_set(my_set<T> &&x) noexcept : my_set()
{
    _size = x._size;
    x._size = 0;
    _root = x._root;
    x._root = nullptr;
    _min = x._min;
    x._min = nullptr;
    _max = x._max;
    x._max = nullptr;
}

/**
 * operator =, make this equal to x
 * @param x the element after the = operator
 * @return this
 */
template<typename T>
my_set<T> &my_set<T>::operator=(my_set<T> x)
{
    //swap idiom
    std::swap(_root, x._root);
    std::swap(_min, x._min);
    std::swap(_max, x._max);
    unsigned int temp = _size;
    _size = x._size;
    x._size = temp;
    return *this;
}

/**
 * insert an item to linked list
 * @param toInsert (data to insert)
 * @return std::pair first element in pair is an iterator
 * pointing at the newly inserted/ existing element.
 * the second one in the pair is true if insertion succeeded, false otherwise.
 */

template<typename T>
typename std::pair<typename my_set<T>::iterator, bool> my_set<T>::insert(const T &toInsert)
{
    return insertHelper(toInsert, _root);
}

/**
 * insert value from a specific node
 * @param val  the value to insert
 * @param insertFrom the node to insert from
 * @return a pair of iterator to the inserted node and bool that says if value was
 * inserted or not
 */
template<typename T>
std::pair<typename my_set<T>::iterator, bool> my_set<T>::insertHelper(
        const T &toInsert, const shared_ptr<typename my_set<T>::Node> &insertFrom)
{
    std::shared_ptr<typename my_set<T>::Node> current = insertFrom;
    std::shared_ptr<typename my_set<T>::Node> parent = nullptr;
    while (current != nullptr)
    {
        // right is bigger left is smaller
        parent = current;
        if (toInsert > current->data)
        {
            current = current->right;
        }
        else if (toInsert < current->data)
        {
            current = current->left;
        }
        else if (toInsert == current->data)
        {
            return std::pair<my_set<T>::iterator, bool>(current, false);
        }
    }
    ++_size;
    current = std::make_shared<typename my_set<T>::Node>(toInsert);
    current->parent = parent;
    if (parent == nullptr)
    {
        setRoot(current);
    }
    else if (current->data < parent->data)
    {
        current = setLeftChild(current, parent);
    }
    else if (current->data > parent->data)
    {
        current = setRightChild(current, parent);
    }
    return std::pair<my_set<T>::iterator, bool>(current, true);
}

/**
* insert value from a specific node
* @param val  the value to insert
* @param insertFrom the node to insert from
* @return a pair of iterator to the inserted node and bool that says if value was
* inserted or not
*/
template<typename T>
typename std::pair<typename my_set<T>::iterator, bool> my_set<T>::insertHelper(
        const T &&val, const shared_ptr<my_set<T>::Node> &insertFrom)
{
    std::shared_ptr<typename my_set<T>::Node> current = insertFrom;
    std::shared_ptr<typename my_set<T>::Node> parent = nullptr;
    while (current != nullptr)
    {
        // right is bigger left is smaller
        parent = current;
        if (val > current->data)
        {
            current = current->right;
        }
        else if (val < current->data)
        {
            current = current->left;
        }
        else if (val == current->data)
        {
            return std::pair<my_set<T>::iterator, bool>(current, false);
        }
    }
    ++_size;
    current = std::make_shared<typename my_set<T>::Node>(std::move(val));
    current->parent = parent;
    if (parent == nullptr)
    {
        setRoot(current);
    }
    else if (current->data < parent->data)
    {
        current = setLeftChild(current, parent);
    }
    else if (current->data > parent->data)
    {
        current = setRightChild(current, parent);
    }

    return std::pair<my_set<T>::iterator, bool>(current, true);
}

/**
* The function optimizes its insertion time if position points to the
* element that will follow the inserted element
* @param position the hint position
* @param val the value to insert
* @return an iterator to the inserted value
*/
template<typename T>
typename my_set<T>::iterator my_set<T>::insert(my_set<T>::const_iterator position, T &&val)
{
    auto hintPos = position._current.lock();
    if (hintPos == nullptr)
    {
        auto result = insert(std::move(val));
        return result.first;
    }
    auto hintPredecessor = predecessor(hintPos);
    if (hintPredecessor != nullptr)
    {
        if (hintPredecessor.get()->data < val && val < hintPos->data)
        {
            auto result = insertHelper(std::move(val), hintPredecessor);
            return result.first;
        }
    }
    auto result = insertHelper(std::move(val), _root);
    return result.first;
}

/**
* insert an item to linked list
* @param toFind (data to insert)
* @return std::pair first element in pair is an iterator
* pointing at the newly inserted/ existing element.
* the second one in the pair is true if insertion succeeded, false otherwise.
*/
template<typename T>
typename std::pair<typename my_set<T>::iterator, bool> my_set<T>::insert(T &&toInsert)
{
    return insertHelper(std::move(toInsert), _root);
}

/**
 * set the right child of the parent to current
 * @param current the current node
 * @param parent the parent node
 * @return current node
 */
template<typename T>
shared_ptr<typename my_set<T>::Node> &
my_set<T>::setRightChild(shared_ptr<typename my_set<T>::Node> &current,
                         shared_ptr<typename my_set<T>::Node> &parent)
{
    parent->right = current;
    current->parent = parent;
    if (current->data > _max->data)
    {
        _max = current;
    }
    return current;
}

/**
 * set the right child of parent to current
 * @param current the current node
 * @param parent the parent node
 * @return current node
 */
template<typename T>
shared_ptr<typename my_set<T>::Node> &
my_set<T>::setLeftChild(shared_ptr<my_set<T>::Node> &current, shared_ptr<my_set<T>::Node> &parent)
{
    parent->left = current;
    current->parent = parent;
    if (current->data < _min->data)
    {
        _min = current;
    }
    return current;
}

/**
 * set the root of the tree
 * @param current the current Node
 */
template<typename T>
void my_set<T>::setRoot(const shared_ptr<my_set<T>::Node> &current)
{
    _root = current;
    _min = current;
    _max = current;
}

/**
 * find the successor of current Node
 * @param node current node
 * @return successor node of current node, if there is no successor returns nullptr
 */
template<typename T>
shared_ptr<typename my_set<T>::Node>
my_set<T>::successor(const shared_ptr<typename my_set<T>::Node> &node)
{
    shared_ptr<typename my_set<T>::Node> current = nullptr;
    //then succ lies in right subtree
    if (node->right != nullptr)
    {
        current = node->right;
        // find minimum of subtree
        while (current->left)
        {
            current = current->left;
        }
        return current;
    }
    // then succ is one of the ancestors
    shared_ptr<typename my_set<T>::Node> parent = node->parent;
    current = node;
    while (parent != nullptr && current == parent->right)
    {
        current = parent;
        parent = parent->parent;
    }
    return parent;
}

/**
* find the predecessor of current Node
* @param node current node
* @return predecessor node of current node, if there is no predecessor returns nullptr
*/
template<typename T>
shared_ptr<typename my_set<T>::Node>
my_set<T>::predecessor(const shared_ptr<typename my_set<T>::Node> &node)
{
    shared_ptr<typename my_set<T>::Node> current = nullptr;
    //then succ lies in right subtree
    if (node->left != nullptr)
    {
        current = node->left;
        // find minimum of subtree
        while (current->right)
        {
            current = current->right;
        }
        return current;
    }
    // then succ is one of the ancestors
    shared_ptr<typename my_set<T>::Node> parent = node->parent;
    current = node;

    while (parent != nullptr && current == parent->left)
    {
        current = parent;
        parent = parent->parent;
    }
    return parent;
}

/**
* return iterator
* @param val value to search for
* @return an iterator with the value
*/
template<typename T>
typename my_set<T>::const_iterator my_set<T>::find(const T &val) const
{
    shared_ptr<typename my_set<T>::Node> current = _root;
    while (current != nullptr)
    {
        if (current->data == val)
        {
            return my_set<T>::iterator(current);
        }
        else if (val < current->data)
        {
            current = current->left;
        }
            // val is greater
        else
        {
            current = current->right;
        }
    }
    return cend();
}

/**
 * erase val from set
 * @param val the value to erase
 * @return the number of values that was erased
 */
template<typename T>
typename my_set<T>::size_type my_set<T>::erase(const T &val)
{
    auto toDel = find(val);
    if (toDel._current.lock() != nullptr)
    {
        erase(toDel);
        return 1;
    }
    return 0;
}

/**
 * erase the node at the given position
 * @param position the position to erase from
 * @return an iterator to successor of position
 */
template<typename T>
typename my_set<T>::iterator my_set<T>::erase(my_set<T>::const_iterator position)
{
    auto current = position._current.lock();
    if (!current || _root == nullptr)
    {
        return iterator(nullptr);
    }
    //update min and max
    if (current.get()->data == _max->data)
    {
        _max = predecessor(_max);
    }
    if (current.get()->data == _min->data)
    {
        _min = successor(_min);
    }
    // delete nodes
    if (current.get()->right == nullptr && current->left == nullptr)
    {
        --_size;
        return deleteLeaf(current);
    }
    if (current.get()->right == nullptr && current->left != nullptr)
    {
        --_size;
        return deleteNodeWithLeftChild(current);
    }
    if (current.get()->right != nullptr && current->left == nullptr)
    {
        --_size;
        return deleteNodeWithRightChild(current);
    }
    return deleteNodeWithTwoChildrens(current);
}

/**
 * delete nodes with two childrens
 * @param toDel node to delete
 * @return an iterator to the successor of the iterator
 */
template<typename T>
typename my_set<T>::iterator
my_set<T>::deleteNodeWithTwoChildrens(shared_ptr<typename my_set<T>::Node> &toDel)
{
    T temp = toDel->data;
    iterator next(successor(toDel));
    toDel->data = *next;
    next._current.lock()->data = temp;
    return erase(next);
}

/**
 * delete node that only has one right child
 * @param toDel node to delete
 * @return the successor of the the deleted node.
 */
template<typename T>
typename my_set<T>::iterator
my_set<T>::deleteNodeWithRightChild(shared_ptr<typename my_set<T>::Node> &toDel)
{
    auto parent = toDel->parent;
    auto right = toDel->right;
    auto cur_successor = successor(toDel);
    if (parent == nullptr)
    {
        _root = right;
        _root->parent = nullptr;
        toDel->right = nullptr;
        return iterator(cur_successor);
    }
    if (parent->left == toDel)
    {
        parent->left = right;
    }
    else if (parent->right == toDel)
    {
        parent->right = right;
    }
    right->parent = parent;
    toDel->parent = nullptr;
    toDel->right = nullptr;
    return iterator(cur_successor);
}

/**
 * delete node that only has one left child
 * @param toDel the node to delete
 * @return true if node was deleted false otherwise.
 */
template<typename T>
typename my_set<T>::iterator
my_set<T>::deleteNodeWithLeftChild(shared_ptr<typename my_set<T>::Node> &toDel)
{
    auto parent = toDel->parent;
    auto left = toDel->left;
    auto cur_successor = successor(toDel);
    if (parent == nullptr)
    {
        _root = left;
        toDel->left = nullptr;
        _root->parent = nullptr;
        return iterator(cur_successor);
    }
    // parent is not null and current is left child of parent
    if (parent->left == toDel)
    {
        parent->left = left;
    }
        // parent is not null and current is right child of parent
    else if (parent->right = toDel)
    {
        parent->right = left;
    }
    left->parent = parent;
    toDel->parent = nullptr;
    toDel->left = nullptr;
    return iterator(cur_successor);
}

/**
 * delete leaf
 * @param toDel the leaf to delete
 * @return iterator to the successor of the deleted node.
 */
template<typename T>
typename my_set<T>::iterator my_set<T>::deleteLeaf(shared_ptr<typename my_set<T>::Node> &toDel)
{
    auto cur_successor = successor(toDel);
    if (toDel.get()->parent != nullptr)
    {
        // if current is right child
        if (toDel.get()->parent->left == toDel)
        {
            toDel.get()->parent->left = nullptr;
        }
            // if current is left child
        else if (toDel.get()->parent->right == toDel)
        {
            toDel.get()->parent->right = nullptr;
        }
        toDel->parent = nullptr;
    }
    if (toDel == _root)
    {
        _root = nullptr;
        _min = nullptr;
        _max = nullptr;
    }
    return iterator(cur_successor);
}

/**
 * remove all elements between first and last not including last
 * @param first the starting point
 * @param last ending point
 * @return an iterator to next value
 */
template<typename T>
typename my_set<T>::iterator
my_set<T>::erase(my_set<T>::const_iterator first, my_set<T>::const_iterator last)
{
    auto toDel = first;
    auto next = first;
    ++next;
    while (toDel != last)
    {
        erase(*toDel);
        toDel = next;
        ++next;
    }
    return next;
}

/**
 * clear all data from my_set
 */
template<typename T>
void my_set<T>::clear() noexcept
{
    auto start = begin();
    while (start != end())
    {
        start = erase(start);
    }
    erase(end());
}


/**
 * std::swap, swap info between a and b
 * @param a the first set
 * @param b the second set
 */

template<typename U>
void swap(my_set<U> &a, my_set<U> &b) noexcept
{
    std::swap(a._root, b._root);
    std::swap(a._min, b._min);
    std::swap(a._max, b._max);
    unsigned int temp = a._size;
    a._size = b._size;
    b._size = temp;
}

/**
 * get an iterator that has * and ++ and insert values [first, last)
 * @tparam InputIterator an iterator
 * @param first the first iterator
 * @param last the last iterator
 */
template<typename T>
template<typename InputIterator>
void my_set<T>::insert(InputIterator first, InputIterator last)
{
    while (first != last)
    {
        insert(*first);
        ++first;
    }
}

/**
 * The function optimizes its insertion time if position points to the
 * element that will follow the inserted element
 * @param position the hint position
 * @param val the value to insert
 * @return an iterator to the inserted value
 */
template<typename T>
typename my_set<T>::iterator my_set<T>::insert(my_set<T>::iterator position, const T &val)
{
    auto hintPos = position._current.lock();
    if (hintPos == nullptr)
    {
        auto result = insert(val);
        return result.first;
    }
    auto hintPredecessor = predecessor(hintPos);
    if (hintPredecessor != nullptr)
    {
        if (hintPredecessor.get()->data < val && val < hintPos->data)
        {
            auto result = insertHelper(val, hintPredecessor);
            return result.first;
        }
    }
    auto result = insertHelper(val, _root);
    return result.first;
}

/**
 * dereference iterator
 * @return the value of the iterator
 */
template<typename T>
const T &my_set<T>::iterator::operator*() const
{
    auto ptr = _current.lock();
    return ptr.get()->data;
}

/**
 * return data address
 * @return data address
 */
template<typename T>
T *my_set<T>::iterator::operator->() const
{
    auto current = _current.lock();
    if (current != nullptr)
    {
        return nullptr;
    }
    return &_current.lock()->data;
}

/**
 * get next iterator
 * @return the next iterator in an ascending order
 */
template<typename T>
typename my_set<T>::iterator &my_set<T>::iterator::operator++()
{
    auto current = _current.lock();
    if (current.get() == nullptr)
    {
        return *this;
    }
    _current = successor(current);
    return *this;
}


/**
 * get next
 * @return next iterator
 */
template<typename T>
typename my_set<T>::iterator my_set<T>::iterator::operator++(int)
{
    iterator iter = *this;
    ++(*this);
    return iter;
}

/**
 * get previous iterator
 * @return the previous iterator
 */
template<typename T>
typename my_set<T>::iterator &my_set<T>::iterator::operator--()
{
    auto current = _current.lock();
    if (current.get() == nullptr)
    {
        return *this;
    }
    _current = predecessor(current);
    return *this;
}

/**
 * make this iterator the previous one and return the current one
 * @current iterator
 */
template<typename T>
typename my_set<T>::iterator my_set<T>::iterator::operator--(int)
{
    iterator iter = *this;
    --(*this);
    return iter;
}

/**
 * check if this == other (compare them by data)
 * @param other the other iterator
 * @return true if they are equal, false otherwise.
 */
template<typename T>
bool my_set<T>::iterator::operator==(const my_set<T>::iterator &other) const
{
    auto thisCurrent = _current.lock();
    auto otherCurrent = other._current.lock();
    if (!thisCurrent && !otherCurrent)
    {
        return true;
    }
    if ((!thisCurrent && otherCurrent) || (thisCurrent && !otherCurrent))
    {
        return false;
    }
    return thisCurrent.get()->data == otherCurrent.get()->data;
}

/**
 * check if this != other (compare them by data)
 * @param other the other iterator
 * @return true if they are not equal, false otherwise.
 */
template<typename T>
bool my_set<T>::iterator::operator!=(const my_set<T>::iterator &other) const
{
    return !(*this == other);
}


/**
* get previous value
* @return the previous vvalue
*/
template<typename T>
typename my_set<T>::iterator &my_set<T>::ReverseIterator::operator++()
{
    iterator::operator--();
    return *this;
}

/**
* get previous value
* @return the previous value
*/
template<typename T>
typename my_set<T>::ReverseIterator my_set<T>::ReverseIterator::operator++(int)
{
    ReverseIterator iter = *this;
    ++(*this);
    return iter;
}

/**
 * reverse iterator prefix --
 * @tparam T the elements of the template
 * @return return previous iterator
 */
template<typename T>
typename my_set<T>::iterator &my_set<T>::reverse_iterator::operator--()
{
    iterator::operator++();
    return *this;
}


/**
 * reverse iterator posfix --
 * @tparam T the elements of the template
 * @return return current iterator
 */
template<typename T>
typename my_set<T>::iterator my_set<T>::reverse_iterator::operator--(int)
{
    reverse_iterator iter = *this;
    --(*this);
    return iter;
}


#endif //EX3_MY_SET_H