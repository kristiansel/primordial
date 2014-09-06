#ifndef BACKGROUNDMASTER_H
#define BACKGROUNDMASTER_H

#include "threadiobuffers.hpp"

class BackGroundMaster
{
    public:
        BackGroundMaster(ThreadIObuffers<char, 200>::IOBuffer<1> in);
        virtual ~BackGroundMaster();
    protected:
    private:
};

#endif // BACKGROUNDMASTER_H
