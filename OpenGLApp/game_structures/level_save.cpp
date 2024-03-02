#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include "level_save.h"

glm::vec3 Level_save::theme0;
glm::vec3 Level_save::theme1;


void Level_save::starting_values() {
    std::ofstream myfile("save.txt");

    theme0[0] = 1;
    theme0[1] = 0;
    theme0[2] = 0;
    theme1[0] = 1;
    theme1[1] = 0;
    theme1[2] = 0;

    myfile.open("save.txt");
    if (!myfile) {
        std::cerr << "Error opening file" << std::endl;
        exit(1);
    }

    myfile << 1;
    myfile << 0;
    myfile << 0;
    myfile << 1;
    myfile << 0;
    myfile << 0;

    myfile.close();
}

void Level_save::update_state(int theme, int level) {
    if (theme == 0) {
        if (theme0[level]++ < 5) {
            theme0[level]++;
        }
    }
    else if (theme == 1) {
        if (theme1[level]++ < 5) {
            theme1[level]++;
        }
    }

    std::ofstream myfile;

    myfile.open("save.txt");
    if (!myfile) {
        std::cerr << "Error opening file" << std::endl;
        exit(1);
    }

    for (int i = 0; i < 3; i++) {
        myfile << theme0[i];
    }
    for (int i = 0; i < 3; i++) {
        myfile << theme1[i];
    }

    myfile.close();
}

void Level_save::unlock_next(int theme, int level) {
    if (theme == 0) {
        if (level < 2) {
            theme0[level++] = 1;
        }
    }
    else if (theme == 1) {
        if (level < 2) {
            theme1[level++] = 1;
        }
    }
}


void Level_save::load_state() {
    std::ifstream myfile;
    char single_digit;

    myfile.open("save.txt");
    if (!myfile) {
        starting_values();
    }
    else {
        // il file esiste
        if (!is_empty(myfile)) {
            for (int i = 0; i < 3; ++i) {
                myfile >> single_digit;
                theme0[i] = single_digit - '0';
            }
            for (int i = 0; i < 3; ++i) {
                myfile >> single_digit;
                theme1[i] = single_digit - '0';

                myfile.close();
            }
        }
            

    }
       
}
    


bool Level_save::is_empty(std::ifstream& pFile)
{
    return pFile.peek() == std::ifstream::traits_type::eof();
}
