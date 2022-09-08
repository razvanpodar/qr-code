#include "galois_field.h"

#include <algorithm>

GaloisField::GaloisField()
{
    InitTables();
}

GaloisField::~GaloisField()
{
    // Empty
}

std::vector<int> GaloisField::GetPrimes(int n)
{
    std::vector<int> primes;

    // TODO: Optimize
    for (int i = 1; i < n; i++)
    {
        bool is_prime = true;
        for (int j = 2; j < i; j++)
        {
            if (i % j == 0)
            {
                is_prime = false;
                break;
            }
        }
        if (is_prime)
            primes.push_back(i);
    }

    return primes;
}

std::vector<std::vector<int>> 
GaloisField::GetPrimePolynomials(int generator, int exponent, 
                                 bool fast_primes, bool single)
{
    int root_char = 2;
    const int f_char = int(pow(root_char, exponent) - 1);
    int f_char_next = int(pow(root_char, exponent + 1) - 1);

    std::vector<int> prime_candidates;

    if (fast_primes)
    {
        prime_candidates = GetPrimes(f_char_next);
        
        for (int i = 0; i < prime_candidates.size(); i++)
        {
            if (prime_candidates[i] < f_char)
                prime_candidates.erase(prime_candidates.begin() + i);
        }
    }
    else
    {
        for (int i = f_char + 2; i < f_char_next; i += root_char)
        {
            prime_candidates.push_back(i);
        }
    }

    std::vector<int> correct_primes;

    for (int i = 0; i < prime_candidates.size(); i++)
    {
        // f_char isn't constexpr so it can't be used for stack allocated arrays
        std::vector<bool> seen(f_char + 1, 10);
        bool conflict = false;

        int x = 1;
        for (int j = 0; j < f_char; j++)
        {
            // Can be swapped with an optimized implementation
            x = Multiply(x, generator);

            if (x > f_char || seen[x])
            {
                conflict = true;
                break;
            }
            else
                seen[x] = 1;
        }

        if (!conflict)
        {
            correct_primes.push_back(prime_candidates[i]);
            if (single)
            {
                //return std::vector<std::vector<int>> {prime_candidates[i]};
            }
        }
    }

    // return correct_primes;
    return std::vector<std::vector<int>>();
}

inline int GaloisField::Add(int x, int y)
{
    return x ^ y;
}

inline int GaloisField::Subtract(int x, int y)
{
    return x ^ y;
}

inline int GaloisField::Negate(int x)
{
    return x;
}

inline int GaloisField::Multiply(int x, int y)
{
    if (x == 0 || y == 0)
        return 0;
    int idx = (m_log[x] + m_log[y]) % FIELD_CHAR;
    return m_exp[idx];
}

inline int GaloisField::Divide(int x, int y)
{
    if (y == 0)
        // Should throw an exception
        return 0;
    if (x == 0)
        return 0;
    int idx = (m_log[x] + FIELD_CHAR - m_log[y]) % FIELD_CHAR;
    return m_exp[idx];
}

inline int GaloisField::Power(int x, int power)
{
    int idx = (m_log[x] * power) % FIELD_CHAR;
    return m_exp[idx];
}

inline int GaloisField::Inverse(int x)
{
    int idx = FIELD_CHAR - m_log[x];
    return m_exp[idx];
}

std::vector<int> GaloisField::ScalePolynomial(std::vector<int> p, int x)
{
    for (int i = 0; i < p.size(); i++)
    {
        p[i] = Multiply(p[i], x);
    }
    return p;
}

std::vector<int> GaloisField::AddPolynomials(std::vector<int> p, std::vector<int> q)
{
    int size = std::max(p.size(), q.size());
    std::vector<int> r(size, 0);

    for (int i = 0; i < p.size(); i++)
    {
        r[i + r.size() - p.size()] = p[i];
    }

    for (int i = 0; i < q.size(); i++)
    {
        r[i + r.size() - q.size()] ^= q[i];
    }

    return r;
}

std::vector<int> GaloisField::MultiplyPolynomials(std::vector<int> p, std::vector<int> q)
{
    int size = p.size() + q.size() - 1;
    std::vector<int> r(size, 0);

    std::vector<float> lp(p.size(), 0);

    for (int i = 0; i < p.size(); i++)
    {
        lp[i] = m_log[p[i]];
    }

    for (int i = 0; i < q.size(); i++)
    {
        int qi = q[i];
        if (qi != 0)
        {
            int lq = m_log[qi];
            for (int j = 0; j < p.size(); j++)
            {
                r[i + j] ^= m_exp[lp[i] + lq];
            }
        }
    }

    return r;
}

inline std::vector<int> GaloisField::NegatePolynomial(std::vector<int> p)
{
    return p;
}

std::vector<int> GaloisField::DividePolynomials(std::vector<int> p, std::vector<int> q)
{
    std::vector<int> message = p;

    int size = p.size() - (q.size() - 1);

    for (int i = 0; i < size; i++)
    {
        int coef = message[i];

        if (coef != 0)
        {
            for (int j = 0; j < q.size(); j++)
            {
                if (q[j] != 0)
                {
                    message[i + j] ^= Multiply(q[j], coef);
                }
            }
        }
    }

    int separator = -(q.size() - 1);

    return message;
}

int GaloisField::EvaluatePolynomial(std::vector<int> p, int x)
{
    int y = p[0];
    
    for (int i = 1; i < p.size(); i++)
    {
        y = Multiply(y, x) ^ p[i];
    }

    return y;
}

void GaloisField::InitTables(int generator, int exponent)
{
    int f_char = int(pow(2, exponent - 1));

    int x = 1;
    for (int i = 0; i < f_char; i++)
    {
        m_exp[i] = x;
        m_log[x] = i;
        x = Multiply(x, generator);
    }
    
    for (int i = f_char; i < f_char * 2; i++)
    {
        int idx = i - f_char;
        m_exp[i] = m_exp[idx];
    }
}
