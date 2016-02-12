#include "route.h"

Route::Route() :
    port0(nullptr), port1(nullptr), base_travel_time(1.0)
{
    //ctor
}

Route::~Route()
{
    //dtor
}

Route::Route(Market* in0, Market* in1, float travel_in)
{
    port0 = in0;
    port1 = in1;
    base_travel_time = travel_in;
}
