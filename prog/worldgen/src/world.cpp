#include "world.h"

using namespace wsim;

World::World()
{
    //ctor
}

World::~World()
{
    //dtor
}

bool World::worldGen()
{
    if(!m_topo.topoGen())
        return false;

    return true;
}

bool World::step(float dt)
{

}

float World::getHeight(float x, float z)
{

}

bool World::serialize(void*& start_addr, unsigned int& data_size)
{

}

bool World::deSerialize(void*& start_addr, unsigned int& data_size)
{

}

bool World::dumpToDisk(std::string filepath)
{

}

bool World::readFromDisk(std::string filepath)
{

}

bool World::updateToDisk(std::string filepath)
{

}


World::DateTime World::getDateTime()
{
    int year = m_day/daysInYear;
    int yearDay = m_day%daysInYear;

    int month = yearDay/daysInMonth + 1;
    int day = m_day%daysInMonth + 1;

    return {day, month, year, m_hr};
}
