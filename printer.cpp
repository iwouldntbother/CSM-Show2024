//
// Created by williamwestwood on 02/06/24.
//

#include "printer.h"
#include "GlobalData.h"
#include <cstdlib>
#include <string>
#include <iostream>

void print_results() {

    std::string const analysis = GlobalData::getInstance()->getAnalsysResults();
    const bool* rawResults = GlobalData::getInstance()->getResultsData();

    std::string scaleResults;
    for (int i=0; i<5; i++) {
        int const idx = i*8;
        int const forVal = rawResults[idx] + rawResults[idx+1] + rawResults[idx+2] + rawResults[idx+3];
        int const againstVal = rawResults[idx+4] + rawResults[idx+5] + rawResults[idx+6] + rawResults[idx+7];
        int const finalVal = forVal + (againstVal*-1);
        scaleResults += std::to_string(finalVal) + "|";
    }

    std::cout << "Printing results:\n" << scaleResults << "\n" << analysis << std::endl;

    const std::string cmd = "python print_python/print_receipt.py -results \"" + scaleResults + "\" -analysis \"" + analysis + "\"";

    std::cout << "Running command: " << cmd << std::endl;
    std::cout << "Printing receipt... " << cmd.c_str() << std::endl;

    system(cmd.c_str());
}
