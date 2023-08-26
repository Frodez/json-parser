#include "json_parser.h"
#include <iostream>

int main(int argc, char** argv) {
    std::string json_string { "[]" };
    auto root = json::deserialize(json_string);
    std::cout << json::serialize(root->clone()) << std::endl;
    return EXIT_SUCCESS;
}