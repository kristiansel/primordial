#ifndef TIMESERIES_H
#define TIMESERIES_H

#include <iostream>

template <class ValueType> class TimeSeries /// Struct like
{
    public:
        struct TimeValuePair
        {
            /// Uninitialized
            float time;
            ValueType value;
        };

        TimeSeries() : num_keys(0),
                       keys(nullptr),
                       duration(0) {};
        ~TimeSeries() {delete[] keys;};

        void init(int num_keys, float duration)
            {this->num_keys = num_keys;
             delete[] keys;
             keys = new TimeValuePair [num_keys];
             this->duration = duration;} ;

        TimeValuePair* seekPrev(float time, int hint = -1);

        TimeValuePair getKey(int ind_in)
        {
            if (ind_in < 0 || !(ind_in<num_keys))
            {
                std::cerr << "TimeSeries error: attempted to read key out of bounds\n";
                return {0.0, ValueType()};
            }
            else return keys[ind_in];

        }
        void setKey(int ind_in, TimeValuePair key_in)
        {
            if (ind_in < 0 || !(ind_in<num_keys))
            {
                std::cerr << "TimeSeries error: attempted to write key out of bounds\n";
            }
            else keys[ind_in] = key_in;
        }

        int getNumKeys() {return num_keys;};

    private:
        int num_keys;
        TimeValuePair* keys;
        float duration;
};

/// Definition of beefy functions
template <class ValueType>                              /// Template parameters
typename TimeSeries<ValueType>::TimeValuePair*          /// Return type
TimeSeries<ValueType>::seekPrev(float time, int hint)   /// Function signature
{
    /// RIGHT NOW CLAMPING (COULD PERIODISE)
    if (time < 0) /// This is clamping (might create unsmooth looping)
        return &(keys[0]); /// The first key_frame
    if ( !(time < duration) ) /// This is clamping (might create unsmooth looping)
        return &(keys[num_keys - 1]);
    else
    {
        if (hint < 0 || !(hint < num_keys)) /// If no/bad hint is given
            hint = (int)((num_keys-1) * time / duration);  /// Start search by assuming uniform distribution
            /// The above guarantees that hint is maximum num_pos_keys - 1

//        std::cout << "num_pos_keys = " << num_pos_keys << "\n";
//        std::cout << "time / ch_duration = " << time / ch_duration << "\n";
//        std::cout << "hint = " << hint << "\n";
        /// First check if last frame?
        /// To eliminate complications further down
        //if ()

        int cand = hint;
        while (true)
        {
//            std::cout << "cand = " << cand << "\n";
            /// check if time is between candidate and next
            bool timeGrCand = (time > keys[cand].time-0.00000001);
            if (timeGrCand) /// If success so far, proceed
            {
                if (cand==num_keys-1)
                    return &(keys[cand]); /// Time is greater than the last key... should not happen

                /// if not returned here, this is safe
                bool timeLsNext = (time < keys[cand+1].time+0.00000001);
                if (timeLsNext)
                    return &(keys[cand]);
                else
                    cand+=1;
            }
            else /// Linear search for next (step one down)
            {
                cand-=1;
            }
        }
    }
}

#endif // TIMESERIES_H
