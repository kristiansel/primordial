#ifndef BACKGROUNDMASTER_H
#define BACKGROUNDMASTER_H

#include "threadiobuffers.hpp"

class BackGroundJob
{
    int job_ID;
    enum class JobType {Job1, Job2}; // etc
    JobType job_type;
    int job_flags; // 32 flag bits to set;

    static const unsigned int BG_JOB_SIZE = 52; // bytes

    char job_data[BG_JOB_SIZE];

    // should be total 64 bytes

};

class BackGroundMaster
{
    public:
        BackGroundMaster(ThreadIObuffers<char, 200>::IOBuffer<1> in);
        virtual ~BackGroundMaster();
    protected:
    private:
};

#endif // BACKGROUNDMASTER_H
