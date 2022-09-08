#include "reed_solomon.h"

ReedSolomon::ReedSolomon()
{
    m_gf = GaloisField();
}

ReedSolomon::~ReedSolomon()
{
    // Empty
}

// Might use template to encode the different types (byte, numeric, alphanumeric...)
std::vector<int> 
ReedSolomon::EncodeMessage(std::vector<int> msg_in, int nsym, int fcr,
                           int generator, std::vector<int> gen)
{
    if ((msg_in.size() + nsym) > FIELD_CHAR)
    {
    	// The message is too long
    	return;
    }
    if (gen.empty())
    {
        gen = GeneratePolynomial(nsym, fcr, generator);
    }
    
    std::vector<int> msg_out(msg_in.size() + (gen.size() - 1), 0);
    
    int out_size = msg_out.size();
    for (int i = 0; i < msg_in.size(); i++)
    {
        msg_out[out_size - 1 - i] = msg_in[i];
    }

    for (int i = 0; i < msg_in.size(); i++)
    {
        int coef = msg_out[i];

        if (coef != 0)
        {
            for (int j = 0; j < gen.size(); j++)
            {
                msg_out[i + j] ^= m_gf.Multiply(gen[j], coef);
            }
        }
    }

    for (int i = 0; i < msg_in.size(); i++)
    {
        msg_out[out_size - 1 - i] = msg_in[i];
    }

    return msg_out;
}

std::vector<int> 
ReedSolomon::GeneratePolynomial(int nsym, int fcr, int generator)
{
    std::vector<int> p = { 1 };

    for (int i = 0; i < nsym; i++)
    {
        std::vector<int> q = { 1 };

        q[1] = m_gf.Power(generator, i + fcr);

        p = m_gf.MultiplyPolynomials(std::vector<int> { 1 }, q);
    }

    return p;
}

std::vector<std::vector<int>> 
ReedSolomon::GenerateAllPolynomials(int max_nsym, int fcr, int generator)
{
    std::vector<std::vector<int>> all_poly;
    all_poly[0] = all_poly[1] = { 1 };
    
    for (int nsym = 0; nsym < max_nsym; nsym++)
    {
        all_poly[nsym] = GeneratePolynomial(nsym, fcr, generator);
    }

    return all_poly;
}

