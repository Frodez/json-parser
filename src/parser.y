%skeleton "lalr1.cc"
%require "3.2"

%defines

%define parse.error detailed
%define parse.lac full
%define parse.assert

%define api.namespace {json}
%define api.parser.class {Parser}


%define api.token.prefix {TOKEN_}
%define api.token.raw
%define api.token.constructor
%define api.value.type variant

%locations

%parse-param { json::Scanner& scanner }
%parse-param { std::unique_ptr<json::JsonNode>& root }

%code requires {
#include <memory>
#include <unordered_map>
#include <string>
#include <sstream>
#include "json_node.h"

namespace json {
    class Scanner;
}

}

%code {
#include "scanner.h"

#undef yylex
#define yylex scanner.lex
}

%token
    END 0 "end"
    LBRACE "{"
    RBRACE "}"
    LBRACKET "["
    RBRACKET "]"
    COMMA ","
    COLON ":"
    NULL "null"
    <bool> BOOLEAN
    <double> NUMBER
    <std::string> STRING
;

%type
    <std::unique_ptr<json::JsonObject>> object
    <std::unordered_map<std::string, std::unique_ptr<json::JsonNode>>> members
    <std::pair<std::string, std::unique_ptr<json::JsonNode>>> member
    <std::unique_ptr<json::JsonArray>> array
    <std::vector<std::unique_ptr<json::JsonNode>>> values
    <std::unique_ptr<json::JsonNode>> value
;

%%

json:       value { 
                auto raw_ptr = $1.get();
                root.reset(static_cast<json::JsonNode*>(raw_ptr)); 
                $1.release();
            }
    ;

value:      object { 
                auto raw_ptr = $1.get();
                $$.reset(static_cast<json::JsonNode*>(raw_ptr)); 
                $1.release();
            }
    |       array { 
                auto raw_ptr = $1.get();
                $$.reset(static_cast<json::JsonNode*>(raw_ptr)); 
                $1.release();
            }
    |       NULL { 
                auto raw_ptr = new json::JsonNull();
                $$.reset(static_cast<json::JsonNode*>(raw_ptr)); 
            }
    |       BOOLEAN { 
                auto raw_ptr = new json::JsonBoolean($1);
                $$.reset(static_cast<json::JsonNode*>(raw_ptr)); 
            }
    |       NUMBER { 
                auto raw_ptr = new json::JsonNumber($1);
                $$.reset(static_cast<json::JsonNode*>(raw_ptr)); 
            }
    |       STRING { 
                auto raw_ptr = new json::JsonString(std::move($1));
                $$.reset(static_cast<json::JsonNode*>(raw_ptr)); 
            }

object:     LBRACE RBRACE { 
                auto raw_ptr = new json::JsonObject();
                $$.reset(static_cast<json::JsonObject*>(raw_ptr)); 
            }
    |       LBRACE members RBRACE { 
                auto raw_ptr = new json::JsonObject(std::move($2));
                $$.reset(static_cast<json::JsonObject*>(raw_ptr)); 
            }

members:    member { 
                $$.insert(std::move($1)); 
            }
    |       members COMMA member { 
                $1.insert(std::move($3));
                $$ = std::move($1); 
            }

member:     STRING COLON value { 
                $$ = {$1, std::move($3)}; 
            }
    
array:      LBRACKET RBRACKET { 
                $$ = std::make_unique<json::JsonArray>(); 
            }
    |       LBRACKET values RBRACKET { 
                $$ = std::make_unique<json::JsonArray>(std::move($2)); 
            }

values:     value { 
                $$.push_back(std::move($1)); 
            }
    |       values COMMA value { 
                $1.push_back(std::move($3)); 
                $$ = std::move($1); 
            }

%%

void json::Parser::error(const location_type &l, const std::string &err_message){
    std::stringstream ss {};
    ss << err_message << " at " << l << "\n";
    throw std::runtime_error {ss.str()};
}