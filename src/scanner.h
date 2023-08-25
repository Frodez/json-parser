#pragma once

#ifndef yyFlexLexerOnce
#include <FlexLexer.h>
#endif

#include "parser.h"

namespace json {

    class Scanner: public yyFlexLexer {
        public:
            Scanner(std::istream *in): yyFlexLexer(in), _location{}{}
            virtual ~Scanner(){}
            json::Parser::symbol_type lex();
        private:
            json::location _location;
    };

}