#pragma once
#include <vector>
int gcd(int a, int b)
{
	while (b)
	{
		a %= b;
		std::swap(a, b);
	}
	return a;
}
int Max(int a, int b)
{
	if (a > b)
	{
		return(a);
	}
	else
	{
		return(b);
	}
}
int phi(int n)
{
    int result = n;
    for (int i = 2; i * i <= n; i++)
    {
        if (n % i == 0)
        {
            while (n % i == 0)
            {
                n /= i;
            }
            result -= result / i;
        }
    }
    if (n > 1)
    {
        result -= result / n;
    }
    return result;
}
bool IsPrime(int number)
{
    if (((!(number & 1)) && number != 2) || (number < 2) || (number % 3 == 0 && number != 3))
        return (false);

    for (int k = 1; 36 * k * k - 12 * k < number;++k)
        if ((number % (6 * k + 1) == 0) || (number % (6 * k - 1) == 0))
            return (false);
    return true;
}
std::vector<long long> PrimeGenerator(long long MaxSize)
{
    long long n = MaxSize;
    std::vector<char> is_prime(n + 1, true);
    is_prime[0] = is_prime[1] = false;
    std::vector<long long> Result = std::vector<long long>(0);
    for (long long i = 2; i <= n; i++)
    {
        if (is_prime[i] && (long long)i * i <= n)
        {
            for (int j = i * i; j <= n; j += i)
            {
                is_prime[j] = false;
            }
        }
    }
    //o optimalt sätt att göra det, men men, går igenom listan och appendar alla primtal
    for (long long i = 0; i < is_prime.size(); i++)
    {
        if (is_prime[i] == true)
        {
            Result.push_back(i);
        }
    }
    return(Result);
}