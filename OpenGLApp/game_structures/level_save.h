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
    static glm::vec3 theme0;
    static glm::vec3 theme1;
    static void starting_values();
    static void update_state(int theme, int level, int phase);
    static void unlock_next(int theme, int level);
    static void load_state();
    static bool is_empty(std::ifstream& pFile);
};


#endif

