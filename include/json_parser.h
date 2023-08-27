#pragma once

#include <memory>
#include <string>

#include "json_node.h"

namespace json {

std::unique_ptr<JsonNode> deserialize(const std::string& str);

std::string serialize(const std::unique_ptr<JsonNode>& node);

}