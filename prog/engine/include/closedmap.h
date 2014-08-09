#ifndef CLOSEDMAP_H
#define CLOSEDMAP_H

#include <cmath>
#include <iostream>

template <typename CellType>
class ClosedMap
{
public:
//    ClosedMap(unsigned int size_exponent)
//    {
//        m_size_exponent = size_exponent;
//        m_dimension = (int)(pow(2, m_size_exponent));
//        m_data = new CellType [m_dimension*m_dimension];
//    }

    ClosedMap(unsigned int dimension)
    {
        m_size_exponent = log2(abs(dimension));
        m_dimension = (int)(pow(2, m_size_exponent));
        m_data = new CellType [m_dimension*m_dimension];

        std::cout << "Map dimension: " << m_dimension << "\n";
    }

    virtual ~ClosedMap()
    {
        delete [] m_data;
    }

    CellType& operator()(unsigned int a, unsigned int b)
    {
        unsigned int a_mapped = a % m_dimension;
        unsigned int b_mapped = b % m_dimension;
//        std::cout << "mapped " << a_mapped << ", " << b_mapped << "\n";
        return m_data[a_mapped*m_dimension+b_mapped];
    }

    CellType* getDataPtr() {return m_data;};
    unsigned int getSizeExp() {return m_size_exponent;};
    unsigned int getDimension() {return m_dimension;};

private:
    unsigned int m_size_exponent;
    unsigned int m_dimension;
    CellType* m_data;
};
#endif // CLOSEDMAP_H
