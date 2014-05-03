#include "Game.h"
#include <Box2D\Box2D.h>

int main(){

    Game game("grasslands.json");
	game.run();
    return 0;
}