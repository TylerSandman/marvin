#include "Game.h"
#include <Box2D\Box2D.h>
#include <exception>
#include <iostream>

int main(){

    Game game;
    try {
        game.run();
    }
    catch (std::exception &e){
        std::cout << "Exception: " << e.what() << std::endl;
    }
    return 0;
}