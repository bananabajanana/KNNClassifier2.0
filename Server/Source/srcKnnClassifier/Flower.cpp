#include "Flower.hpp"

void Flower::setType(typeIris iris) {
    type = iris;
}
typeIris Flower::getTypeOfIris() const{
    return type;
}
const NPoint Flower::getPoint() const{
    return character;
}

Flower::Flower(double sepalWidth, double sepalLength, double petalWidth, double petalLength, typeIris type)
    :character(sepalWidth, sepalLength, petalWidth, petalLength)
{
    this->type = type;
}
Flower::Flower(double sepalWidth, double sepalLength, double petalWidth, double petalLength)
        :character(sepalWidth, sepalLength, petalWidth, petalLength)
{
    type = undifined;
}