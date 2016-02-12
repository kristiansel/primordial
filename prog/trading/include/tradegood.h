#ifndef TRADEGOOD_H
#define TRADEGOOD_H


class TradeGood
{
    public:
        TradeGood();
        virtual ~TradeGood();

        enum Category {     TRADE_GOODS_START, // 0

                            Wood, // 1
                            Stone,
                            Clay,
                            Brick,
                            Blocks, // stone merge?
                            Planks,
                            Grain,
                            // Flour, too local
                            Fruit,
                            // Bread, local see grain
                            Beer,
                            Liqor, // quite technologically advanced
                            // Seed, local (see grain)
                            // Meat, too local (see cattle)
                            Hides, // fur
                            Oil,
                            Cloth, // Linen, silk, wool, cotton local production
                            Coal, // mined or charcoal
                            Ore, // copper, tin, iron, silver, gold
                            Gems, // cut and combined with metals for jewellery
                            Metal, // ingots, dust, wreaths: copper, tin, iron, silver, gold, alloys
                            Tools,
                            Weapons,
                            Jewellery,
                            Armor, // metal, leather
                            Pottery,
                            Hemp, // plant fibers
                            Paper,
                            Containers, // Baskets, crates, barrels
                            Horses, // camels, elephants
                            Cattle,
                            Sheep,
                            Pig,
                            Chicken,
                            Fish,
                            Slaves,

                            LAST_TRADE_GOODS
                        };
        static constexpr unsigned int TRADE_GOODS_BEGIN = TRADE_GOODS_START+1;
        static constexpr unsigned int TRADE_GOODS_END = LAST_TRADE_GOODS-1;

        TradeGood(Category category_in) {category = category_in;}

        Category category;
        // Material
        // Distinctions

        //float quantity; // Number/weight etc...

    protected:
    private:
};

#endif // TRADEGOOD_H
