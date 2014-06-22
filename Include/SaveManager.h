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

    void markLevelCompleted(const std::string &map, float time, int deaths, bool gemCollected){
        mLevelDataMap[map].completed = true;
        if ((time < mLevelDataMap[map].bestTime) || (mLevelDataMap[map].bestTime == 0.0f))
            mLevelDataMap[map].bestTime = time;
        if (gemCollected)
            mLevelDataMap[map].collectedGem = true;
        mLastCompletedData.deaths = deaths;
        mLastCompletedData.map = map;
        mLastCompletedData.time = time;

    }

    bool isLevelCompleted(const std::string &map){
        return mLevelDataMap[map].completed;
    }

    void makeNewSave(){
        mLevelDataMap["grasslands.json"] = LevelData("Grasslands", true, true, 0.0f, 0);
        mLevelDataMap["theclimb.json"] = LevelData("The Climb", true, true, 0.0f, 0);
        mLevelDataMap["boing.json"] = LevelData("Boing", true, true,  0.0f, 2);
        mLevelDataMap["spelunking.json"] = LevelData("Spelunking", true, true,  0.0f, 2);
        mLevelDataMap["ohbarnacles.json"] = LevelData("Oh Barnacles", true, true,  0.0f, 3);
        mLevelDataMap["vertigo.json"] = LevelData("Vertigo", true, true,  0.0f, 3);
        mLevelDataMap["landdownunder.json"] = LevelData("Land Down Under", false, false,  0.0f, 4);
        mLevelDataMap["hotpursuit.json"] = LevelData("Hot Pursuit", false, false,  0.0f, 4);
        mLevelDataMap["dangahzone.json"] = LevelData("Dangah Zone", false, false,  0.0f, 5);
        mLevelDataMap["dangahzone2.json"] = LevelData("Dangah Zone 2", false, false,  0.0f, 5);
        mLevelDataMap["dangahzone3.json"] = LevelData("Dangah Zone 3", false, false, 0.0f, 17);
    }

    LevelData getLevelData(const std::string &map){
        return mLevelDataMap[map];
    }

    std::vector<std::string> getLevelPaths(){
        return mLevelPaths;
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
    SaveManager(){
        mLevelPaths.push_back("grasslands.json");
        mLevelPaths.push_back("theclimb.json");
        mLevelPaths.push_back("boing.json");
        mLevelPaths.push_back("spelunking.json");
        mLevelPaths.push_back("ohbarnacles.json");
        mLevelPaths.push_back("vertigo.json");
        mLevelPaths.push_back("landdownunder.json");
        mLevelPaths.push_back("hotpursuit.json");
        mLevelPaths.push_back("dangahzone.json");
        mLevelPaths.push_back("dangahzone2.json");
        mLevelPaths.push_back("dangahzone3.json");       
    }
    SaveManager(SaveManager const&){}
    void operator=(SaveManager const&){}
    std::map<std::string, LevelData> mLevelDataMap;
    std::vector<std::string> mLevelPaths;
    LevelCompletionData mLastCompletedData;
};