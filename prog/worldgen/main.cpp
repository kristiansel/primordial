#include <iostream>

#include "world.h"

using namespace std;

int main()
{
    /**
        The purpose of this function is to generate a valid world simulation
        initial condition.

        Classes and procedures used shall provide a library for stepping the
        simulation.
    */
    cout << "Generating world...\n" << endl;

    wsim::World simworld;

    if (!simworld.worldGen())
        cerr<<"could not generate world\n";

    cout << "Finished generating world!\n" << endl;
    return 0;
}
