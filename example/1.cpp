#include <iostream>
#include "json_parser.h"

int main(int argc, char** argv) {
    std::string json_string {"[]"};
    auto root = json::deserialize(json_string);
    std::cout << json::serialize(root) << std::endl;
    return EXIT_SUCCESS;
}