#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include "level_save.h"

void Level_save::starting_values() {
    std::ofstream myfile;

    this->theme0[0] = 1;
    this->theme0[1] = 0;
    this->theme0[2] = 0;
    this->theme1[0] = 1;
    this->theme1[1] = 0;
    this->theme1[2] = 0;

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
        this->theme0[level]++;
    }
    else if (theme == 1) {
        this->theme1[level]++;
    }

    std::ofstream myfile;

    myfile.open("save.txt");
    if (!myfile) {
        std::cerr << "Error opening file" << std::endl;
        exit(1);
    }

    for (int i = 0; i < 3; i++) {
        myfile << this->theme0[i];
    }
    for (int i = 0; i < 3; i++) {
        myfile << this->theme1[i];
    }

    myfile.close();
}


void Level_save::load_state() {
    std::ifstream myfile;
    char single_digit;

    myfile.open("save.txt");
    if (!myfile) {
        std::cerr << "Error opening file" << std::endl;
        exit(1);
    }

    for (int i = 0; i < 3; ++i) {
        myfile >> single_digit;
        this->theme0[i] = single_digit - '0';
    }
    for (int i = 0; i < 3; ++i) {
        myfile >> single_digit;
        this->theme1[i] = single_digit - '0';

        myfile.close();
}