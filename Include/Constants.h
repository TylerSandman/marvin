#pragma once
namespace phys{

    const float MIN_JUMP_HEIGHT = 1.f;
    const float MAX_JUMP_HEIGHT = 5.f;
    const float JUMP_APEX_TIME = 0.5f;
    const float ACCELERATION_CONSTANT = 0.35f;
    const float DECELERATION_CONSTANT = 0.95f;
    const float MAX_RUN_SPEED = 10.f;

    //Physics values based on predefined constants
    const float GRAVITY = -1 * ((2 * MAX_JUMP_HEIGHT) / powf(JUMP_APEX_TIME,2.f));
    const float JUMP_VELOCITY = sqrt(fabs(2 * GRAVITY * MAX_JUMP_HEIGHT));
    const float JUMP_TERMINATION_VELOCITY = sqrt(powf(JUMP_VELOCITY,2) + 2 * GRAVITY * (MAX_JUMP_HEIGHT - MIN_JUMP_HEIGHT));
}

namespace time{
    const sf::Time FRAME_RATE = sf::seconds(1.f/60.f);
}