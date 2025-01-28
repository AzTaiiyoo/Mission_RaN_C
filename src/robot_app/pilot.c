#include "pilot.h"
#include "robot.h"
#include "../utils.h"
#include <stdio.h>

#define DISTANCE_FORWARD 200
#define DISTANCE_TURN 242
#define OBSTACLE_THRESHOLD 200

static move_status_t robot_moving = MOVE_DONE;
static int target_pos;
static wheel_t reference_wheel;

void pilot_start_move(move_t a_move) {
    TRACE("Starting move: type=%d, distance/angle=%d, speed=%d\n",
          a_move.move_type, a_move.range.distance, a_move.speed);
    
    robot_reset_wheel_pos(); 
    
    switch(a_move.move_type) {
        case FORWARD:
            TRACE("Forward movement\n");
            reference_wheel = LEFT_WHEEL;
            target_pos = robot_get_wheel_position(reference_wheel) + 
                        a_move.range.distance * DISTANCE_FORWARD;
            robot_set_speed(a_move.speed, a_move.speed);
            robot_moving = MOVE_FORWARD;
            break;
            
        case ROTATION:
            TRACE("Rotation movement\n");
            switch(a_move.range.angle) {
                case RIGHT:
                    reference_wheel = LEFT_WHEEL;
                    target_pos = robot_get_wheel_position(reference_wheel) + DISTANCE_TURN;
                    robot_set_speed(a_move.speed, -a_move.speed);
                    break;
                case LEFT:
                    reference_wheel = RIGHT_WHEEL;
                    target_pos = robot_get_wheel_position(reference_wheel) + DISTANCE_TURN;
                    robot_set_speed(-a_move.speed, a_move.speed);
                    break;
                case U_TURN:
                    reference_wheel = RIGHT_WHEEL;
                    target_pos = robot_get_wheel_position(reference_wheel) + DISTANCE_TURN * 2;
                    robot_set_speed(-a_move.speed, a_move.speed);
                    break;
            }
            robot_moving = MOVE_ROTATION;
            break;
    }
}

move_status_t pilot_stop_at_target(void) {
    int current_pos = robot_get_wheel_position(reference_wheel);
    TRACE("Encoder position: %d, Target: %d, Moving status: %d\n", 
          current_pos, target_pos, robot_moving);
    
    if(current_pos >= target_pos) {
        TRACE("Target reached\n");
        robot_set_speed(0, 0);
        robot_moving = MOVE_DONE;
    }
    
    return robot_moving;
}