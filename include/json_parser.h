#pragma once

#include "json_node.h"

#include <memory>
#include <string>

namespace json {

    std::unique_ptr<JsonNode> deserialize(const std::string& str);

    std::string serialize(std::unique_ptr<JsonNode>& node);

}