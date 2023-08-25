#pragma once

#include <string>
#include <memory>
#include <optional>
#include <vector>
#include <unordered_map>

namespace json
{

    struct JsonNode;
    struct JsonObject;
    struct JsonArray;
    struct JsonString;
    struct JsonNumber;

    struct JsonNode
    {
        virtual ~JsonNode(){};
    };

    struct JsonObject : JsonNode
    {
        JsonObject() : _attributes{} {}
        JsonObject(std::unordered_map<std::string, std::unique_ptr<json::JsonNode>> &&attributes) : _attributes{std::move(attributes)} {}
        virtual ~JsonObject(){};
        std::unordered_map<std::string, std::unique_ptr<json::JsonNode>> _attributes;
    };

    struct JsonArray : JsonNode
    {
        JsonArray() : _elements{} {}
        JsonArray(std::vector<std::unique_ptr<json::JsonNode>>&& elements) : _elements{std::move(elements)} {}
        virtual ~JsonArray(){};
        std::vector<std::unique_ptr<json::JsonNode>> _elements;
    };

    struct JsonString : JsonNode
    {
        JsonString(const std::string &value) : _value{value} {}
        virtual ~JsonString(){};
        std::string _value;
    };

    struct JsonNumber : JsonNode
    {
        JsonNumber(double value) : _value{value} {}
        virtual ~JsonNumber(){};
        double _value;
    };

    struct JsonBoolean : JsonNode
    {
        JsonBoolean(bool value) : _value{value} {}
        virtual ~JsonBoolean(){};
        bool _value;
    };

    struct JsonNull : JsonNode
    {
        virtual ~JsonNull(){};
    };

};