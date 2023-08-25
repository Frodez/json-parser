#include <sstream>

#include "json_parser.h"

#include "scanner.h"

namespace json
{

    std::unique_ptr<JsonNode> deserialize(const std::string &str);
    std::string serialize(std::unique_ptr<JsonNode>& node);
    void serialize(const JsonNode* node, std::stringstream& stream);

    std::unique_ptr<JsonNode> deserialize(const std::string &str)
    {
        std::unique_ptr<JsonNode> root {nullptr};
        std::stringstream stream{str};
        Scanner scanner{&stream};
        Parser parser {scanner, root};
        parser.parse();
        return root;
    }

    std::string serialize(std::unique_ptr<JsonNode>& node) {
        std::stringstream stream {};
        serialize(node.get(), stream);
        return stream.str();
    }

    void serialize(const JsonNode* node, std::stringstream& stream) {
        if(auto object = dynamic_cast<const JsonObject*>(node)) {
            stream << '{';
            auto it = object->_attributes.begin();
            if(it != object->_attributes.end()) {
                stream << '"' << it->first << '"' << ':';
                serialize(it->second.get(), stream);
                while(++it != object->_attributes.end()) {
                    stream << ',' << '"' << it->first << '"' << ':';
                    serialize(it->second.get(), stream);
                }
            }
            stream << '}';
        } else if(auto array = dynamic_cast<const JsonArray*>(node)) {
            stream << '[';
            auto it = array->_elements.begin();
            if(it != array->_elements.end()) {
                serialize(it->get(), stream);
                while(++it != array->_elements.end()) {
                    stream << ',';
                    serialize(it->get(), stream);
                }
            }
            stream << ']';
        } else if(auto string = dynamic_cast<const JsonString*>(node)) {
            stream << '"' << string->_value << '"';
        } else if(auto number = dynamic_cast<const JsonNumber*>(node)) {
            stream << number->_value;
        } else if(auto boolean = dynamic_cast<const JsonBoolean*>(node)) {
            stream << (boolean->_value ? "true" : "false");
        } else if(dynamic_cast<const JsonNull*>(node) != nullptr) {
            stream << "null";
        }
    }

}