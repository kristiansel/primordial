#ifndef STATS_H
#define STATS_H

#include <string>
#include <iostream>

using namespace std;

struct Stat { // physical stats changes and effects should be determined from weight... and strength development from how much you eat
    string name;
    string description;

    Stat() {
        name = "stat";
        description = "description";

        day_value = 10.0;
        current_value = 10.0;
        projected_value = 10.0;

        dcurrent_dt = 0; // change in current value per second >= 0; This should go to zero with tiredness
        dcurrent_duse = 0; // change in current value per use
        dprojected_dt = 0; // change in projected value per second; This should be negative and go to zero with tiredness
        dprojected_duse = 0;
    }

    float day_value; // constant throughout the day. Regen stops here and at 0
    float current_value; // the value available, used for calculations
    float projected_value; // if fell asleep now, this would be the

    float dcurrent_dt; // change in current value per second >= 0; This should go to zero with tiredness
    float dcurrent_duse; // change in current value per use
    float dprojected_dt; // change in projected value per second; This should be negative and go to zero with tiredness
    float dprojected_duse;  // change in projected value per use; This should be positive and go to zero with tiredness

    void updateNow(float dt) { // assume constant dependence on time...
        if (current_value < day_value)
        {
            current_value += dt*dcurrent_dt;
        }
        else if (current_value > day_value)
        {
            current_value = day_value;
        }

        projected_value += dt*dprojected_dt;

    }
    void use(float effort) { // assume constant dependence on current value...
        current_value += dcurrent_duse*effort;
        projected_value += dprojected_duse*effort;
    }
    void updateDay() {
        day_value = projected_value;
        current_value = day_value;
    };
    void print() {
        cout<<name<<": \n";
        cout<<description<<" \n";
        cout<<"value = "<<current_value<<"\n";
        cout<<"for day = "<<day_value<<"\n";
        cout<<"projected = "<<projected_value<<"\n";
        cout<<"\n";
    }
};


class Stats
{
    public:
        Stats();
        virtual ~Stats();

        Stat strength;
        Stat agility;
        Stat endurance;
        Stat instinct;
        Stat reason;
        Stat confidence;

        float weight; // total weight of animal, body weight plus carrying
        float fatigue; // weariness, sleepiness, tiredness. Determines ability to sleep and improve stats and skills.


    protected:
    private:
};

#endif // STATS_H
