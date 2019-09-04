#ifndef EX3_MY_SET_H
#define EX3_MY_SET_H

#include <iostream>
#include <memory>


using std::shared_ptr;

/**
 * @author Idan Yamin
 * this class represents a template set
 * @class my_set
 * @tparam T the template parameter
 */
template<typename T>
class my_set
{

private:
    /**
     * nested struct this class represents a node with
     * left and right child and parent. the node contains T data
     * @struct Node
     *
     */
    struct Node
    {
    public:
        /**
         * constructor
         * @param data the data to put in the node
         */
        explicit Node(const T &data) : data(data), parent(nullptr), left(nullptr),
                                       right(nullptr)
        {};
        /** struct members **/
        T data;
        shared_ptr<Node> parent;
        shared_ptr<Node> left;
        shared_ptr<Node> right;
    };


public:
    /**
     * this class represents an iterator of my_set.
     * iterator is a friend class of my_set.
     * my_set is a friend of this class
     * @class iterator
     */
    class iterator
    {
        //todo add comments on this class
    public:
        friend class my_set;

        /**
         * constructor, gets a node and creating an iterator from this node
         * @param node a node for the iterator
         */
        explicit iterator(const shared_ptr<Node> &node) : _current(
                node)
        {}

        /**
        * dereference iterator
        * @return the value of the iterator
        */
        virtual const T &operator*() const;

        /**
        * return data address
        * @return data address
        */
        virtual T *operator->() const;

        /**
        * get next iterator
        * @return the next iterator in an ascending order
        */
        virtual iterator &operator++();

        /**
        * posfix operator ++,
        * @return current iterator
        */
        iterator operator++(int);

        /**
        * prefix operator -- get previous iterator
        * @return previous iterator
        */
        virtual iterator &operator--();

        /**
        * make the iterator the previous one and returning the current one
        * @return the current iterator
        */
        iterator operator--(int);

        /**
        * check if this == other (compare them by data)
        * @param other the other iterator
        * @return true if they are equal, false otherwise.
        */
        virtual bool operator==(const iterator &other) const;

        /**
        * check if this != other (compare them by data)
        * @param other the other iterator
        * @return true if they are not equal, false otherwise.
        */
        virtual bool operator!=(const iterator &other) const;

    protected:
        /** data member **/
        std::weak_ptr<Node> _current;
    };

    /**
     * reverse iterator
     * @class ReverseIterator
     */
    class ReverseIterator : public iterator
    {
    public:
        explicit ReverseIterator(const shared_ptr<Node> &node)
                : iterator(node)
        {};

        /**
        * get previous value
        * @return the previous value
        */
        iterator &operator++() override;

        /**
         * get the next value
         * @return the next value
         */
        iterator &operator--() override;

        /**
         * posfix operator --, make the iterator the successor while return the current
         * @return the current iterator
         */
        iterator operator--(int);

        /**
        * get previous value
        * @return the previous value
        */
        ReverseIterator operator++(int);
    };

    /** typedefs **/
    typedef const iterator const_iterator;
    typedef my_set::ReverseIterator reverse_iterator;
    typedef const my_set::reverse_iterator const_reverse_iterator;
    typedef unsigned int size_type;

    /**
     * public members, I went throw the API of set in c++.
     * and made any public members in its API a public member here
     */
    size_type _size;


    /**
     * constructor, init an empty list.
     */
    my_set() : _size(0), _root(nullptr), _min(nullptr), _max(nullptr)
    {};

    /**
    * copy constructor, deep copy x into this
    * @param x the other my_set
    */
    my_set(const my_set &x);

    /**
    * move x values into this
    * @param x the obj to move from
    */
    my_set(my_set &&x) noexcept;

    /**
     * constructor from iterator
     * @tparam InputIterator an iterator
     * @param start the start of the iterator
     * @param end the end of the iterator
     */
    template<class InputIterator>
    my_set(InputIterator start, InputIterator end);

    /**
    * operator =, make this equal to x
    * @param x the element after the = operator
    * @return this
    */
    my_set &operator=(my_set x);

    /**
     * returns whether the set is empty
     * @return true if set is empty, false otherwise
     */
    bool empty()
    { return _size == 0; }

    /**
     * insert an item to linked list
     * @param toFind (data to insert)
     * @return std::pair first element in pair is an iterator
     * pointing at the newly inserted/ existing element.
     * the second one in the pair is true if insertion succeeded, false otherwise.
     */
    std::pair<iterator, bool> insert(const T &toInsert);

    /**
    * insert an item to linked list
    * @param toFind (data to insert)
    * @return std::pair first element in pair is an iterator
    * pointing at the newly inserted/ existing element.
    * the second one in the pair is true if insertion succeeded, false otherwise.
    */
    std::pair<iterator, bool> insert(T &&toInsert);

    /**
     * get an iterator that has * and ++ and insert values [first, last)
     * @tparam InputIterator an iterator
     * @param first the first iterator
     * @param last the last iterator
     */
    template<class InputIterator>
    void insert(InputIterator first, InputIterator last);

    /**
    * The function optimizes its insertion time if position points to the
    * element that will follow the inserted element
    * @param position the hint position
    * @param val the value to insert
    * @return an iterator to the inserted value
    */
    iterator insert(iterator position, const T &val);

    /**
    * The function optimizes its insertion time if position points to the
    * element that will follow the inserted element
    * @param position the hint position
    * @param val the value to insert
    * @return an iterator to the inserted value
    */
    iterator insert(const_iterator position, T &&val);

