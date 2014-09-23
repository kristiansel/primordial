#ifndef WORLD_H
#define WORLD_H

#include <string>
#include "topology.h"

namespace wsim
{
    class World
    {
        public:
            World();
            virtual ~World();

            bool worldGen();
            bool step(float dt);

            // getters
            float getHeight(float x, float z); // return the terrain height at a point in the world

            struct DateTime
            {
                int day;
                int month;
                int year;
                float hour;
            };

            DateTime getDateTime();

        protected:
            // Memory and storage
            bool serialize(void* &start_addr, unsigned int &data_size);
            bool deSerialize(void* &start_addr, unsigned int &data_size);
            bool dumpToDisk(std::string filepath);
            bool readFromDisk(std::string filepath);
            bool updateToDisk(std::string filepath);
        private:
            Topology m_topo; // Base topology
            // Weather m_weather; // Temperature, pressure, precipitation and wind
            // Geofeatures m_geofeats; // Rivers, cliffs, quarries, roads, leveled areas,
                                      // anything that further displaces the terrain
            // Climate m_climate; // biome/climate info, determines vegetation etc
            // Travellers m_travs; //
            // Economy m_econ; // economic data structure map+markets-routes map
            // Factions m_faction; // Organizations

            const int daysInYear = 360;
            const int daysInMonth = 30;

            float m_hr; // 0.0-24;
            int m_day; // 0-?;


    };
}

#endif // WORLD_H
