#pragma once
#include <memory>
#include <map>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/map.hpp>

class SaveManager{

public:
    static SaveManager* getInstance(){
        static std::unique_ptr<SaveManager> saveInstance(new SaveManager());
        return saveInstance.get();
    }
    void markLevelCompleted(const std::string &levelName){
        mLevelCompletionMap[levelName] = true;
    }
    bool isLevelCompleted(const std::string &levelName){
        return mLevelCompletionMap[levelName];
    }


private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive &ar, const unsigned int version){
        ar & mLevelCompletionMap;
    }
    SaveManager(){
        mLevelCompletionMap["Grasslands"] = false;
        mLevelCompletionMap["Test Map"] = false;
        mLevelCompletionMap["Waterboy"] = false;
        mLevelCompletionMap["Clear Walk"] = false;
        mLevelCompletionMap["Go Fast"] = false;
        mLevelCompletionMap["Slow Down"] = false;
        mLevelCompletionMap["High Heights"] = false;
        mLevelCompletionMap["Hot Pursuit"] = false;
        mLevelCompletionMap["Dangah Zone"] = false;
        mLevelCompletionMap["Dangah Zone 2"] = false;
        mLevelCompletionMap["Dangah Zone 3"] = false;
    }
    SaveManager(SaveManager const&){}
    void operator=(SaveManager const&){}
    std::map<std::string, bool> mLevelCompletionMap;
};