    /**
    * std::swap, swap info between a and b
    * @param a the first set
    * @param b the second set
    */
    template<typename U>
    friend void swap(my_set<U> &a, my_set<U> &b) noexcept;

    /**
    * return iterator
    * @param val value to search for
    * @return an iterator with the value
    */
    const_iterator find(const T &val) const;

    /**
     * return iterator to past the end container
     * @return iterator to past the end container
     */

    /**
     * @return const beginning iterator
     */
    const_iterator cbegin() const noexcept
    { return iterator(_min); }

    /**
     * @return const end iterator
     */
    const_iterator cend() const noexcept
    { return iterator(nullptr); }

    /**
     * @return beginning iterator
     */
    iterator begin() noexcept
    { return iterator(_min); }

    /**
     * @return beginning iterator
     */
    const_iterator begin() const noexcept
    { return iterator(_min); }

    /**
     * @return end iterator
     */
    iterator end() noexcept
    { return iterator(nullptr); }

    /**
     * @return end iterator
     */
    const_iterator end() const noexcept
    {
        return iterator(nullptr);
    }

    /**
     * @return reverse beginning iterator
     */
    reverse_iterator rbegin() noexcept
    {
        return reverse_iterator(_max);
    }

    /**
     * @return reversed beginning iterator
     */
    const_reverse_iterator rbegin() const noexcept
    {
        return reverse_iterator(_max);
    }

    /**
     * @return reverse end iterator
     */
    reverse_iterator rend() noexcept
    {
        return reverse_iterator(nullptr);
    }

    /**
     * @return reversed end iterator
     */
    const_reverse_iterator rend() const noexcept
    {
        return reverse_iterator(nullptr);
    }

    /**
     * @return const beginning iterator
     */
    const_reverse_iterator crbegin() const noexcept
    {
        return reverse_iterator(_max);
    }

    /**
     * @return const end iterator
     */
    const_reverse_iterator crend() const noexcept
    {
        return reverse_iterator(nullptr);
    }

    /**
     * erase the node at the given position
     * @param position the position to erase from
     * @return an iterator to successor of position
     */
    iterator erase(const_iterator position);

    /**
    * erase val from set
    * @param val the value to erase
    * @return the number of values that was erased
    */
    size_type erase(const T &val);

    /**
    * remove all elements between first and last not including last
    * @param first the starting point
    * @param last ending point
    * @return an iterator to next value
    */
    iterator erase(const_iterator first, const_iterator last);

    /**
     * clear all data from my_set
     */
    void clear() noexcept;

    /**
     * returns the size of the set
     * @return set size
     */
    size_type size()
    { return _size; }

    /**
     * destructor
     */
    ~my_set()
    {
        clear();
    }


private:
    /** data members **/
    shared_ptr<Node> _root;
    shared_ptr<Node> _min;
    shared_ptr<Node> _max;

    /**
    * set the root of the tree
    * @param current the current Node
    */
    void setRoot(const shared_ptr<Node> &current);

    /**
     * insert value from a specific node
     * @param val  the value to insert
     * @param insertFrom the node to insert from
     * @return a pair of iterator to the inserted node and bool that says if value was
     * inserted or not
     */
    std::pair<my_set::iterator, bool> insertHelper(const T &val,
                                                   const std::shared_ptr<Node> &insertFrom);

    /**
     * set the right child of parent to current
     * @param current the current node
     * @param parent the parent node
     * @return current node
     */
    shared_ptr<Node> &setLeftChild(shared_ptr<Node> &current, shared_ptr<Node> &parent);

    /**
     * set the right child of the parent to current
     * @param current the current node
     * @param parent the parent node
     * @return current node
     */
    shared_ptr<Node> &setRightChild(shared_ptr<Node> &current, shared_ptr<Node> &parent);

    /**
     * delete leaf
     * @param toDel the leaf to delete
     * @return iterator to the successor of the deleted node.
     */
    iterator deleteLeaf(shared_ptr<Node> &current);

    /**
     * delete node that only has one left child
     * @param toDel the node to delete
     * @return true if node was deleted false otherwise.
     */
    iterator deleteNodeWithLeftChild(shared_ptr<Node> &current);

    /**
     * delete node that only has one right child
     * @param toDel node to delete
     * @return the successor of the the deleted node.
     */
    iterator deleteNodeWithRightChild(shared_ptr<Node> &current);

    /**
    * find the successor of current Node
    * @param node current node
    * @return successor node of current node, if there is no successor returns nullptr
    */
    static shared_ptr<Node> successor(const shared_ptr<Node> &node);

    /**
    * find the predecessor of current Node
    * @param node current node
    * @return predecessor node of current node, if there is no predecessor returns nullptr
    */
    static shared_ptr<Node> predecessor(const shared_ptr<Node> &node);

    /**
     * delete nodes with two childrens
     * @param toDel node to delete
     * @return an iterator to the successor of the iterator
     */
    iterator deleteNodeWithTwoChildrens(shared_ptr<Node> &toDel);

    /**
    * insert value from a specific node
    * @param val  the value to insert
    * @param insertFrom the node to insert from
    * @return a pair of iterator to the inserted node and bool that says if value was
    * inserted or not
    */
    std::pair<typename my_set<T>::iterator, bool> insertHelper(
            const T &&val,
            const std::shared_ptr<Node> &insertFrom);

    /**
     * get max node
     * @return max node
     */
    shared_ptr<Node> getMin()
    { return _min; }

    /**
     * get min node
     * @return return min node
     */
    shared_ptr<Node> getMax()
    { return _max; }
};

#include "my_set.hpp"

#endif //EX3_MY_SET_H
