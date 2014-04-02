#include "artificialintelligence.h"

using namespace ai;


Agent::Agent() :
    m_pos(glm::vec3(0.0, 0.0, 0.0)),
    m_dir(glm::vec3(0.0, 0.0, -1.0)),
    m_passive(false),
    m_team_flag(Team0),
    m_hostile_flags(0), // no hostiles
    m_target(nullptr),
    m_user(nullptr)
{

}

Agent::Agent(glm::vec3 pos, glm::vec3 dir, bool passive) :
    m_pos(pos),
    m_dir(dir),
    m_passive(passive),
    m_target(nullptr),
    m_user(nullptr)
{
    if (passive) // If this is the player set him against the world;
    {
        m_team_flag = Team1;
        m_hostile_flags = ~Team1;
    }
    else
    {
        m_team_flag = Team2;
        m_hostile_flags = ~Team2;
    }
}

void Agent::setPos(glm::vec3 pos)
{
    m_pos = pos;
}

void Agent::setDir(glm::vec3 dir)
{
    m_dir = dir;
}

glm::vec3 Agent::getPos() const
{
    return m_pos;
}

glm::vec3 Agent::getDir() const
{
    return m_dir;
}

SignalReceiver *Agent::getUserPointer()
{
    return m_user;
}

void Agent::setUserPointer(SignalReceiver* user)
{
    m_user = user;
}

Agent::~Agent()
{

}

World::World()
{
    std::cout << "creating AI world\n";

    //agents.reserve(100);
}

void World::addAgent(Agent* agent)
{
    agents.push_back(agent);
}

void World::removeAgent(Agent* agent)
{
    agents.remove(agent);
}

void World::deleteAgent(Agent* agent)
{
    agents.remove(agent);
    delete agent;
}

void World::stepAI() // as it stands O(n^2), where n is number of ai agents
{
    //int counter = 0;
    for (auto agent : agents)
    {
        if (!(agent->m_passive))
        {
            if (!(agent->m_target)) // if no target
            { // acquire target
                for (auto agent_inner : agents)
                {
                    if (agent_inner->m_team_flag & agent->m_hostile_flags) // we have a hostile
                    {
                        agent->m_target = agent_inner;
                    }
                }
            }

            if (agent->m_user)
            {
                // check if I have a target
                if (agent->m_target)
                {
////                    // if I have a target
////                    // Face it
//                    glm::vec3 from_me_2_target = (agent->m_target)->getPos() - (agent)->getPos();
//                    glm::vec3 from_me_2_target_dir = glm::normalize(from_me_2_target);
//                    glm::vec3 from_me_2_target_dir_xz = glm::normalize(glm::vec3(
//                                                            from_me_2_target_dir.x,
//                                                            0.f,
//                                                            from_me_2_target_dir.z
//                                                                       ));
//
//                    float from_me_2_target_dist = from_me_2_target.length();
////
//                    glm::vec3 this_dir = agent->getDir();
//                    glm::vec3 this_dir_xz = glm::normalize(glm::vec3(this_dir.x, 0.f, this_dir.z));
////
////                    float diff_angle = glm::acos(glm::dot(from_me_2_target_dir_xz, this_dir_xz));
////
////                    agent->m_user->rotateLeft(-diff_angle, 1.0); // second argument unused
//                    if (glm::dot(this_dir_xz, from_me_2_target_dir_xz)<0.95)
                } // if not
                else
                {
                    // relax (wait till next tick)
                }
            }
            else
            {
                std::cerr << "error: ai agent has no user\n";
            }
        }


        //
    }
}

World::~World()
{
    for (auto agent : agents)
    {
        delete agent;
    }
}
