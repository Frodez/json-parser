%option c++
%option yyclass="json::Scanner"

%option noinput nounput noyywrap
%option never-interactive

%{
#include <string>
#include <charconv>
#include "scanner.h"
#include "parser.h"

#undef YY_DECL
#define YY_DECL json::Parser::symbol_type json::Scanner::lex()

/*! Don't include unistd.h, which is not availible under MSVC++. */
#define YY_NO_UNISTD_H

/*!
 * In our case yyterminate() must return Parser::symbol_type instead of
 * 0.
 */
#define yyterminate() {return json::Parser::make_END(loc);}
%}

null null

boolean true|false

oneline [1-9]
digit [0-9]
hex [0-9A-Fa-f]
digits {digit}+
integer [-]?({oneline}{digits}|{digit})
fraction [.]{digits}
exponent [eE][-+]?{digits}
number {integer}{fraction}?{exponent}?

escape_sequence ["\\bfnrt\/]
unicode_escape_sequence u[0-9a-f]{4}
control_exclude_codepoint [^"\\\0-\32]
character ({control_exclude_codepoint}|\\({escape_sequence}|{unicode_escape_sequence}))
string \"{character}*\"

whitespaces ([\u0009\u000A\u000D\u0020])+

%%

%{
    json::Parser::location_type& loc = _location;
%}

"{" {return json::Parser::make_LBRACE(loc);}
"}" {return json::Parser::make_RBRACE(loc);}
"[" {return json::Parser::make_LBRACKET(loc);}
"]" {return json::Parser::make_RBRACKET(loc);}
"," {return json::Parser::make_COMMA(loc);}
":" {return json::Parser::make_COLON(loc);}

{null} {
    return json::Parser::make_NULL(loc);
}

{boolean} {
    bool result = std::strcmp(yytext, "true") == 0;
    return json::Parser::make_BOOLEAN(result, loc);
}

{number} {
    double number;
    std::from_chars_result result = std::from_chars(yytext, yytext + yyleng, number);
    if(result.ec == std::errc()) {
        return json::Parser::make_NUMBER(number, loc);
    } else {
        throw std::invalid_argument {std::string{"invalid number: "} + yytext};
    }
}

{string} {
    return json::Parser::make_STRING(std::string {yytext + 1, yytext + yyleng - 1}, loc);
}

{whitespaces} {
    loc.columns(yyleng);
    loc.step();
}

%%
