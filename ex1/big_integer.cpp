#include "big_integer.h"
#include <iostream>
#include <string>

const char MINUS_SIGN = '-';
const string MINUS_STR = "-";

/**
 * get an integer and construct a big number
 * @param number a number
 */
big_integer::big_integer(int number)
{
    if (number == 0)
    {
        _initDataMembers();
        return;
    }
    // init data
    this->_data = "";
    this->_isPositive = true;
    if (number < 0)
    {
        this->_isPositive = false;
    }
    // build number
    while (number != 0)
    {
        this->_data += std::to_string(abs(number % 10));
        number /= 10;
    }
}

/**
 * construct a big integer out of a big integer
 * @param bigNum an integer
 */
big_integer::big_integer(const big_integer &other)
{
    this->_data = string(other._data);
    this->_isPositive = other._isPositive;
}

/**
 * constructor
 * @param number a string representation of number
 */
big_integer::big_integer(const string &number)
{
    // if string len is 0 or string is "-" init with default values
    if (number.length() == 0 || number == MINUS_STR)
    {
        _initDataMembers();
        return;
    }

    // init data members
    this->_data = "";
    this->_isPositive = true;

    // put the number in data
    int end = 0;
    this->_isPositive = true;
    if (!number.empty() && number[0] == MINUS_SIGN)
    {
        end = 1;
        this->_isPositive = false;
    }
    for (long i = number.size() - 1; i >= end; i--)
    {
        if (number[i] < '0' || number[i] > '9')
        {
            _initDataMembers();
            return;
        }
        _data += number[i];
    }
    _removeZeros(_data);
    _makeZeroPositive(*this);
}


/**
 * init data members
 */
void big_integer::_initDataMembers()
{
    this->_data = "0";
    this->_isPositive = true;
}

/**
 * adds this to other and return the result
 * @param other big_integer
 * @return this + other
 */
big_integer big_integer::operator+(const big_integer &other) const
{
    big_integer res;
    res._isPositive = true;
    // if this and other are both positive or negative
    if (other._isPositive == _isPositive)
    {
        // if their sign is negative so is the sign of res
        if (!other._isPositive)
        {
            res._isPositive = false;
        }
        res._data = _add(other._data, _data);
    }
        // if this is greater or equal to other in abs value
    else if (_absValue(*this) >= _absValue(other))
    {
        // change to negative
        if (!_isPositive && *this != other)
        {
            res._isPositive = false;
        }
        res._data = _substract(_data, other._data);
    }
        // other is greater than this
    else
    {
        // if other is negative the sign is -
        if (!other._isPositive)
        {
            res._isPositive = false;
        }
        res._data = _substract(other._data, _data);
    }
    _makeZeroPositive(res);
    return res;
}

/**
 * return this - other
 * @param other big_integer
 * @return this - other
 */
big_integer big_integer::operator-(const big_integer &other) const
{
    big_integer negativeOther = big_integer(other);
    negativeOther._isPositive = !other._isPositive;
    return *this + negativeOther;
}

/**
 * multiply this to other and return the result
 * @param other big_integer
 * @return this * other
 */
big_integer big_integer::operator*(const big_integer &other) const
{
    big_integer tempNum, res = big_integer(), smaller, larger;
    _setLargerAndSmaller(*this, other, larger, smaller);
    res._isPositive = true;
    for (unsigned long i = 0; i < smaller._data.length(); i++)
    {
        big_integer digit = big_integer(_getDigit(smaller._data, i));
        tempNum = big_integer(larger);
        // long _multiplication
        for (unsigned long j = 0; j < i; j++)
        {
            tempNum._data = "0" + tempNum._data;
        }
        tempNum = _multiplication(digit, tempNum);
        res = res + tempNum;
    }
    // make the sign right
    res._isPositive = false;
    if (_isPositive == other._isPositive || res._data == "0")
    {
        res._isPositive = true;
    }
    return res;
}

