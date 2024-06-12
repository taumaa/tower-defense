#pragma once

class Square {
public:

    enum SquareType {
        PATH,
        IN,
        OUT,
        EMPTY,
    };

    // Empty par défaut
    Square(SquareType type = EMPTY) : type(type) {}

    SquareType type;
};

