#ifndef CLOSEDMAP_HPP
#define CLOSEDMAP_HPP

#include <cmath>
#include <iostream>

template <typename CellType>
class ClosedMap
{
public:
    ClosedMap() : m_size_exponent(0), m_dimension(0), m_data(nullptr) {};
    ClosedMap(unsigned int size_exponent)
    {
        reserve(size_exponent);
    }

    void reserve(unsigned int size_exponent)
    {
        if (!m_data)
        {
            m_size_exponent = size_exponent;
            m_dimension = (int)(pow(2, m_size_exponent));
            m_data = new CellType [m_dimension*m_dimension];
        }
        else
            std::cerr << "Error: attempted to reserve already reserved closedmap\n";
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
#endif // CLOSEDMAP_HPP