/**
 * divide: this/ other
 * @param other divisor
 * @return this/ other
 */
big_integer big_integer::operator/(const big_integer &other) const
{
    string numStr = _getReversedString(_data), resStr = "";
    big_integer otherCpy = big_integer(other);
    big_integer currSum = big_integer(), zero = big_integer(), ten = big_integer("10"),
            div = big_integer();
    otherCpy._isPositive = true;
    for (unsigned long i = 0; i < numStr.length(); i++)
    {
        string temp = "";
        temp += numStr[i];
        currSum = currSum * ten;
        currSum = currSum + big_integer(temp);
        div = division(currSum, otherCpy);
        string digit = _getReversedString(div._data);
        resStr += digit;
        currSum = currSum - (div * otherCpy);
    }
    //_removeZeros(resStr);
    big_integer result = big_integer(resStr);
    _removeZeros(result._data);
    result._isPositive = _isPositive == other._isPositive;
    result = _makeZeroPositive(result);
    return result;
}

/**
 * divide: this%other
 * @param other divisor
 * @return this% other
 */
big_integer big_integer::operator%(const big_integer &other) const
{
    string numStr = _getReversedString(_data), resStr = "";
    big_integer otherCpy = big_integer(other);
    big_integer currSum = big_integer(), zero = big_integer(), ten = big_integer("10"),
            div = big_integer();
    otherCpy._isPositive = true;
    for (unsigned long i = 0; i < numStr.length(); i++)
    {
        string temp = "";
        temp += numStr[i];
        currSum = currSum * ten;
        currSum = currSum + big_integer(temp);
        div = division(currSum, otherCpy);
        string digit = _getReversedString(div._data);
        resStr += digit;
        currSum = currSum - (div * otherCpy);
    }
    currSum._isPositive = _isPositive == other._isPositive;
    currSum = _makeZeroPositive(currSum);
    if (other == 0)
    {
        currSum = big_integer();
    }
    return currSum;
}

/**
 * this function calculate (this /= other)
 * @param other big_integer
 * @return return this after doing (this /= other)
 */
big_integer &big_integer::operator/=(const big_integer &other)
{
    big_integer temp = big_integer(*this);
    temp = temp / other;
    this->_data = temp._data;
    this->_isPositive = temp._isPositive;
    return *this;
}

/**
 * put this big_integer in stream
 * @param output the stream
 * @param num the big integer to put in the stream
 * @return an updated stream
 */
ostream &operator<<(ostream &output, const big_integer &num)
{
    string numStr = "";
    if (!num._isPositive)
    {
        numStr += "-";
    }
    numStr += num._getReversedString(num._data);
    output << numStr;
    return output;
}

/**
 * if num is 0 make it positive
 * @param num big_integer
 * @return the number
 */
big_integer &big_integer::_makeZeroPositive(big_integer &num) const
{
    if (num._data == "0")
    {
        num._isPositive = true;
    }
    return num;
}


/**
 * assigns other * this to this
 * @param other
 * @return this after doing this * other
 */
big_integer &big_integer::operator*=(const big_integer &other)
{
    big_integer temp = big_integer(*this);
    temp = temp * other;
    this->_data = temp._data;
    this->_isPositive = temp._isPositive;
    return *this;
}

/**
 * this function calculate this += other
 * @param other big_integer
 * @return return this after doing this += other
 */
big_integer &big_integer::operator+=(const big_integer &other)
{
    big_integer temp = big_integer(*this);
    temp = temp + other;
    this->_data = temp._data;
    this->_isPositive = temp._isPositive;
    return *this;
}

/**
 * this method calculate this -= other
 * @param other big_integer
 * @return return this after doing this -= other
 */
big_integer &big_integer::operator-=(const big_integer &other)
{
    big_integer temp = big_integer(*this);
    temp = temp - other;
    this->_data = temp._data;
    this->_isPositive = temp._isPositive;
    return *this;
}


/**
 * compare to other big_integer for equality check.
 * @param other big_integer to compare.
 * @return true if other is equal to this object, false otherwise.
 */
