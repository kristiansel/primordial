#ifndef TIMESERIES_H
#define TIMESERIES_H

#include <iostream>

template <class ValueType> struct TimeValuePair
{
    /// Uninitialized
    float time;
    ValueType value;
};

template <class ValueType> class TimeSeries /// Struct like
{
    public:
        struct SeekResult
        {
            struct KeyWeightPair
            {
                TimeValuePair<ValueType>* key;
                float weight;
            };
            KeyWeightPair prev;
            KeyWeightPair next;
        };
    public:
        TimeSeries() : num_keys(0),
                       keys(nullptr),
                       duration(0.f) {};
        ~TimeSeries() {delete[] keys;};

//        void init(int num_keys, float duration)
//            {this->num_keys = num_keys;
//             delete[] keys;
//             keys = new TimeValuePair [num_keys];
//             this->duration = duration;} ;

        SeekResult seekPrev(float time, int hint = -1);

//        TimeValuePair getKey(int ind_in)
//        {
//            if (ind_in < 0 || !(ind_in<num_keys))
//            {
//                std::cerr << "TimeSeries error: attempted to read key out of bounds\n";
//                return {0.0, ValueType()};
//            }
//            else return keys[ind_in];
//
//        }
//        void setKey(int ind_in, TimeValuePair key_in)
//        {
//            if (ind_in < 0 || !(ind_in<num_keys))
//            {
//                std::cerr << "TimeSeries error: attempted to write key out of bounds\n";
//            }
//            else keys[ind_in] = key_in;
//        }

//        int getNumKeys() {return num_keys;};

        int num_keys;
        TimeValuePair<ValueType>* keys;
        float duration;
};

/// Definition of beefy functions

template <class ValueType>                              /// Template parameters
typename TimeSeries<ValueType>::SeekResult              /// Return type
TimeSeries<ValueType>::seekPrev(float time, int hint)   /// Function signature
{
    /// This function will be called a lot, so might become the bottleneck


    /// Modulate time
    time = time - int(time/duration) * duration;

    TimeValuePair<ValueType>* prev = &(keys[0]);

    if (hint < 0 || !(hint < num_keys)) /// If no/bad hint is given
        hint = (int)((num_keys-1) * time / duration);  /// Start search by assuming uniform distribution
        /// The above guarantees that hint is maximum num_pos_keys - 1

//        std::cout << "num_pos_keys = " << num_pos_keys << "\n";
//        std::cout << "time / ch_duration = " << time / ch_duration << "\n";
//        std::cout << "hint = " << hint << "\n";
    /// First check if last frame?
    /// To eliminate complications further down
    //if ()

    bool found = false;
    int cand = hint;
    while (!found)
    {
//            std::cout << "cand = " << cand << "\n";
        /// check if time is between candidate and next
        bool timeGrCand = (time > keys[cand].time-0.00000001);
        if (timeGrCand) /// If success so far, proceed
        {
            if (cand==num_keys-1)
            {
                prev = &(keys[cand]); /// Time is greater than the last key... should not happen
                found = true;
            }
            else
            {
                /// if not returned here, this is safe
                bool timeLsNext = (time < keys[cand+1].time+0.00000001);
                if (timeLsNext)
                {
                    prev = &(keys[cand]);
                    found = true;
                }
                else
                    cand+=1;
            }
        }
        else /// Linear search for next (step one down)
        {
            cand-=1;
        }
    }
//    }

    SeekResult res;
    res.prev.key = prev;

    if (prev==&(keys[num_keys-1]))
    {
        res.next.key = &(keys[0]);
        res.next.weight = (time-prev->time)/(duration-prev->time);
    }
    else
    {
        res.next.key = &(prev[1]);
        res.next.weight = (time-prev->time)/(res.next.key->time-prev->time);
    }

    res.prev.weight = 1.0-res.next.weight;
    return res;
//    return prev;
}



#endif // TIMESERIES_H
