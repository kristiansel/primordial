#ifndef ROUTE_H
#define ROUTE_H


//forward declaration of a market
class Market;

class Route
{
    public:
        Route();
        virtual ~Route();

        Route(Market* in0, Market* in1, float travel_in);

        Market* getPort0() {return port0;};
        Market* getPort1() {return port1;};
    protected:
    private:
        Market* port0;
        Market* port1;

        float base_travel_time; // days of 8 hour march

};

#endif // ROUTE_H
