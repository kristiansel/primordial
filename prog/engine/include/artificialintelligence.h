#ifndef ARTIFICIALINTELLIGENCE_H
#define ARTIFICIALINTELLIGENCE_H

//#include "packedlinkedlist.hpp"
#include <list>
#include <iostream>
#include <glm/glm.hpp>
#include "signals.h"

namespace ai
{

typedef unsigned char TeamFlag;
const TeamFlag Team0 = 1 << 0;
const TeamFlag Team1 = 1 << 1;
const TeamFlag Team2 = 1 << 2;
const TeamFlag Team3 = 1 << 3;
const TeamFlag Team4 = 1 << 4;
//
//class World;

class Agent
{
friend class World;
public:
    Agent();
    Agent(glm::vec3 pos, glm::vec3 dir, bool passive = false);

    virtual ~Agent();

    void setPos(glm::vec3 pos);
    void setDir(glm::vec3 dir);
    glm::vec3 getPos() const;
    glm::vec3 getDir() const;

    SignalReceiver* getUserPointer();
    void setUserPointer(SignalReceiver* user);

private:
    TeamFlag m_team_flag;
    TeamFlag m_hostile_flags;
    const Agent * m_target;

    SignalReceiver* m_user;

    bool m_passive;

    glm::vec3 m_pos;
    glm::vec3 m_dir;
};

class World
{
public:
    World();
    virtual ~World();

    void addAgent(Agent* agent);
    void removeAgent(Agent* agent);

    // on the side
    void deleteAgent(Agent* agent);

    void stepAI();

private:
    //PackedFlagList<Agent> agents;
    std::list<Agent*> agents;
};

}
#endif // ARTIFICIALINTELLIGENCE_H
