#include "Player.h"
#include "Constants.h"

enum MovementDirection{
    None,
    Left,
    Right
};

//Function object to handle jumping
std::function<void(SceneNode&, sf::Time)> PlayerJumpInitiator(){

    return [=] (SceneNode& node, sf::Time deltaTime){

        Marvin &player = static_cast<Marvin&>(node);
        b2Vec2 currentVelocity = player.getVelocity();
        if (player.isOnGround()){
            player.setVelocity(b2Vec2(currentVelocity.x, phys::JUMP_VELOCITY));  
            player.setAnimationID(Marvin::AnimationID::Jump);
        }
        
    };
}

//Function object to handle vertical acceleration in the air
std::function<void(SceneNode&, sf::Time)> PlayerJumpAccelerator(bool jumpHeld){

    return [=] (SceneNode& node, sf::Time deltaTime){

        Marvin &player = static_cast<Marvin&>(node);
        b2Vec2 currentVelocity = player.getVelocity();
        if (!jumpHeld){
            player.setVelocity(b2Vec2(
                currentVelocity.x, 
                std::min(phys::JUMP_TERMINATION_VELOCITY, currentVelocity.y)));
        }
    };
} 

//Function object to handle horizontal player movement
std::function<void(SceneNode&, sf::Time)> PlayerMoveAccelerator(float targetSpeed){

    return [=] (SceneNode& node, sf::Time deltaTime){
        Marvin &player = static_cast<Marvin&>(node);
        b2Vec2 currentVelocity = player.getVelocity();        
        b2Vec2 interpolatedVelocity = b2Vec2(
            phys::ACCELERATION_CONSTANT * targetSpeed + (1 - phys::ACCELERATION_CONSTANT) * currentVelocity.x,
            currentVelocity.y);
        player.setVelocity(interpolatedVelocity);
    };
}

//Function object to handle animation
std::function<void(SceneNode&, sf::Time)> PlayerAnimator(MovementDirection inputDirection){

    return [=] (SceneNode& node, sf::Time deltaTime){
        Marvin &player = static_cast<Marvin&>(node);
        if (inputDirection == MovementDirection::Left){
            if (player.getFacingDirection() == Marvin::FacingDirection::Right){
                player.turn();
            }
        }
        if (inputDirection == MovementDirection::Right){
            if (player.getFacingDirection() == Marvin::FacingDirection::Left){
                player.turn();
            }
        }
        if (inputDirection == MovementDirection::None){
            player.setAnimationID(Marvin::AnimationID::None);
        }
        else {
            if (player.isOnGround())
                player.setAnimationID(Marvin::AnimationID::Walk);
            else
                player.setAnimationID(Marvin::AnimationID::Jump);
        }
    };
}

void Player::handleEvent(const sf::Event &event, CommandQueue &commandQueue){

    if (event.type == sf::Event::KeyPressed){
        if (event.key.code == sf::Keyboard::W){
            Command jumpCommand;
            jumpCommand.category = Category::Player;
            jumpCommand.action = PlayerJumpInitiator();
            commandQueue.push(jumpCommand);
        }
    }

}

void Player::handleRealtimeInput(CommandQueue &commandQueue){

    //Handles horizontal movement
    Command moveCommand;
    moveCommand.category = Category::Player;

    //Handles vertical movement/jumping
    Command jumpCommand;
    jumpCommand.category = Category::Player;

    //Handles player animations
    Command animateCommand;
    animateCommand.category = Category::Player;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
        moveCommand.action = PlayerMoveAccelerator(phys::MAX_RUN_SPEED);
        animateCommand.action = PlayerAnimator(MovementDirection::Right);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
        moveCommand.action = PlayerMoveAccelerator(-1 * (phys::MAX_RUN_SPEED));
        animateCommand.action = PlayerAnimator(MovementDirection::Left);
    }
    else{
        moveCommand.action = PlayerMoveAccelerator(0);
        animateCommand.action = PlayerAnimator(MovementDirection::None);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
        jumpCommand.action = PlayerJumpAccelerator(true);
    }
    else{
        jumpCommand.action = PlayerJumpAccelerator(false);
    }      
    commandQueue.push(jumpCommand); 
    commandQueue.push(moveCommand);
    commandQueue.push(animateCommand);
}