#include "market.h"

Market::Market()
{
    //ctor
    initialize("unnamed_market");
}

Market::~Market()
{
    //dtor
    for (auto &neighbor : neighbors)
    {
        std::cout << "disconnecting " << neighbor.dest_market->name << " from " << name << "\n";
        disconnect(neighbor.dest_market, this);

//        if (neighbor.dest_market->hasNoNeighbor())
//        {
//            std::cout << "deleting " << neighbor.dest_market->name << "\n";
//            delete neighbor.dest_market;
//        }

    }
    std::cout << "deleting market " << name << "\n";
}

void Market::initialize(std::string name_in)
{
    neighbors.reserve(10);

    for (unsigned int i = TradeGood::TRADE_GOODS_BEGIN; i!=TradeGood::TRADE_GOODS_END; i++)
    {
        goods_listings[i].category = i;
        float price = 50.f*((float)(rand()%1000)/1000.f+0.001);
        //std::cout << "setting price: " << price << "\n";
        goods_listings[i].price = price;
    }

    name=name_in;
}


void Market::connect(Market* mark0, Market* mark1, float distance)
{
    mark0->neighbors.push_back({mark1, distance});
    mark1->neighbors.push_back({mark0, distance});
}

void Market::disconnect(Market* this_market, Market* from_this)
{
    this_market->neighbors.erase(
        std::remove_if(this_market->neighbors.begin(),
                       this_market->neighbors.end(),
                       [&](Route &in){
                            if (in.dest_market==from_this)std::cout << "found match\n";
                            return in.dest_market==from_this;
                       }),
        this_market->neighbors.end());

    std::cout << "neighbors of " << this_market->name << " are ";
    for (auto &neighbor : this_market->neighbors)
    {
        std::cout << neighbor.dest_market->name << " ";
    }
    std::cout << "\n";
}

bool Market::hasNoNeighbor()
{
    return (neighbors.size()==0);
}

void Market::coutCurrentPrices()
{
    std::cout << "Market: " << name << "\n";
    std::cout << "\ttradegood      \tprice" << "\n";
    for (unsigned int i = TradeGood::TRADE_GOODS_BEGIN; i!=TradeGood::TRADE_GOODS_END; i++)
    {
        std::cout << i << ":\t" <<"name\t"<< goods_listings[i].price << "\n";
    }
}