bool big_integer::operator==(const big_integer &other) const
{
    return this->_data == other._data && this->_isPositive == other._isPositive;
}

/**
 * compare to other big_integer for inequality
 * @param other big_integer to compare.
 * @return true if other is not equal to this object, false otherwise.
 */
bool big_integer::operator!=(const big_integer &other) const
{
    return !(*this == other);
}

/**
 * compare the two number by > operator
 * @param other the number to compare to
 * @return compare the two number, if this > other return true, false otherwise
 */
bool big_integer::operator>(const big_integer &other) const
{
    if (_isPositive && !other._isPositive)
    {
        return true;
    }
    if (!_isPositive && other._isPositive)
    {
        return false;
    }
    if (!_isPositive)
    {
        if (_data.length() < other._data.length())
        {
            return true;
        }
        else if (_data.length() > other._data.length())
        {
            return false;
        }
        // we need to look at the reversed strings, so we could compare them
        string thisStr = _getReversedString(_data);
        string otherStr = _getReversedString(other._data);
        bool flag = (thisStr) < (otherStr);
        return flag;
    }
    if (_data.length() < other._data.length())
    {
        return false;
    }
    else if (_data.length() > other._data.length())
    {
        return true;
    }
    // we need to look at the reversed strings, so we could compare them
    string thisStr = _getReversedString(_data);
    string otherStr = _getReversedString(other._data);
    bool flag = (thisStr) > (otherStr);
    return flag;
}

/**
 * compare the two number by < operator
 * @param other the number to compare to
 * @return compare the two number, if this < other return true, false otherwise
 */
bool big_integer::operator<(const big_integer &other) const
{
    if (_isPositive && !other._isPositive)
    {
        return false;
    }
    if (!_isPositive && other._isPositive)
    {
        return true;
    }
    if (!_isPositive)
    {
        if (_data.length() < other._data.length())
        {
            return false;
        }
        else if (_data.length() > other._data.length())
        {
            return true;
        }
        // we need to look at the reversed strings, so we could compare them
        string thisStr = _getReversedString(_data);
        string otherStr = _getReversedString(other._data);
        bool flag = (thisStr) > (otherStr);//
        return flag;
    }
    if (_data.length() < other._data.length())
    {
        return true;
    }
    else if (_data.length() > other._data.length())
    {
        return false;
    }
    // we need to look at the reversed strings, so we could compare them
    string thisStr = _getReversedString(_data);
    string otherStr = _getReversedString(other._data);
    bool flag = (thisStr) < (otherStr);
    return flag;
}


/**
 * get the string in reverse
 * @param str string to reverse
 * @return reversed string
 */
string big_integer::_getReversedString(const string &str) const
{
    //todo free the memory
    string newStr = string(str);
    unsigned long len = newStr.length();
    // reverse the strings
    for (unsigned long i = 0; i < len / 2; i++)
    {
        char temp = newStr[i];
        newStr[i] = newStr[len - i - 1];
        newStr[len - i - 1] = temp;
    }
    return newStr;
}

/**
 * compare this to other using >=
 * @param other big_integer
 * @return true if this >= other, false otherwise.
 */
bool big_integer::operator>=(const big_integer &other) const
{
    return !((*this) < other);
}

/**
 * comapre this to other using <=
 * @param other big_integer
 * @return true if this <= other, false otherwise.
 */
bool big_integer::operator<=(const big_integer &other) const
{
    return !((*this) > other);
}

/**
 * add num1 and num2 and return the result
 * @param num1 big_integer
 * @param num2 big_integer
 * @return sum of num1 and num2
 */
