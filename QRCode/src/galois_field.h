#pragma once

#include <vector>
#include <array>

constexpr int GF_SIZE = 256;
const int FIELD_CHAR = int(pow(2, 8) - 1);

class GaloisField
{
public:
    GaloisField();
    ~GaloisField();

    // IDEA: Change vector to array<int, 8>

    std::vector<int> GetPrimes(int n);
    std::vector<std::vector<int>> GetPrimePolynomials(
            int generator = 2, 
            int exponent = 8,
            bool fast_primes = false,
            bool single = false
    );
    int Add(int x, int y);
    int Subtract(int x, int y);
    int Negate(int x);
    int Multiply(int x, int y);
    int Divide(int x, int y);
    int Power(int x, int power);
    int Inverse(int x);
    std::vector<int> ScalePolynomial(std::vector<int> p, int x);
    std::vector<int> AddPolynomials(std::vector<int> p, std::vector<int> q);
    std::vector<int> MultiplyPolynomials(std::vector<int> p, std::vector<int> q);
    std::vector<int> NegatePolynomial(std::vector<int> p);
    std::vector<int> DividePolynomials(std::vector<int> p, std::vector<int> q);
    int EvaluatePolynomial(std::vector<int> p, int x);

private:
    std::array<int, 2 * GF_SIZE> m_exp = { 0 };
    std::array<int, GF_SIZE + 1> m_log = { 0 };

    void InitTables(int generator = 2, int exponent = 8);
};