#ifndef ARTIFICIALINTELLIGENCE_H
#define ARTIFICIALINTELLIGENCE_H

//#include "packedlinkedlist.hpp"
#include <list>
#include <iostream>
#include <glm/glm.hpp>
#include "signals.h"
#include <cstdlib>

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
private:
    Agent();
public:
    Agent(glm::vec3 pos, glm::vec3 dir, bool passive = false);

    virtual ~Agent();

    void setPos(glm::vec3 pos);
    void setDir(glm::vec3 dir);
    glm::vec3 getPos() const;
    glm::vec3 getDir() const;

    // send interrupt signal
    void interrupt(); // induce a temporary lapse in AI control (where it waits)

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

    // AI properties
    static constexpr float pursuit_stop_dist = 1.5; // m
    static constexpr float pursuit_start_dist = 2.0; // m
    static constexpr float run_stop_dist = 3.0; // m
    static constexpr float run_start_dist = 4.0; // m
    static constexpr float interact_dist = pursuit_start_dist; // m
    float m_reactionTime;   // an AI parameter for average reaction time


    // book-keeping
    float m_reactionWait;   // an internal book-keeping variable to measure how long it has left to wait

    // state flags (can be bit-packed later)
    bool m_isPursuing;
    bool m_isRunning;
//    int m_randomWait;

    // private methods

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

    void stepAI(float dt); // really should not rely on default arg here

private:
    //PackedFlagList<Agent> agents;
    std::list<Agent*> agents;
};

}
#endif // ARTIFICIALINTELLIGENCE_H
