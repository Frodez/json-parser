#include "json_parser.h"
#include <iostream>

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        std::cout << "input:" << std::endl << argv[i] << std::endl;
        auto root = json::deserialize(argv[i]);
        std::cout << "output:" << std::endl << json::serialize(root->clone()) << std::endl;
    }
    return EXIT_SUCCESS;
}