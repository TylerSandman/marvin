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
    bool badgeEarned;
    float badgeTime;
    float bestTime;
    unsigned int requiredGems;

    LevelData(){}
    LevelData(const std::string &name, bool completed, bool collectedGem, bool badgeEarned, float badgeTime, float bestTime, unsigned int requiredGems) : 
        name(name), completed(completed), collectedGem(collectedGem), badgeEarned(badgeEarned), badgeTime(badgeTime), bestTime(bestTime), requiredGems(requiredGems) {}

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version){
        ar & name;
        ar & completed;
        ar & collectedGem;
        ar & badgeEarned;
        ar & badgeTime;
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
        if (time <= mLevelDataMap[map].badgeTime)
            mLevelDataMap[map].badgeEarned = true;
        mLastCompletedData.deaths = deaths;
        mLastCompletedData.map = map;
        mLastCompletedData.time = time;

    }

    bool isLevelCompleted(const std::string &map){
        return mLevelDataMap[map].completed;
    }

    void makeNewSave(){
        mLevelDataMap["easypeasy.json"] = LevelData("Easy Peasy", false, false, false, 2.5f, 0.0f, 0);
        mLevelDataMap["grasslands.json"] = LevelData("Grasslands", false, false, false, 3.5f, 0.0f, 1);
        mLevelDataMap["theclimb.json"] = LevelData("The Climb", false, false, false, 6.0f, 0.0f, 1);
        mLevelDataMap["boing.json"] = LevelData("Boing", false, false,  false, 99.f, 3.2f, 3);
        mLevelDataMap["spelunking.json"] = LevelData("Spelunking", false, false, false, 6.0f, 0.0f, 3);
        mLevelDataMap["ohbarnacles.json"] = LevelData("Oh Barnacles", false, false, false, 12.5f, 0.0f, 3);
        mLevelDataMap["vertigo.json"] = LevelData("Vertigo", false, false,  false, 30.f, 0.f, 5);
        mLevelDataMap["landdownunder.json"] = LevelData("Land Down Under", false, false, false, 15.f, 0.0f, 7);       
        mLevelDataMap["hivemind.json"] = LevelData("Hivemind", false, false, false, 28.f,  0.0f, 7);
        mLevelDataMap["hotpursuit.json"] = LevelData("Hot Pursuit", false, false, false, 12.5f, 0.0f, 8);
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
        mLevelPaths.push_back("easypeasy.json");
        mLevelPaths.push_back("grasslands.json");
        mLevelPaths.push_back("theclimb.json");
        mLevelPaths.push_back("boing.json");
        mLevelPaths.push_back("spelunking.json");
        mLevelPaths.push_back("ohbarnacles.json");
        mLevelPaths.push_back("vertigo.json");
        mLevelPaths.push_back("landdownunder.json");
        mLevelPaths.push_back("hivemind.json");
        mLevelPaths.push_back("hotpursuit.json");      
    }
    SaveManager(SaveManager const&){}
    void operator=(SaveManager const&){}
    std::map<std::string, LevelData> mLevelDataMap;
    std::vector<std::string> mLevelPaths;
    LevelCompletionData mLastCompletedData;
};