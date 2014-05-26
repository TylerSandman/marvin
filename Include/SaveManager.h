#pragma once
#include <memory>

class SaveManager{

public:
    static SaveManager* getInstance();
    //Save/load here

private:
    SaveManager();
    SaveManager(SaveManager const&);
    void operator=(SaveManager const&);
};