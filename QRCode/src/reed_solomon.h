# pragma once

#include "galois_field.h"

class ReedSolomon
{
public:
    ReedSolomon();
    ~ReedSolomon();

    std::vector<int> EncodeMessage(std::vector<int> msg_in, int nsym, int fcr, 
                       int generator, std::vector<int> gen);

private:
    GaloisField m_gf;
    
    std::vector<int> GeneratePolynomial(int nsym, int fcr, int generator);
    std::vector<std::vector<int>> 
    GenerateAllPolynomials(int max_nsym, int fcr, int generator);
};
