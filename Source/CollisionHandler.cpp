#include "Command.h"
#include "CollisionHandler.h"
#include "Marvin.h"
#include "Logger.h"
#include "World.h"

CollisionHandler::CollisionHandler(World &world) : 
    mWorld(world){}

void CollisionHandler::BeginContact(b2Contact *contact){

    //User data is defined as a scene node
    auto firstNode = static_cast<SceneNode*>(
        contact->GetFixtureA()->GetBody()->GetUserData());
    auto secondNode = static_cast<SceneNode*>(
        contact->GetFixtureB()->GetBody()->GetUserData());

    //No user data means the nodes don't require further collision handling
    if (!firstNode || !secondNode) return;

    SceneNode::Pair collisionPair(firstNode, secondNode);
    CommandQueue& commandQueue = mWorld.getCommandQueue();

     //Movable terrain collisions
     if ((matchesCategories(collisionPair, Category::Type::Player, Category::Type::Walkable)) &&
         (matchesCategories(collisionPair, Category::Type::Player, Category::Type::Enemy))){
        
        //Player stepping on top
        if ((contact->GetFixtureA()->IsSensor()) && 
            (firstNode->getCategory() & Category::Type::Player)){
            Marvin &player = static_cast<Marvin&>(*firstNode);
            player.setNumFootContacts(player.getNumFootContacts() + 1);
        }           
        else if ((contact->GetFixtureB()->IsSensor()) &&
                 (secondNode->getCategory() & Category::Type::Player)){
            Marvin &player = static_cast<Marvin&>(*secondNode);
            player.setNumFootContacts(player.getNumFootContacts() + 1);
        }

        //Block on top of Player
        if ((contact->GetFixtureA()->IsSensor()) && 
            (firstNode->getCategory() & Category::Type::Enemy)){
            mWorld.requestReset();
        }           
        else if ((contact->GetFixtureB()->IsSensor()) &&
                 (secondNode->getCategory() & Category::Type::Enemy)){
            mWorld.requestReset();
        }
    }

    //Player and damagers
    else if ((matchesCategories(collisionPair, Category::Type::Player, Category::Type::Damager))||
        (matchesCategories(collisionPair, Category::Type::Player, Category::Type::Enemy))){
        mWorld.requestReset();
    }



    //Terrain collisions.
    else if (matchesCategories(collisionPair, Category::Type::Player, Category::Type::Walkable)){
        
        //Is our foot sensor touching walkable terrain?
        if ((contact->GetFixtureA()->IsSensor()) && 
            (firstNode->getCategory() & Category::Type::Player)){
            Marvin &player = static_cast<Marvin&>(*firstNode);
            player.setNumFootContacts(player.getNumFootContacts() + 1);
        }           
        else if ((contact->GetFixtureB()->IsSensor()) &&
                 (secondNode->getCategory() & Category::Type::Player)){
            Marvin &player = static_cast<Marvin&>(*secondNode);
            player.setNumFootContacts(player.getNumFootContacts() + 1);
        }
    }

    else if (matchesCategories(collisionPair, Category::Type::Player, Category::Type::Exit)){
        mWorld.requestCompletion();
    }
}

void CollisionHandler::EndContact(b2Contact *contact){

    //User data is defined as a scene node
    auto firstNode = static_cast<SceneNode*>(
        contact->GetFixtureA()->GetBody()->GetUserData());
    auto secondNode = static_cast<SceneNode*>(
        contact->GetFixtureB()->GetBody()->GetUserData());

    //No user data means the nodes don't require further collision handling
    if (!firstNode || !secondNode) return;

    SceneNode::Pair collisionPair(firstNode, secondNode);
    CommandQueue& commandQueue = mWorld.getCommandQueue();

    //Terrain collisions. Assumes no terrains have sensors (may need to reimplement)
    if ((matchesCategories(collisionPair, Category::Type::Player, Category::Type::Walkable))||
        ((matchesCategories(collisionPair, Category::Type::Player, Category::Type::Walkable))&&
        (matchesCategories(collisionPair, Category::Type::Player, Category::Type::Enemy)))){
        //Has our foot sensor left walkable terrain?
        if (contact->GetFixtureA()->IsSensor()){
            Marvin &player = static_cast<Marvin&>(*firstNode);
            player.setNumFootContacts(player.getNumFootContacts() - 1);
        }           
        else if (contact->GetFixtureB()->IsSensor()){
            Marvin &player = static_cast<Marvin&>(*secondNode);
            player.setNumFootContacts(player.getNumFootContacts() - 1);
        }
    }
}

bool CollisionHandler::matchesCategories(SceneNode::Pair &nodes, unsigned int type1, unsigned int type2){
    
    unsigned int cat1 = nodes.first->getCategory();
    unsigned int cat2 = nodes.second->getCategory();

    if ((cat1 & type1) && (cat2 & type2))
        return true;
    else if ((cat1 & type2) && (cat2 & type1)){
        std::swap(nodes.first, nodes.second);
        return true;
    }
    return false;
}