#ifndef LEVEL_SAVE_H
#define LEVEL_SAVE_H

#include "../glm-master/glm/glm.hpp"

//0: non sbloccato
//1: sbloccato
//2: bronzo
//3: argento
//4: oro

class Level_save {
public:
    glm::vec3 theme0;
    glm::vec3 theme1;
    void starting_values();
    void update_state(int theme, int level);
    void load_state();
};


#endif

