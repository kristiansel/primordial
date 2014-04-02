#include "master.h"

using namespace std;

int game()
{
    Master master;

    return master.restart();
}

int main()
{
    int restart = 1; // yes restart
    while (restart)
    {
        restart = game();
    }
    return 0;
}
