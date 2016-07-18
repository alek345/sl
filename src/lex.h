#ifndef LEX_H
#define LEX_H

typedef enum {
    /* Punctuation */
    TOKEN_LEFTPAR = 0, // (
    TOKEN_RIGHTPAR,    // )
    TOKEN_LEFTCURLY,   // {
    TOKEN_RIGHTCURLY,  // }
    TOKEN_LEFTSQUARE,  // [
    TOKEN_RIGHTSQUARE, // ]
    TOKEN_DOT,         // .
    TOKEN_COMMA,       // ,
    TOKEN_EQUAL,       // =
    TOKEN_SEMICOLON,   // ;
    
    /* Operators */
    TOKEN_PLUS,        // +
    TOKEN_MINUS,       // -
    TOKEN_ASTRIX,      // *
    TOKEN_SLASH,       // /
    TOKEN_BOR,         // |
    TOKEN_BAND,        // &

    /* Condition operator */
    TOKEN_EQUALS,      // ==
    TOKEN_NEQUALS,     // !=
    TOKEN_LT,          // <
    TOKEN_GT,          // >
    TOKEN_LTE,         // <=
    TOKEN_GTE,         // >=
    TOKEN_LOR,         // ||
    TOKEN_LAND,        // &&

    /* Other */
    TOKEN_NUMBER,      // 123123 /* to be supported or 0x12adf2 or 'a'*/
    TOKEN_STRING,      // "some text"
    TOKEN_IDENT,       // identifier
    
    /* Keywords */
    TOKEN_FUNC,        // func
    TOKEN_IF,          // if
    TOKEN_ELSE,        // else
    TOKEN_VAR,         // var
    TOKEN_WHILE,       // while
    TOKEN_RETURN,      // return
} TokenType;

typedef struct {
    int x;
    int line;
    
    int type;
    union {
        char *val;
    };
} Token;

typedef struct {
    Token **tokens;
    int len;
} TokenArray;

TokenArray* lex(char *text);

const char* token_to_string(TokenType type);

#endif /* LEX_H */