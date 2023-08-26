#pragma once

#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace json {

struct JsonNode;
struct JsonObject;
struct JsonArray;
struct JsonString;
struct JsonNumber;

struct JsonNode {
    JsonNode();
    JsonNode(JsonNode const& other);
    JsonNode& operator=(JsonNode const& other);
    virtual std::unique_ptr<JsonNode> clone();
    virtual ~JsonNode();
};

struct JsonObject final : JsonNode {
    JsonObject();
    JsonObject(std::unordered_map<std::string, std::unique_ptr<json::JsonNode>>&& attributes);
    JsonObject(JsonObject const& other);
    JsonObject& operator=(JsonObject const& other);
    std::unique_ptr<JsonNode> clone() override;
    virtual ~JsonObject();
    std::unordered_map<std::string, std::unique_ptr<json::JsonNode>> _attributes;
};

struct JsonArray final : JsonNode {
    JsonArray();
    JsonArray(std::vector<std::unique_ptr<json::JsonNode>>&& elements);
    JsonArray(JsonArray const& other);
    JsonArray& operator=(JsonArray const& other);
    std::unique_ptr<JsonNode> clone() override;
    virtual ~JsonArray();
    std::vector<std::unique_ptr<json::JsonNode>> _elements;
};

struct JsonString final : JsonNode {
    JsonString();
    JsonString(const std::string& value);
    JsonString(JsonString const& other);
    JsonString& operator=(JsonString const& other);
    std::unique_ptr<JsonNode> clone() override;
    virtual ~JsonString();
    std::string _value;
};

struct JsonNumber final : JsonNode {
    JsonNumber();
    JsonNumber(double value);
    JsonNumber(JsonNumber const& other);
    JsonNumber& operator=(JsonNumber const& other);
    std::unique_ptr<JsonNode> clone() override;
    virtual ~JsonNumber();
    double _value;
};

struct JsonBoolean final : JsonNode {
    JsonBoolean();
    JsonBoolean(bool value);
    JsonBoolean(JsonBoolean const& other);
    JsonBoolean& operator=(JsonBoolean const& other);
    std::unique_ptr<JsonNode> clone() override;
    virtual ~JsonBoolean();
    bool _value;
};

struct JsonNull final : JsonNode {
    JsonNull();
    JsonNull(JsonNull const& other);
    JsonNull& operator=(JsonNull const& other);
    std::unique_ptr<JsonNode> clone() override;
    virtual ~JsonNull();
};

};