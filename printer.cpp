//
// Created by williamwestwood on 02/06/24.
//

#include "printer.h"
#include "GlobalData.h"
#include <cstdlib>
#include <string>
#include <thread>
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

    const std::string cmd = "python3 /home/admin/form-scan/build/print_python/print_receipt.py -results \"" + scaleResults + "\" -analysis \"" + analysis + "\"";

    std::cout << "Printing receipt... " << cmd.c_str() << std::endl;


    GlobalData::getInstance()->setProgressText("Analysis complete, printing...");

    system(cmd.c_str());

    std::this_thread::sleep_for(std::chrono::seconds(5));

    GlobalData::getInstance()->setProgressText("Please take your results from the printer.");

    std::this_thread::sleep_for(std::chrono::seconds(5));

    GlobalData::getInstance()->setShowProgress(false);
}
