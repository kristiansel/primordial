#ifndef TOPOLOGY_H
#define TOPOLOGY_H

#include <ctime>
#include <cstring>

#include "closedmap.hpp"
#include "interpolation.hpp"

namespace wsim
{
    class Topology
    {
        public:
            Topology();
            virtual ~Topology();

            float getHeight(float x, float z); // x=0, z=0 marks the center of the world (for numerical reasons)

            bool topoGen();

            bool copyData(float* &dataPtr, unsigned int &data_size);
            bool downsizeCopyData(float* &dataPtr, unsigned int new_dim);
            bool dumpToCout(unsigned int width, unsigned int height);

            float* getDataPtr();
            unsigned int getNumData();

        protected:
            double rand_range(double a, double b);
        private:
            float m_length;
            unsigned int m_size_exponent;
            unsigned int m_dimension;

            // precalcs
            float m_halfLength;
            float m_invStride;

            int m_seed;

            ClosedMap<float> m_heightMap;

            bool m_isInit;
    };
}


#endif // TOPOLOGY_H
