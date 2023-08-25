#include <iostream>
#include "json_parser.h"

int main(int argc, char** argv) {
    for(int i = 1; i < argc; i++) {
        std::cout << "input:" << std::endl << argv[i] << std::endl;
        auto root = json::deserialize(argv[i]);
        std::cout << "output:" << std::endl << json::serialize(root) << std::endl;
    }
    return EXIT_SUCCESS;
}