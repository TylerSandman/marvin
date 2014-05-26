#include "SaveManager.h"

SaveManager::SaveManager(){}

SaveManager::SaveManager(SaveManager const&){}

void SaveManager::operator=(SaveManager const&){}

SaveManager* SaveManager::getInstance(){

    static std::unique_ptr<SaveManager> saveInstance(new SaveManager());
    return saveInstance.get();
}