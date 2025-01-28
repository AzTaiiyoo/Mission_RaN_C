#include <stdbool.h>
#include <stdlib.h> 
#include <string.h>
#include "copilot.h"
#include "pilot.h"

#define AVOIDANCE_STEPS 4
static const move_t avoidance_path[AVOIDANCE_STEPS] = {
    {ROTATION, {LEFT}, 20},    
    {FORWARD, {2}, 30},         
    {ROTATION, {RIGHT}, 20},    
    {FORWARD, {2}, 30}          
};

static path_status_t path_status = PATH_DONE;
static int path_step;
static move_t* path = NULL;
static int current_step;
static bool in_avoidance = false;
static int avoidance_step = 0;
static move_t interrupted_move; 

void set_path_config(path_cfg_t config) {
    if (path != NULL) {
        free(path);
    }
    path_step = config.step_count;
    path = (move_t*)calloc(path_step, sizeof(move_t));
    if (path != NULL) {
        memcpy(path, config.moves, path_step * sizeof(move_t));
    }
}

void copilot_start_path(void) {
    if (path == NULL) {
        path_status = PATH_ERROR;
        return;
    }
    current_step = 0;
    in_avoidance = false;
    avoidance_step = 0;
    path_status = PATH_MOVING;
    pilot_start_move(path[current_step]);
}

path_status_t copilot_stop_at_step_completion(void) {
    move_status_t move_status = pilot_stop_at_target();
    
    switch (move_status) {
        case MOVE_DONE:
            if (in_avoidance) {
                
                avoidance_step++;
                if (avoidance_step >= AVOIDANCE_STEPS) {
                    
                    in_avoidance = false;
                    pilot_start_move(interrupted_move);
                } else {
                    // Continue le chemin d'évitement
                    pilot_start_move(avoidance_path[avoidance_step]);
                }
            } else {
               
                current_step++;
                if (current_step >= path_step) {
                    path_status = PATH_DONE;
                } else {
                    pilot_start_move(path[current_step]);
                }
            }
            break;
            
        case MOVE_OBSTACLE_FORWARD:
            if (!in_avoidance) {
               
                interrupted_move = path[current_step];
                in_avoidance = true;
                avoidance_step = 0;
                pilot_start_move(avoidance_path[avoidance_step]);
            } else {
                
                avoidance_step = 0;
                pilot_start_move(avoidance_path[avoidance_step]);
            }
            path_status = PATH_OBSTACLE;
            break;
            
        default:
            break;
    }
    
    return path_status;
}

bool copilot_is_path_complete(void) {
    return path_status == PATH_DONE;
}

void copilot_wait_user_input(move_t user_move) {
    
}