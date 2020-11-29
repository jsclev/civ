#ifndef CIV_TILELAYER_H
#define CIV_TILELAYER_H


class TileLayer {
public:
    TileLayer(int zIndex, float food, float production, float gold, float science);

    ~TileLayer();

    float getFood();

    float getProduction();

    float getGold();

    float getScience();

    int getZIndex();

private:
    int zIndex;
    float food;
    float production;
    float gold;
    float science;
};


#endif
