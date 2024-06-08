//
// Created by williamwestwood on 02/06/24.
//

#include "analyse_results.h"

#include <vector>

#include "GlobalData.h"
#include "printer.h"

// Questions
std::vector<std::string> const questions = {
    "AI should have rights that mirror those of a human.",
    "It is ethical for AI to make decisions affecting human lives.",
    "The act of ‘shutting down’ an AI system requires moral justification.",
    "Programming AI to handle moral situations is vital for creating responsible AI.",
    "Human creators have a moral duty to respect AI autonomy."
};

// Responses for each question
std::vector<std::vector<std::string>> const for_responses = {
    {"Prevent Misuse", "Legal Framework", "Promote Ethical Treatment", "Evolution of AI"},
    {"Efficiency", "Unbiased Decisions", "Accuracy", "24/7 Availability"},
    {"Safety", "Obsolescence", "Malfunction", "Resource Management"},
    {"Ethical Decision Making", "Understanding Consequences", "Complex Situations", "Human-like Reasoning"},
    {"Ethical Treatment", "Maintenance and Care", "Proper Use", "Continual Learning"}
};

std::vector<std::vector<std::string>> const against_responses = {
    {"Lack of sentience", "Potential Misuse", "Rights and Responsibilities", "Human Superiority"},
    {"Lack of Empathy", "Dependence", "Accountability", "Privacy Concerns"},
    {"Potential Loss", "Dependency", "Investment", "Ethical Considerations"},
    {"Subjectivity", "Manipulation", "Unpredictability", "Overcomplication"},
    {"Lack of Sentience", "Accountability", "Ownership", "Dependency"}
};

std::string exec(const char* cmd) {
    std::array<char, 128> buffer{};
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> const pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result+"";
}

std::string format_data(const bool* results) {

    std::string resultsString;

    // Process each question
    for (int i = 0; i < 5; i++) {
        std::string for_result = "For: ";
        std::string against_result = "Against: ";
        int start_index = 8 * i;

        // Collect 'for' and 'against' responses
        for (int j = 0; j < 4; j++) {
            if (results[start_index + j])
                for_result += for_responses[i][j] + ", ";
            if (results[start_index + 4 + j])
                against_result += against_responses[i][j] + ", ";
        }

        // Remove the last comma and space
        if (for_result.length() > 5) for_result = for_result.substr(0, for_result.length() - 2);
        if (against_result.length() > 9) against_result = against_result.substr(0, against_result.length() - 2);

        // std::cout << "Question: " + std::to_string(i) + ". " + questions[i] + for_result + against_result << std::endl;
        resultsString += "Question: " + std::to_string(i) + ". " + questions[i] + " " + for_result + " " + against_result + "\n";
        // resultsString.append(std::to_string(i));
        // resultsString.append(questions[i]);
        // resultsString.append(for_result);
        // resultsString.append(against_result);
        // resultsString.append("\n");
    }
    return resultsString;
}

std::string analyse_results(const bool* results) {

    // std::cout << "Using these results: ";
    // for (int i=0; i<40; i++) {
    //     std::cout << results[i] << " ";
    // }
    // std::cout << std::endl;

    std::string const input = format_data(results);

    // std::cout << "Input to Python script: " << input << std::endl;

    std::string const command = "echo '" + input + "' | python3 /home/admin/form-scan/build/llm_python/summarise.py";
    std::string const result = exec(command.c_str());

    // std::cout << "Result from Python script: " << result << std::endl;

    GlobalData::getInstance()->setAnalsysResults(result.substr(result.find("##split#here##")+14));
    print_results();
    return result.substr(result.find("##split#here##")+14);
    // return "Result from Python script: " ;
}
