#include "json_node.h"

namespace json {

JsonNode::JsonNode() = default;
JsonNode::JsonNode(JsonNode const& other) = default;
JsonNode& JsonNode::operator=(JsonNode const& other) = default;
std::unique_ptr<JsonNode> JsonNode::clone() {
    auto raw_ptr = new JsonNode(*this);
    return std::unique_ptr<JsonNode>(static_cast<JsonNode*>(raw_ptr));
}
JsonNode::~JsonNode() = default;


JsonObject::JsonObject()
: JsonNode {}
, _attributes {} {
}
JsonObject::JsonObject(std::unordered_map<std::string, std::unique_ptr<json::JsonNode>>&& attributes)
: JsonNode {}
, _attributes { std::move(attributes) } {
}
JsonObject::JsonObject(JsonObject const& other)
: JsonNode {}
, _attributes {} {
    _attributes.reserve(other._attributes.size());
    for (auto it = other._attributes.begin(); it != other._attributes.end(); it++) {
        _attributes.emplace(it->first, it->second->clone());
    }
}
JsonObject& JsonObject::operator=(JsonObject const& other) {
    std::unordered_map<std::string, std::unique_ptr<json::JsonNode>> attributes {};
    attributes.reserve(other._attributes.size());
    for (auto it = other._attributes.begin(); it != other._attributes.end(); it++) {
        attributes.emplace(it->first, it->second->clone());
    }
    this->_attributes = std::move(attributes);
    return *this;
}
std::unique_ptr<JsonNode> JsonObject::clone() {
    auto raw_ptr = new JsonObject(*this);
    return std::unique_ptr<JsonNode>(static_cast<JsonNode*>(raw_ptr));
}
JsonObject::~JsonObject() = default;


JsonArray::JsonArray()
: JsonNode {}
, _elements {} {
}
JsonArray::JsonArray(std::vector<std::unique_ptr<json::JsonNode>>&& elements)
: JsonNode {}
, _elements { std::move(elements) } {
}
JsonArray::JsonArray(JsonArray const& other)
: JsonNode {}
, _elements {} {
    _elements.reserve(other._elements.size());
    for (auto it = other._elements.begin(); it != other._elements.end(); it++) {
        _elements.emplace_back((*it)->clone());
    }
}
JsonArray& JsonArray::operator=(JsonArray const& other) {
    std::vector<std::unique_ptr<json::JsonNode>> elements {};
    _elements.reserve(other._elements.size());
    for (auto it = other._elements.begin(); it != other._elements.end(); it++) {
        elements.emplace_back((*it)->clone());
    }
    this->_elements = std::move(elements);
    return *this;
}
std::unique_ptr<JsonNode> JsonArray::clone() {
    auto raw_ptr = new JsonArray(*this);
    return std::unique_ptr<JsonNode>(static_cast<JsonNode*>(raw_ptr));
}
JsonArray::~JsonArray() = default;


JsonString::JsonString() = default;
JsonString::JsonString(const std::string& value)
: JsonNode {}
, _value { value } {
}
JsonString::JsonString(JsonString const& other) = default;
JsonString& JsonString::operator=(JsonString const& other) = default;
std::unique_ptr<JsonNode> JsonString::clone() {
    auto raw_ptr = new JsonString(*this);
    return std::unique_ptr<JsonNode>(static_cast<JsonNode*>(raw_ptr));
}
JsonString::~JsonString() = default;


JsonNumber::JsonNumber() = default;
JsonNumber::JsonNumber(double value)
: JsonNode {}
, _value { value } {
}
JsonNumber::JsonNumber(JsonNumber const& other) = default;
JsonNumber& JsonNumber::operator=(JsonNumber const& other) = default;
std::unique_ptr<JsonNode> JsonNumber::clone() {
    auto raw_ptr = new JsonNumber(*this);
    return std::unique_ptr<JsonNode>(static_cast<JsonNode*>(raw_ptr));
}
JsonNumber::~JsonNumber() = default;


JsonBoolean::JsonBoolean() = default;
JsonBoolean::JsonBoolean(bool value)
: _value { value } {
}
JsonBoolean::JsonBoolean(JsonBoolean const& other) = default;
JsonBoolean& JsonBoolean::operator=(JsonBoolean const& other) = default;
std::unique_ptr<JsonNode> JsonBoolean::clone() {
    auto raw_ptr = new JsonBoolean(*this);
    return std::unique_ptr<JsonNode>(static_cast<JsonNode*>(raw_ptr));
}
JsonBoolean::~JsonBoolean() = default;


JsonNull::JsonNull() = default;
JsonNull::JsonNull(JsonNull const& other) = default;
JsonNull& JsonNull::operator=(JsonNull const& other) = default;
std::unique_ptr<JsonNode> JsonNull::clone() {
    auto raw_ptr = new JsonNull(*this);
    return std::unique_ptr<JsonNode>(static_cast<JsonNode*>(raw_ptr));
}
JsonNull::~JsonNull() = default;

}