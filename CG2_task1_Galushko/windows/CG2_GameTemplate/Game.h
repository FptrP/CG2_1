#pragma once

#include "stl.h"
#include <SFML/Graphics.hpp>
#include "Renderer.h"
#include "GameResources.h"

enum Window_size
{
    WIDTH=1440,
    HEIGHT=1080,
    CELL_X=20,
    CELL_Y=15
};
namespace game
{

class GameLevel
{

};

extern std::atomic<bool> gEndGame;

struct GameResults
{
public:
    enum CompleteStatus
    {
        FAILED,
        COMPLETE
    }
    status;
};

class Game
{
public:

    void Init(const stl::shared_ptr<sf::RenderWindow> &window);
    void Release();

    GameResults StartLevel(const char *levelFilename);
protected:

    stl::map<stl::string, unsigned> texNames;
    stl::shared_ptr<sf::RenderWindow> window;
};

}