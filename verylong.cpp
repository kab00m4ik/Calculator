#include "VeryLong.h"
#include <string>

std::string VeryLong::putvl() const
{
    char temp[SZ];
    strcpy_s(temp, vlstr);
    _strrev(temp);
    return std::string(temp);
}

void VeryLong::input_vl(const char* s)
{
    strcpy_s(vlstr, s);
    vlen = strlen(s);
    _strrev(vlstr);
}

VeryLong VeryLong ::operator + (const VeryLong& other)
{
    char temp[SZ];
    int j;
    int maxlen = (vlen > other.vlen) ? vlen : other.vlen;
    int carry = 0;

    if (vlstr[vlen - 1] == '-') {
        if (other.vlstr[other.vlen - 1] == '-')
            return((other).negate() + (*this).negate()).negate();
        else
            return(other)-(*this).negate();
    }
    else if (other.vlstr[other.vlen - 1] == '-')
    {
        return(*this) - (other).negate();
    }

    for (j = 0; j < maxlen; j++)
    {
        int d1 = (j > vlen - 1) ? 0 : vlstr[j] - '0';
        int d2 = (j > other.vlen - 1) ? 0 : other.vlstr[j] - '0';
        int digitsum = d1 + d2 + carry;
        if (digitsum >= 10)
        {
            digitsum -= 10; carry = 1;
        }
        else
            carry = 0;
        temp[j] = digitsum + '0';
    }
    if (carry == 1)
        temp[j++] = '1';
    temp[j] = '\0';
    return VeryLong(temp);
}

VeryLong VeryLong ::operator * (const VeryLong& other)
{
    if (vlstr[vlen - 1] == '-')
    {
        if (other.vlstr[other.vlen - 1] == '-')
            return(other).negate() * (*this).negate();
        else
            return((*this).negate() * (other)).negate();
    }
    else if (other.vlstr[other.vlen - 1] == '-')
    {
        return((other).negate() * (*this)).negate();
    }
    VeryLong num1 = VeryLong((*this).vlstr);
    VeryLong num2 = VeryLong(other.vlstr);

    int point1 = 0, point2 = 0, i = 0;
    //Якщо в числах є крапка або кома, то вона забирається і множиться без неї
    for (int j = 0; j < vlen || j < num2.vlen; j++)
    {
        if (num1.vlstr[j] == '.' || num1.vlstr[j] == ',')
        {
            point1 = j;

            for (i = j; i < num1.vlen; i++)
            {
                num1.vlstr[i] = num1.vlstr[i + 1];
            }
            num1.vlen = i - 1;
        }
        if (num2.vlstr[j] == '.' || num2.vlstr[j] == ',')
        {
            point2 = j;

            for (i = j; i < num2.vlen; i++)
            {
                num2.vlstr[i] = num2.vlstr[i + 1];
            }
            num2.vlen = i - 1;
        }
    }

    VeryLong pprod;
    VeryLong tempsum;
    for (int j = 0; j < num2.vlen; j++)
    {
        int digit = num2.vlstr[j] - '0';
        pprod = num1.multdigit(digit);
        for (int k = 0; k < j; k++)
            pprod = num1.mult10(pprod);
        tempsum = tempsum + pprod;
    }
    //Якщо в числах була крапка або кома, то додає її в потрібне місце в результаті
    if (point1 + point2 != 0)
    {
        tempsum.vlen += 1;
        for (int j = tempsum.vlen; j > point1 + point2; j--)
        {
            tempsum.vlstr[j] = tempsum.vlstr[j - 1];
        }
        tempsum.vlstr[point1 + point2] = '.';
        //Видалення зайвих нулів на початку числа
        i = tempsum.vlen;
        while (i > 1 && tempsum.vlstr[i - 1] == '0')
        {
            if (tempsum.vlstr[i - 2] != '.')
            {
                tempsum.vlstr[i - 1] = '\0';
                --i;
            }
            else
                break;
        }
        tempsum.vlen = i;
        //Видалення зайвих нулів вкінці числа
        _strrev(tempsum.vlstr);
        while (i > 1 && tempsum.vlstr[i - 1] == '0')
        {
            tempsum.vlstr[i - 1] = '\0';
            --i;
        }
        if (tempsum.vlstr[i - 1] == '.')
        {
            tempsum.vlstr[i - 1] = '\0';
            --i;
        }
        _strrev(tempsum.vlstr);
        tempsum.vlen = i;
    }

    return tempsum;
}

VeryLong VeryLong::operator - (const VeryLong& other) const
{
    char result[SZ];
    int borrow = 0;
    int i = 0;

    if (vlstr[vlen - 1] == '-')
    {
        if (other.vlstr[other.vlen - 1] == '-')
            return (other).negate() - (*this).negate();
        else
            return ((*this).negate() + (other)).negate();
    }
    else if (other.vlstr[other.vlen - 1] == '-')
    {
        return(other).negate() + (*this);
    }

    if (compare(other) < 0)
    {
        return (other - *this).negate();
    }

    for (i = 0; i < vlen || i < other.vlen; ++i)
    {
        int digit1 = (i < vlen) ? (vlstr[i] - '0') : 0;
        int digit2 = (i < other.vlen) ? (other.vlstr[i] - '0') : 0;

        int diff = digit1 - digit2 - borrow;
        if (diff < 0)
        {
            diff += 10;
            borrow = 1;
        }
        else
        {
            borrow = 0;
        }

        result[i] = diff + '0';
    }

    result[i] = '\0';

    while (i > 1 && result[i - 1] == '0')
    {
        result[i - 1] = '\0';
        --i;
    }
    return VeryLong(result);
}

