#ifndef MARKET_H
#define MARKET_H

//#include "route.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include "tradegood.h"
#include "trader.h"
#include <cstdlib>

class Market
{
    public:
        Market();
        virtual ~Market();

        Market(std::string name_in) {initialize(name_in);};

        static void connect(Market* mark0, Market* mark1, float distance);
        static void disconnect(Market* mark0, Market* mark1);

        void coutCurrentPrices();

        std::string name;

    protected:
        bool hasNoNeighbor();

        void initialize(std::string name_in);


    private:
        struct Route
        {
            Market* dest_market;
            float travel_time; // days of 8 hour march
        };

        std::vector<Route> neighbors;



        struct GoodsListing
        {
            unsigned int category;
            float price;
        };

        // resulting local prices
        GoodsListing goods_listings[TradeGood::TRADE_GOODS_END];

        // vector of traders available

        // vector of local demands

};

#endif // MARKET_H
