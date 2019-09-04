#ifndef EX1_BIG_INTEGER_H
#define EX1_BIG_INTEGER_H

#include <iostream>

using std::string;
using std::ostream;

/**
 * this class represents a big integer
 * @author Idan Yamin
 * @class big_integer
 */
class big_integer
{
public:
    /**
     * default constructor, create a 0 big_integer
     */
    big_integer() : _data("0"), _isPositive(true)
    {};

    /**
    * get an integer and construct a big number
    * @param number a number
    */
    big_integer(int number);

    /**
     * constructor
     * @param number a string representation of number
     */
    big_integer(const string &number);

    /**
     * construct a big integer out of a big integer
     * @param bigNum an integer
     */
    big_integer(const big_integer &bigNum);

    /**
     * put this big_integer in stream
     * @param output the stream
     * @param num the big integer to put in the stream
     * @return an updated stream
     */
    friend ostream &operator<<(ostream &output, const big_integer &num);

    /**
     * adds this to other and return the result
     * @param other big_integer
     * @return this + other
     */
    big_integer operator+(const big_integer &other) const;

    /**
     * return this - other
     * @param other big_integer
     * @return this - other
     */
    big_integer operator-(const big_integer &other) const;

    /**
     * this function calculate this   other
     * @param other big_integer
     * @return return this after doing this += other
     */
    big_integer &operator+=(const big_integer &other);

    /**
     * this method calculate this -= other
     * @param other big_integer
     * @return return this after doing this -= other
     */
    big_integer &operator-=(const big_integer &other);

    /**
     * multiply this to other and return the result
     * @param other big_integer
     * @return this * other
     */
    big_integer operator*(const big_integer &other) const;

    /**
     * assigns other * this to this
     * @param other
     * @return this after doing this * other
     */
    big_integer &operator*=(const big_integer &other);

    /**
     * divide: this/ other
     * @param other divisor
     * @return this/ other
     */
    big_integer operator/(const big_integer &other) const;

    /**
     * this function calculate (this /= other)
     * @param other big_integer
     * @return return this after doing (this /= other)
     */
    big_integer &operator/=(const big_integer &other);

    /**
     * divide: this%other
     * @param other divisor
     * @return this% other
     */
    big_integer operator%(const big_integer &other) const;

    /**
    * default = operator
    * @param other the other big_integer
    * @return this integer after shallow copy from other
    */
    big_integer &operator=(const big_integer &other) = default;

    /**
    * compare to other big_integer for equality check.
    * @param other big_integer to compare.
    * @return true if other is equal to this object, false otherwise.
    */
    bool operator==(const big_integer &other) const;

    /**
    * compare to other big_integer for inequality
    * @param other big_integer to compare.
    * @return true if other is not equal to this object, false otherwise.
    */
    bool operator!=(const big_integer &other) const;

    /**
    * compare the two number by > operator
    * @param other the number to compare to
    * @return compare the two number, if this > other return true, false otherwise
    */
    bool operator>(const big_integer &other) const;

    /**
    * compare the two number by < operator
    * @param other the number to compare to
    * @return compare the two number, if this < other return true, false otherwise
    */
    bool operator<(const big_integer &other) const;

    /**
    * compare this to other using >=
    * @param other big_integer
    * @return true if this >= other, false otherwise.
    */
    bool operator>=(const big_integer &other) const;

    /**
    * compares this to other using <=
    * @param other big_integer
    * @return true if this <= other, false otherwise.
    */
    bool operator<=(const big_integer &other) const;

    /**
    * division of num1/num2 (positive sign)
    * @param num1 big_integer
    * @param num2 big_integer
    * @return num1/num2
    */
    static big_integer division(const big_integer &num1, const big_integer &num2);

private:
    /* this data */
    string _data;

    /* the sign of the number, true for positive false for negative */
    bool _isPositive;

    /**
    * return abs value of current integer
    * @return abs value of current integer
    */
    big_integer _absValue(big_integer num) const;

    /**
    * get the string in reverse
    * @param str string to reverse
    * @return reversed string
    */
    string _getReversedString(const string &str) const;

    /**
    * remove unwanted zeros from the end of a number
    * @param num a number
    * @return number without zeros at the end
    */
    string _removeZeros(string &num) const;

    /**
    * add num1 and num2 and return the result
    * @param num1 big_integer
    * @param num2 big_integer
    * @return sum of num1 and num2
    */
    string _add(const string &num1, const string &num2) const;

    /**
    * substract num2 from num1
    * @param num1 a nubmer
    * @param num2 a number
    * @return num1 - num2
    */
    string _substract(const string &num1, const string &num2) const;

    /**
    * get digit at index
    * @param index the index of the of the digit
    * @return the digit at place index.
    */
    int _getDigit(const string &number, unsigned long index) const;

    /**
    * multiplication of integers
    * @param num1 big_integer
    * @param num2 big_integer
    * @return num1 * num2
    */
    static big_integer _multiplication(const big_integer &num1, const big_integer &num2);

    /**
    * init data members
    */
    void _initDataMembers();

    /**
    * set larger and smaller (length of number)
    * @param num1 big_integer
    * @param num2 big_integer
    * @param larger big_integer
    * @param smaller big_integer
    */
    static void
    _setLargerAndSmaller(const big_integer &num1, const big_integer &num2, big_integer &larger,
                        big_integer &smaller);

    /**
    * if num is 0 make it positive
    * @param num big_integer
    * @return the number
    */
    big_integer &_makeZeroPositive(big_integer &result) const;
};


#endif //EX1_BIG_INTEGER_H
