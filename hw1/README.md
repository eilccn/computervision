### Results

std::pow(i,2): 0.002381s (result = 3.33333e+07)
i*i: 0.000315s (result = 3.33333e+07)

std::pow(i,3): 0.002357s (result = 2.5e+07)
i*i*i: 0.000311s (result = 2.5e+07)

### Given your results, explain whether there is ever a justified use of the pow function to square a value. When is using the pow function justified?

Given how the results show that the pow function is much more computationally expensive than simple multiplication, there should never be a justified use of the pow function for simply squaring a value. This is because pow operates with real floating point numbers and computes pow(x,y) = e^(y log(x)). Since this computation is not even carrying out simple multiplication, this may result in incorrect values. In select cases, the pow() function may be justified when the exponent is either quite large, or not an integer. However, even for large exponents, we can do the calculation by squaring or cubing multiple times, and it will be faster than pow(). An example of a more optimal way of raising an integer to a large power is exponentiation by squaring:

int ipow(int base, int exp)
{
    int result = 1;
    for (;;)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (!exp)
            break;
        base *= base;
    }

    return result;
}