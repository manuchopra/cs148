#pragma once

#ifndef __LIGHT_PROPERTIES__
#define __LIGHT_PROPERTIES__

#include "common/common.h"

struct LightProperties {
    glm::vec4 cLight;
    
    // epic
    float light_radius;
    glm::vec4 point_position;
    
    glm::vec4 forward_direction;
    
    glm::vec4 sky_color;
    glm::vec4 ground_color;

};

#endif
