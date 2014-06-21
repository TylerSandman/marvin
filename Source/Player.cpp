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
            b2Vec2 velocity(currentVelocity.x, phys::JUMP_VELOCITY);
            player.setVelocity(velocity);
            player.setControlledVelocity(velocity);
        }
    };
}

//Function object to handle vertical acceleration in the air
std::function<void(SceneNode&, sf::Time)> PlayerJumpAccelerator(bool jumpHeld){

    return [=] (SceneNode& node, sf::Time deltaTime){

        Marvin &player = static_cast<Marvin&>(node);
        b2Vec2 currentVelocity = player.getVelocity();
        if ((!jumpHeld) && (!player.isOnGround())){
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
        GrassPlatform *platform = player.getAttachedPlatform();
        b2Vec2 interpolatedControlledVelocity(
            phys::ACCELERATION_CONSTANT * targetSpeed + (1 - phys::ACCELERATION_CONSTANT) * player.getControlledVelocity().x,
            currentVelocity.y);
        if (platform){
            b2Vec2 interpolatedPlatformVelocity(
                interpolatedControlledVelocity.x + platform->getVelocity().x,
                currentVelocity.y);
            player.setVelocity(interpolatedPlatformVelocity);
        }
        else{
            player.setVelocity(interpolatedVelocity);       
        }
        player.setControlledVelocity(interpolatedControlledVelocity);
    };
}

//Function object to handle animation
std::function<void(SceneNode&, sf::Time)> PlayerAnimator(MovementDirection inputDirection){

    return [=] (SceneNode& node, sf::Time deltaTime){
        Marvin &player = static_cast<Marvin&>(node);
        if (inputDirection == MovementDirection::Left){
            player.turn(Entity::FacingDirection::Left);
        }
        if (inputDirection == MovementDirection::Right){
            player.turn(Entity::FacingDirection::Right);
        }
        if (inputDirection == MovementDirection::None){
            player.stopAnimation();
        }
    };
}

void Player::handleEvent(const sf::Event &event, CommandQueue &commandQueue){

    if (event.type == sf::Event::KeyPressed){
        if (event.key.code == sf::Keyboard::Space){
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

    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
        (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)))){
        moveCommand.action = PlayerMoveAccelerator(phys::MAX_RUN_SPEED);
        animateCommand.action = PlayerAnimator(MovementDirection::Right);
    }
    else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
             (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)))){
        moveCommand.action = PlayerMoveAccelerator(-1 * (phys::MAX_RUN_SPEED));
        animateCommand.action = PlayerAnimator(MovementDirection::Left);
    }
    else{
        moveCommand.action = PlayerMoveAccelerator(0);
        animateCommand.action = PlayerAnimator(MovementDirection::None);
    }
    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Space))){
        jumpCommand.action = PlayerJumpAccelerator(true);
    }
    else{
        jumpCommand.action = PlayerJumpAccelerator(false);
    }      
    commandQueue.push(jumpCommand); 
    commandQueue.push(moveCommand);
    commandQueue.push(animateCommand);
}