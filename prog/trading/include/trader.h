#ifndef TRADER_H
#define TRADER_H

#include "tradegood.h"

class Trader
{
    public:
        Trader();
        virtual ~Trader();

        float cash_balance; // could have this as a part of trade goods...

        struct GoodsInventory
        {
            unsigned int category;
            float quantity; // weight, volume, number, bundles etc..
        };

        GoodsInventory goods_inventory[TradeGood::TRADE_GOODS_END];
    protected:
    private:
};

class LocalDemand // Could be food, industry, other things
// total market demand is a sum of all local demands
{
    public:
        LocalDemand();
        virtual ~LocalDemand();

        struct GoodsDemand
        {
            unsigned int category;
            float quant_ref; // relate to ideal consumption
            float price_ref; // relate to ideal spending (as percentage of income)

            float demand_elasticity; // use elasticity of demand model dQ/dP * P/Q = E
            // gives Q(P) = C*P^(E) // E is usually negative E = -1 -> elast 1%/1%
            // finally C = Q_ref/(P_ref^E)

            // shift the curve to the right for minimum needed (like food) // of they cannot afford -> starvation + unrest
        };

        GoodsDemand goods_demand[TradeGood::TRADE_GOODS_END];

    protected:
    private:
};

#endif // TRADER_H
