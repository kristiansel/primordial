#ifndef TRADEWORLD_H
#define TRADEWORLD_H

#include "market.h"


class TradeWorld
{
    public:
        TradeWorld();
        virtual ~TradeWorld();

        void coutAllMarketsPrices();
    protected:
    private:
        Market* root_market;

        std::vector<Market*> markets_overview;
};

#endif // TRADEWORLD_H
