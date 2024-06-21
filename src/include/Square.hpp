#pragma once



class Square {
public:

    enum SquareType {
        PATH,
        IN,
        OUT,
        EMPTY,
    };

    bool isHovered = false;

    // en haut à gauche
    int uiX;
    int uiY;

    // Empty par défaut
    Square(SquareType type, int uiX, int uiY) {
        this->type = type;
        this->uiX = uiX;
        this->uiY = uiY;
    }

    SquareType type;

    void render();

};

