#include "topology.h"

using namespace wsim;

Topology::Topology() :
    m_size_exponent(10), // 10: 1024, 11: 2048, 12: 4096
    m_dimension(pow(2,m_size_exponent)),
    m_length(100000), // 100x100 km map, ~25 m res in dim: 4096
    m_halfLength(m_length/2),
    m_invStride(m_dimension/m_length)
{
    //ctor
}

Topology::~Topology()
{
    //dtor
}

bool Topology::copyData(float*& dataPtr, unsigned int& data_size)
{
    data_size = m_dimension*m_dimension;
    dataPtr = new float [data_size];
    float* source = m_heightMap.getDataPtr();
    memcpy ( &dataPtr[0], source, data_size);
}

bool Topology::downsizeCopyData(float*& dataPtr, unsigned int new_dim)
{
    unsigned int new_data_size = new_dim*new_dim;
    dataPtr = new float [new_data_size];

    for (int i = 0; i<new_dim; i++)
    {
        for (int j = 0; j<new_dim; j++)
        {
            float x = (float)(j)*m_length/(float)(new_dim);
            float z = (float)(i)*m_length/(float)(new_dim);

            float height = getHeight(x, z);

            dataPtr[i*new_dim+j] = height;
        }
    }
}

float* Topology::getDataPtr()
{
    return m_heightMap.getDataPtr();
}

unsigned int Topology::getNumData()
{
    return m_dimension*m_dimension;
}


double Topology::rand_range(double a, double b)
{
    double random = ((double) rand()) / (double) RAND_MAX;
    double diff = b - a;
    double r = random * diff;
    return a + r;
}

bool Topology::topoGen()
{
    // do something
    //m_seed = std::time(0);
    m_seed = 1398237320;
    //  1398237758 "strait island"
    //  1398237868 "two continents"
    //  1398333390 "two oceans"
    //  1398237320 "western reaches"
    //  1398706648 "tongue of the world"

    srand(m_seed);

    m_heightMap.reserve(m_size_exponent);

    { // height map generation

        double range = 8000.0; // m

        // find the first (middle point) m_dimension/2
        m_heightMap(m_dimension/2, m_dimension/2) = rand_range(-range, range);

//        std::cout << "first point: " << m_heightMap(m_dimension/2, m_dimension/2) << "\n";

        // find the first corner
        m_heightMap(0,0) = m_heightMap(m_dimension/2, m_dimension/2) + rand_range(-range/2.0, range/2.0);

//        std::cout << "corner point: " << m_heightMap(m_dimension, m_dimension) << "\n";
//        std::cout << "corner point: " << m_heightMap(0, 0) << "\n";

        // find the first edge points
        m_heightMap(m_dimension/2, 0) = m_heightMap(0, 0) + rand_range(-range/2.0, range/2.0);
        m_heightMap(0, m_dimension/2) = m_heightMap(0, 0) + rand_range(-range/2.0, range/2.0);

//        std::cout << "edge point: " << m_heightMap(m_dimension, m_dimension) << "\n";

        unsigned int max_size = m_dimension+1;
        unsigned int depth = 0;
        unsigned int length_scale = (max_size-1)/pow(2, depth);

        while (length_scale > 2) // each layer
        {
//            std::cout << "level===================================\n";

            length_scale = (max_size-1)/pow(2, depth);
//            std::cout << "length_scale = " << length_scale << "\n";
            unsigned int num_quads = (max_size-1)/length_scale;
//            std::cout << "num_quads = " << num_quads << "\n";

            int scale_factor = pow(2, depth);

            for (int j = 0; j<num_quads; j++)
            {
//                std::cout << "yrow========================\n";
                // update y
                unsigned int low_y = j*length_scale;
                unsigned int high_y = (j+1)*length_scale;
                for (int i = 0; i<num_quads; i++)
                {
//                    std::cout << "xrow============\n";
                    unsigned int low_x = i*length_scale;
                    unsigned int high_x = (i+1)*length_scale;

                    // fill in edges
//                    if (length_scale==2 && low_x == 32)
//                    {
//                        std::cout << "low_x = " << low_x << "\n";
//                        std::cout << "high_x = " << high_x << "\n";
//                        std::cout << "low_y = " << low_y << "\n";
//                        std::cout << "high_y = " << high_y << "\n";
//                    }


                    m_heightMap((low_x+high_x)/2, low_y) = (m_heightMap(low_x, low_y)+m_heightMap(high_x, low_y))/2.0 +
                                                 rand_range(-range/2.0/scale_factor, 0.0+range/2.0/scale_factor);
                    m_heightMap((low_x+high_x)/2, high_y) = (m_heightMap(low_x, high_y)+m_heightMap(high_x, high_y))/2.0 +
                                                 rand_range(-range/2.0/scale_factor, 0.0+range/2.0/scale_factor);
                    m_heightMap(low_x, (low_y+high_y)/2) = (m_heightMap(low_x, low_y)+m_heightMap(low_x, high_y))/2.0 +
                                                 rand_range(-range/2.0/scale_factor, 0.0+range/2.0/scale_factor);
                    m_heightMap(high_x, (low_y+high_y)/2) = (m_heightMap(high_x, low_y)+m_heightMap(high_x, high_y))/2.0 +
                                                 rand_range(-range/2.0/scale_factor, 0.0+range/2.0/scale_factor);

                    // middle
//                    m_heightMap((low_x+high_x)/2, (low_y+high_y)/2) = (m_heightMap(low_x, low_y)+m_heightMap(low_x, high_y)+m_heightMap(high_x, low_y)+m_heightMap(high_x, high_y))/4.0 +
//                                                 rand_range(-range/2.0/scale_factor/2.0, 0.0+range/2.0/scale_factor/2.0);

                    // Need to change this to make smoother middle (take all 9 surrounding into account with different weights)
                    m_heightMap((low_x+high_x)/2, (low_y+high_y)/2) = 0.414213562*(m_heightMap(low_x, low_y)+m_heightMap(low_x, high_y)+m_heightMap(high_x, low_y)+m_heightMap(high_x, high_y))/4.0
                                                 + 0.585786438*(m_heightMap((low_x+high_x)/2, low_y) + m_heightMap((low_x+high_x)/2, high_y) + m_heightMap(low_x, (low_y+high_y)/2) + m_heightMap(high_x, (low_y+high_y)/2))/4.0
                                                 + rand_range(-range/2.0/scale_factor/2.0, 0.0+range/2.0/scale_factor/2.0);
                }

            }
        depth++;
        }

        // adjust water lvl
        float av_height;
        float num_cells = m_dimension*m_dimension;

        for (int i = 0; i<m_dimension; i++)
        {
            for (int j = 0; j<m_dimension; j++)
            {
                av_height+=m_heightMap(i,j)/num_cells;
            }
        }

        for (int i = 0; i<m_dimension; i++)
        {
            for (int j = 0; j<m_dimension; j++)
            {
                m_heightMap(i,j)=m_heightMap(i,j)-av_height;
            }
        }

        // debug
        dumpToCout(79, 46); // width and height

    }

    return true;
}


