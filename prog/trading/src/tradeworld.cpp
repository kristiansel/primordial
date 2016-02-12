#include "tradeworld.h"

TradeWorld::TradeWorld() : root_market(new Market("mainTown"))
{
    //ctor
    Market* gordonVille = new Market("gordonVille");
    Market* basuVille = new Market("basuVille");
    Market::connect(root_market, gordonVille, 5);
    Market::connect(root_market, basuVille, 8);
    Market::connect(gordonVille, basuVille, 3);

    std::cout << "number of different trade goods: " << TradeGood::TRADE_GOODS_END << "\n";

    markets_overview.push_back(root_market);
    markets_overview.push_back(gordonVille);
    markets_overview.push_back(basuVille);
}

TradeWorld::~TradeWorld()
{
    //dtor
    for (auto &market : markets_overview)
    {
        std::cout << "attempting to delete market " << market->name << "\n";
        delete market;
        market = nullptr;
        std::cout << "gets here!!!\n";
    }
}

void TradeWorld::coutAllMarketsPrices()
{
    for (auto ptr : markets_overview)
    {
        ptr->coutCurrentPrices();
    }
}
