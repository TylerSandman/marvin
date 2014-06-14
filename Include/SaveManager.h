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
    bool collectedGem;
    float bestTime;
    unsigned int requiredGems;

    LevelData(){}
    LevelData(const std::string &name, bool completed, bool collectedGem, float bestTime, unsigned int requiredGems) : 
        name(name), completed(completed), collectedGem(collectedGem), bestTime(bestTime), requiredGems(requiredGems) {}

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version){
        ar & name;
        ar & completed;
        ar & collectedGem;
        ar & bestTime;
        ar & requiredGems;
    }
};

struct LevelCompletionData{

    LevelCompletionData(){}
    LevelCompletionData(const std::string &map, float time, int deaths) : 
        map(map), time(time), deaths(deaths){}

    std::string map;
    float time;
    int deaths;
};

class SaveManager{

public:

    static SaveManager& getInstance(){
        static SaveManager saveManager;
        return saveManager;
    }

    void markLevelCompleted(const std::string &map, float time, int deaths){
        mLevelDataMap[map].completed = true;
        if ((time < mLevelDataMap[map].bestTime) || (mLevelDataMap[map].bestTime == 0.0f))
            mLevelDataMap[map].bestTime = time;
        mLastCompletedData.deaths = deaths;
        mLastCompletedData.map = map;
        mLastCompletedData.time = time;
    }

    bool isLevelCompleted(const std::string &map){
        return mLevelDataMap[map].completed;
    }

    void makeNewSave(){
        mLevelDataMap["grasslands.json"] = LevelData("Grasslands", false, false, 0.0f, 0);
        mLevelDataMap["testmap.json"] = LevelData("Test Map", false, false, 0.0f, 0);
        mLevelDataMap["waterboy.json"] = LevelData("Waterboy", false, false,  0.0f, 0);
        mLevelDataMap["clearwalk.json"] = LevelData("Clear Walk", false, false,  0.0f, 1);
        mLevelDataMap["gofast.json"] = LevelData("Go Fast", false, false,  0.0f, 1);
        mLevelDataMap["slowdown.json"] = LevelData("Slow Down", false, false,  0.0f, 3);
        mLevelDataMap["highheights.json"] = LevelData("High Heights", false, false,  0.0f, 4);
        mLevelDataMap["hotpursuit.json"] = LevelData("Hot Pursuit", false, false,  0.0f, 4);
        mLevelDataMap["dangahzone.json"] = LevelData("Dangah Zone", false, false,  0.0f, 5);
        mLevelDataMap["dangahzone2.json"] = LevelData("Dangah Zone 2", false, false,  0.0f, 5);
        mLevelDataMap["dangahzone3.json"] = LevelData("Dangah Zone 3", false, false, 0.0f, 6);
    }

    LevelData getLevelData(const std::string &map){
        return mLevelDataMap[map];
    }

    LevelCompletionData getLastCompletedData(){
        return mLastCompletedData;
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
    LevelCompletionData mLastCompletedData;
};