VeryLong VeryLong::operator / (const VeryLong& denominator)
{
    if (vlstr[vlen - 1] == '-')
    {
        if (denominator.vlstr[denominator.vlen - 1] == '-')
            return (*this).negate() / (denominator).negate();
        else
            return ((*this).negate() / denominator).negate();
    }
    else if (denominator.vlstr[denominator.vlen - 1] == '-')
    {
        return (*this / (denominator).negate()).negate();
    }
    if (denominator.compare(VeryLong("0")) == 0)
    {
        cerr << "Division by zero is not allowed." << endl;
        exit(1);
    }

    VeryLong numerator = VeryLong((*this).vlstr);
    VeryLong integer;
    int count = 0;

    while (numerator.compare(denominator) >= 0)
    {
        numerator = numerator - denominator;
        count++;
    }

    integer = VeryLong(count);

    if (numerator.compare(VeryLong("0")) == 0)
        return integer;
    else
    {
        VeryLong fractional = VeryLong(".");

        while (numerator.compare(denominator) == -1)
        {
            if (numerator.compare(VeryLong("0")) == 0)
                return integer.add(fractional);

            numerator = numerator.add(VeryLong("0"));

            if (numerator.compare(denominator) == -1)
            {
                fractional = fractional.add(VeryLong("0"));
            }
            else
            {
                int count2 = 0;
                if (fractional.vlen < 10)
                {
                    while (numerator.compare(denominator) >= 0)
                    {
                        numerator = numerator - denominator;
                        count2++;
                    }
                    fractional = fractional.add(count2);
                }
            }
        }
        return integer.add(fractional);
    }
}

VeryLong VeryLong::multdigit(const int d2) const
{
    char temp[SZ];
    int j, carry = 0;
    for (j = 0; j < vlen; j++)
    {
        int d1 = vlstr[j] - '0';
        int digitprod = d1 * d2;
        digitprod += carry;
        if (digitprod >= 10)
        {
            carry = digitprod / 10;
            digitprod -= carry * 10;
        }
        else
            carry = 0;
        temp[j] = digitprod + '0';
    }
    if (carry != 0)
        temp[j++] = carry + '0';
    temp[j] = '\0';
    return VeryLong(temp);
}

VeryLong VeryLong::mult10(const VeryLong other) const
{
    char temp[SZ];
    for (int j = other.vlen - 1; j >= 0; j--)
        temp[j + 1] = other.vlstr[j];
    temp[0] = '0';
    temp[other.vlen + 1] = '\0';
    return VeryLong(temp);
}

int VeryLong::compare(const VeryLong& other) const
{
    VeryLong num1 = VeryLong((*this).vlstr);
    VeryLong num2 = VeryLong((other).vlstr);
    int point1 = 0, point2 = 0, i;

    for (int j = 0; j < vlen || j < num2.vlen; j++)
    {
        if (num1.vlstr[j] == '.' || num1.vlstr[j] == ',')
        {
            point1 = j;

            for (i = 0; i <= point1; ++i)
            {
                num1.vlen = num1.vlen - 1;
            }
        }
        if (num2.vlstr[j] == '.' || num2.vlstr[j] == ',')
        {
            point2 = j;

            for (i = 0; i <= point2; ++i)
            {
                num2.vlen = num2.vlen - 1;
            }
        }
    }

    if (num1.vlen > num2.vlen)
    {
        return 1;
    }
    else if (num1.vlen < num2.vlen)
    {
        return -1;
    }
    else
    {
        int n = vlen - 1;
        int m = other.vlen - 1;

        for (; n >= 0 || m >= 0; --n, --m)
        {
            if (vlstr[n] > other.vlstr[m])
            {
                return 1;
            }
            else if (vlstr[n] < other.vlstr[m])
            {
                return -1;
            }
        }
        return 0;
    }
}

VeryLong VeryLong::negate() const
{
    char result[SZ];
    int i = 0;

    for (i = 0; i < vlen; ++i)
    {
        result[i] = vlstr[i];
    }

    if (result[i - 1] == '-')
    {
        result[i - 1] = '\0';
    }
    else
    {
        result[i] = '-';
        result[i + 1] = '\0';
    }

    return VeryLong(result);
}

VeryLong VeryLong::add(const VeryLong& other) const
{
    char temp[SZ];
    int j = 0;
    for (; j < other.vlen; j++)
    {
        temp[j] = other.vlstr[j];
    }
    for (int i = 0; i < vlen; i++, j++)
    {
        temp[j] = vlstr[i];
    }
    temp[j] = '\0';
    return VeryLong(temp);
}
