#pragma once
#include <memory>
#include <map>
#include <iostream>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/map.hpp>

struct LevelData{
    std::string name;
    bool completed;
    float bestTime;

    LevelData(){}
    LevelData(const std::string &name, bool completed, float bestTime) : 
        name(name), completed(completed), bestTime(bestTime){}

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version){
        ar & name;
        ar & completed;
        ar & bestTime;
    }
};

class SaveManager{

public:

    static SaveManager& getInstance(){
        static SaveManager saveManager;
        return saveManager;
    }

    void markLevelCompleted(const std::string &map, float time){
        mLevelDataMap[map].completed = true;
        if ((time < mLevelDataMap[map].bestTime) || (mLevelDataMap[map].bestTime == 0.0f))
            mLevelDataMap[map].bestTime = time;
    }

    bool isLevelCompleted(const std::string &map){
        return mLevelDataMap[map].completed;
    }

    void makeNewSave(){
        mLevelDataMap["grasslands.json"] = LevelData("Grasslands", false, 0.0f);
        mLevelDataMap["testmap.json"] = LevelData("Test Map", false, 0.0f);
        mLevelDataMap["waterboy.json"] = LevelData("Waterboy", false, 0.0f);
        mLevelDataMap["clearwalk.json"] = LevelData("Clear Walk", false, 0.0f);
        mLevelDataMap["gofast.json"] = LevelData("Go Fast", false, 0.0f);
        mLevelDataMap["slowdown.json"] = LevelData("Slow Down", false, 0.0f);
        mLevelDataMap["highheights.json"] = LevelData("High Heights", false, 0.0f);
        mLevelDataMap["hotpursuit.json"] = LevelData("Hot Pursuit", false, 0.0f);
        mLevelDataMap["dangahzone.json"] = LevelData("Dangah Zone", false, 0.0f);
        mLevelDataMap["dangahzone2.json"] = LevelData("Dangah Zone 2", false, 0.0f);
        mLevelDataMap["dangahzone3.json"] = LevelData("Dangah Zone 3", false, 0.0f);
    }

    LevelData getLevelData(const std::string &map){
        return mLevelDataMap[map];
    }

private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive &ar, const unsigned int version){
        ar & mLevelDataMap;
    }
    SaveManager(){}
    SaveManager(SaveManager const&){}
    void operator=(SaveManager const&){}
    std::map<std::string, LevelData> mLevelDataMap;
};