float Topology::getHeight(float x, float z)
{
    // use bicubic interpolation
    // find the x and z of the top left corner
//    float top_left_x = -m_halfLength;
//    float top_left_z = -m_halfLength;
//
//    float rel_x = x-top_left_x;
//    float rel_z = z-top_left_z;

    float dec_ind_x = x*m_invStride;
    float dec_ind_z = z*m_invStride;

    int upper_x = ceil(dec_ind_x);
    int upper_z = ceil(dec_ind_z);
    int lower_x = floor(dec_ind_x);
    int lower_z = floor(dec_ind_z);

    if (upper_x == lower_x) upper_x++;
    if (upper_z == lower_z) upper_z++;

    float tx = dec_ind_x-lower_x;
    float tz = dec_ind_z-lower_z;

    float p[4][4];
    p[0][0] = m_heightMap(lower_x-1, lower_z-1);
    p[0][1] = m_heightMap(lower_x-1, lower_z);
    p[0][2] = m_heightMap(lower_x-1, upper_z);
    p[0][3] = m_heightMap(lower_x-1, upper_z+1);

    p[1][0] = m_heightMap(lower_x, lower_z-1);
    p[1][1] = m_heightMap(lower_x, lower_z);
    p[1][2] = m_heightMap(lower_x, upper_z);
    p[1][3] = m_heightMap(lower_x, upper_z+1);

    p[2][0] = m_heightMap(upper_x, lower_z-1);
    p[2][1] = m_heightMap(upper_x, lower_z);
    p[2][2] = m_heightMap(upper_x, upper_z);
    p[2][3] = m_heightMap(upper_x, upper_z+1);

    p[3][0] = m_heightMap(upper_x+1, lower_z-1);
    p[3][1] = m_heightMap(upper_x+1, lower_z);
    p[3][2] = m_heightMap(upper_x+1, upper_z);
    p[3][3] = m_heightMap(upper_x+1, upper_z+1);



    return interpolate::bicubic(p, tx, tz);

//    return m_heightScale*bilinear(tx, tz,
//                    m_height_map(lower_x, lower_z), m_height_map(upper_x, lower_z),
//                    m_height_map(lower_x, upper_z), m_height_map(upper_x, upper_z));
}

bool Topology::dumpToCout(unsigned int width, unsigned int height)
{
    unsigned int x_side = width < m_dimension? width : m_dimension;
    unsigned int z_side = height < m_dimension? height : m_dimension;

//        std::cout << "seed = " << m_seed << "m_halfLength:\n";
    for (int i = 0; i<z_side; i++)
    {
        for (int j = 0; j<x_side; j++)
        {
            float x = (float)(j)*m_length/(float)(x_side);
            float z = (float)(i)*m_length/(float)(z_side);

            float height = getHeight(x, z);

            if (height < 0.0)
            {
                std::cout << "-";
//                std::printf("\x1b[34m"     "-"     "\x1b[0m" "\n");
            }
            else if (height < 500.0)
            {
                std::cout << "=";
            }
            else if (height < 1500.0)
            {
                std::cout << "o";
            }
            else if (height < 3000.0)
            {
                std::cout << "U";
            }
            else
            {
                std::cout << "A";
            }
        }
        std::cout << "\n";
    } // finished printing debug map
}
