//
// Created by williamwestwood on 02/06/24.
//

#ifndef ANALYSE_RESULTS_H
#define ANALYSE_RESULTS_H

#include <iostream>
#include <cstdio>
#include <string>
#include <array>
#include <memory>
#include <stdexcept>

std::string exec(const char* cmd);

std::string analyse_results(const bool* results);

std::string format_data(const bool* results);



#endif //ANALYSE_RESULTS_H