string big_integer::_add(const string &num1, const string &num2) const
{
    unsigned long maxLen = std::max(num1.length(), num2.length());
    unsigned long i = 0;
    int num = 0;
    int carry = 0;
    string result = "";
    for (; i < maxLen; i++)
    {
        num = _getDigit(num1, i) + _getDigit(num2, i) + carry;
        if (num >= 10)
        {
            num -= 10;
            carry = 1;
        }
        else
        {
            carry = 0;
        }
        result += (char(num + '0'));
    }
    if (carry == 1)
    {
        result += "1";
    }
    if (result.empty())
    {
        return "0";
    }
    return result;
}

/**
 * substract num2 from num1
 * @param num1 a nubmer
 * @param num2 a number
 * @return num1 - num2
 */
string big_integer::_substract(const string &num1, const string &num2) const
{
    string res = "";
    int carry = 0;
    unsigned long maxLen = std::max(num1.length(), num2.length());
    for (unsigned long i = 0; i < maxLen; i++)
    {
        // _substract digits
        int sub = _getDigit(num1, i) - _getDigit(num2, i) - carry;
        // if negative, make it positive
        if (sub < 0)
        {
            sub += 10;
            carry = 1;
        }
        else
        {
            carry = 0;
        }
        res += char(sub + '0');
    }
    if (res.empty())
    {
        return "0";
    }
    return _removeZeros(res);
}

/**
 * get digit at index
 * @param index the index of the of the digit
 * @return the digit at place index.
 */
int big_integer::_getDigit(const string &number, unsigned long index) const
{
    if (index >= number.length())
    {
        return 0;
    }
    return number[index] - '0';
}

/**
 * remove unwanted zeros at the end of a number
 * @param num a number
 * @return number without zeros at the end
 */
string big_integer::_removeZeros(string &num) const
{
    while (num[num.length() - 1] == '0' && num.length() > 1)
    {
        num.pop_back();
    }
    return num;
}

/**
 * multiplication of integers
 * @param num1 big_integer
 * @param num2 big_integer
 * @return num1 * num2
 */
big_integer big_integer::_multiplication(const big_integer &num1, const big_integer &num2)
{
    // dir = direction (positive to zero or negative to zero), res = result
    big_integer res = big_integer(), dir, zero = big_integer();
    big_integer smaller = big_integer(num1), larger = big_integer(num2);
    dir = big_integer("-1");
    // smaller than zero
    if (smaller < zero)
    {
        dir = big_integer("1");
    }
    // calculate _multiplication
    while (smaller != zero)
    {
        res = res + larger;
        smaller = smaller + dir;
    }
    res._isPositive = true;
    return res;
}

/**
 * division of num1/num2 (positive sign)
 * @param num1 big_integer
 * @param num2 big_integer
 * @return num1/num2
 */
big_integer big_integer::division(const big_integer &num1, const big_integer &num2)
{
    big_integer zero = big_integer(), res = big_integer(), one = big_integer("1");
    if (num2 == zero)
    {
        return zero;
    }
    big_integer numOneCpy = big_integer(num1), numTwoCpy = big_integer(num2);
    numOneCpy._isPositive = true;
    numTwoCpy._isPositive = true;
    big_integer temp;
    // divide
    while (numOneCpy > zero)
    {
        temp = numOneCpy - numTwoCpy;
        if (temp >= zero)
        {
            res = res + one;
        }
        numOneCpy = temp;
    }
    res._isPositive = true;
    return res;
}


/**
 * set larger and smaller (length of number)
 * @param num1 big_integer
 * @param num2 big_integer
 * @param larger big_integer
 * @param smaller big_integer
 */
void big_integer::_setLargerAndSmaller(const big_integer &num1, const big_integer &num2,
                                      big_integer &larger, big_integer &smaller)
{
    if (num2._data.length() < num1._data.length())
    {
        smaller = big_integer(num2);
        larger = big_integer(num1);
    }
    else
    {
        smaller = big_integer(num1);
        larger = big_integer(num2);
    }
}

/**
 * return abs value of current integer
 * @return abs value of current integer
 */
big_integer big_integer::_absValue(big_integer num) const
{
    big_integer abs = big_integer(num);
    abs._isPositive = true;
    return abs;
}