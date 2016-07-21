#include "lex.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* token_to_string(TokenType type)
{
    switch(type) {
#define NAME(tok) case tok: { return #tok ; }
        NAME(TOKEN_LEFTPAR);
        NAME(TOKEN_RIGHTPAR);
        NAME(TOKEN_LEFTCURLY);
        NAME(TOKEN_RIGHTCURLY);
        NAME(TOKEN_DOT);
        NAME(TOKEN_ASTRIX);
        NAME(TOKEN_SLASH);
        NAME(TOKEN_PLUS);
        NAME(TOKEN_MINUS);
        NAME(TOKEN_EQUAL);
        NAME(TOKEN_EQUALS);
        NAME(TOKEN_SEMICOLON);
        NAME(TOKEN_LT);
        NAME(TOKEN_GT);
        NAME(TOKEN_LEFTSQUARE);
        NAME(TOKEN_RIGHTSQUARE);
        NAME(TOKEN_NUMBER);
        NAME(TOKEN_STRING);
        NAME(TOKEN_IDENT);
        NAME(TOKEN_FUNC);
        NAME(TOKEN_IF);
        NAME(TOKEN_ELSE);
        NAME(TOKEN_VAR);
        NAME(TOKEN_WHILE);
        NAME(TOKEN_LTE);
        NAME(TOKEN_GTE);
        NAME(TOKEN_LOR);
        NAME(TOKEN_LAND);
        NAME(TOKEN_BOR);
        NAME(TOKEN_BAND);
        NAME(TOKEN_COMMA);
        NAME(TOKEN_NEQUALS);
        NAME(TOKEN_RETURN);
#undef NAME
    }
    
    return "Not added to token_to_string!";
}

int is_alpha(int c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int is_num(int c)
{
    return (c >= '0' && c <= '9');
}

int is_alnum(int c)
{
    return is_alpha(c) || is_num(c);
}

int is_valid_ident(int c)
{
    return is_alnum(c) || c == '_';
}

Token* make_basic_token(TokenType type, int x, int line)
{
    Token *t = malloc(sizeof(Token));
    t->type = type;
    t->x = x;
    t->line = line;
    
    return t;
}

void list_add(TokenArray *list, Token *token)
{
    list->len++;
    list->tokens = realloc(list->tokens, sizeof(Token*)*list->len);
    list->tokens[list->len-1] = token;
}

TokenArray* lex(char *data)
{
    char *ptr = data;
    
    TokenArray *list = malloc(sizeof(TokenArray));
    list->len = 0;
    list->tokens = 0;
    
    int x = 1;
    int line = 1;
    
    while(*ptr) {
        
        if((*ptr == '/') && (*(ptr+1) == '/')) {
            ptr += 2;
            while(*ptr != '\n') {
                if(*ptr == 0) {
                    break;
                }
                ptr++;
            }
            line++;
            x = 1;
            
            continue;
        }
        
        if((*ptr == '/') && (*(ptr+1) == '*')) {
            ptr += 2;
            x += 2;
            
            while(*ptr) {
                
                if(*ptr == '\r') {
                    line++;
                    ptr++;
                    x = 1;
                    
                    // TODO: Handle if ptr == null
                    if(*ptr == '\n') {
                        ptr++;
                    }
                    continue;
                }
                
                /* Is this right? */
                if(*ptr == '\n') {
                    line++;
                    x = 1;
                    ptr++;
                    continue;
                }
                
                if((*ptr == '*') && (*(ptr+1) == '/')) {
                    ptr += 2;
                    x += 2;
                    break;
                }
                
                ptr++;
                x++;
            }
            
            continue;
        }
        
        if(*ptr == ' ' || *ptr == '\t') {
            x++;
            ptr++;
            continue;
        }
        
        if(*ptr == '\r') {
            ptr++;
            x = 1;
            line++;
            
            if(*ptr == '\n') {
                ptr++;
            }
            continue;
        }
        
        if(*ptr == '\n') {
            ptr++;
            x = 1;
            line++;
            continue;
        }
             
        /* If there ever is triple tokens. Remeber to check that    */
        /* ptr+1 and ptr+2 isnt zero. Otherwise segfault will ocurr */
        
        /* If ptr+1 is 0, then theese checks are unnescesary */
#define DOUBLE_TOKEN(t1, t2, type) if((*ptr == t1) && (*(ptr+1) == t2)) { list_add(list, make_basic_token(type, x, line)); ptr += 2; x += 2; }
        DOUBLE_TOKEN('=', '=', TOKEN_EQUALS);
        DOUBLE_TOKEN('!', '=', TOKEN_NEQUALS);
        DOUBLE_TOKEN('<', '=', TOKEN_LTE);
        DOUBLE_TOKEN('>', '=', TOKEN_GTE);
        DOUBLE_TOKEN('|', '|', TOKEN_LOR);
        DOUBLE_TOKEN('&', '&', TOKEN_LAND);
#undef DOUBLE_TOKEN
        
        switch(*ptr) {
#define SINGLE_TOKEN(tok, type) case tok: { list_add(list, make_basic_token(type, x, line)); ptr++; x++; } continue;
            SINGLE_TOKEN('(', TOKEN_LEFTPAR);
            SINGLE_TOKEN(')', TOKEN_RIGHTPAR);
            SINGLE_TOKEN('{', TOKEN_LEFTCURLY);
            SINGLE_TOKEN('}', TOKEN_RIGHTCURLY);
            SINGLE_TOKEN('.', TOKEN_DOT);
            SINGLE_TOKEN(',', TOKEN_COMMA);
            SINGLE_TOKEN('*', TOKEN_ASTRIX);
            SINGLE_TOKEN('/', TOKEN_SLASH);
            SINGLE_TOKEN('+', TOKEN_PLUS);
            SINGLE_TOKEN('-', TOKEN_MINUS);
            SINGLE_TOKEN('=', TOKEN_EQUAL);
            SINGLE_TOKEN(';', TOKEN_SEMICOLON);
            SINGLE_TOKEN('<', TOKEN_LT);
            SINGLE_TOKEN('>', TOKEN_GT);
            SINGLE_TOKEN('[', TOKEN_LEFTSQUARE);
            SINGLE_TOKEN(']', TOKEN_RIGHTSQUARE);
            SINGLE_TOKEN('|', TOKEN_BOR);
            SINGLE_TOKEN('&', TOKEN_BAND);
#undef SINGLE_TOKEN
        }
        
        if(is_alpha(*ptr)) {
            char *start = ptr;
            int len = 1;
            ptr++;
            x++;
            
            while(is_valid_ident(*ptr)) {
                len++;
                ptr++;
                x++;
            }
                        
            char *text = malloc(sizeof(char)*(len+1));
            text[len] = 0;
            
            int i;
            for(i = 0; i < len; i++) {
                text[i] = start[i];
            }
            
            Token *t = malloc(sizeof(Token));
            t->x = x;
            t->line = line;
            t->type = TOKEN_IDENT;
            t->val = text;
            list_add(list, t);
            
            continue;
        }
        
        if(*ptr == '"') {
            ptr++;
            x++;
            char* start = ptr;
            int len = 0;
            while(*ptr != '"') {
                if(*ptr == 0) {
                    printf("%d:%d: Unexpected end of input while parsing string.\n", line, x);
                    exit(-1);
                }
                ptr++;
                len++;
                x++;
            }
            ptr++;
            x++;
            
            char* text = malloc(sizeof(char)*(len+1));
            text[len] = 0;
            
            int i;
            for(i = 0; i < len; i++) {
                text[i] = start[i];
            }
            
            Token *t = malloc(sizeof(Token));
            t->x = x;
            t->line = line;
            t->type = TOKEN_STRING;
            t->val = text;
            list_add(list, t);
            
            continue;
        }
        
        if(is_num(*ptr)) {
            char *start = ptr;
            int len = 1;
            ptr++;
            x++;
            
            while(is_num(*ptr)) {
                len++;
                ptr++;
                x++;
            }
            
            char *text = malloc(sizeof(char)*(len+1));
            text[len] = 0;
            
            int i;
            for(i = 0; i < len; i++) {
                text[i] = start[i];
            }
            
            Token *t = malloc(sizeof(Token));
            t->x = x;
            t->line = line;
            t->type = TOKEN_NUMBER;
            t->val = text;
            list_add(list, t);
            
            continue;
        }
        
        printf("%d:%d: Ignoring unexpected token '%c'\n", line, x, *ptr);
        ptr++;
    }
    
    int i;
    for(i = 0; i < list->len; i++) {
        Token *t = list->tokens[i];
        if(t->type != TOKEN_IDENT) continue;
#define KEYWORD(text, _type) else if(strcmp(text, t->val) == 0){t->type=_type; continue;}
        if(strcmp("func", t->val) == 0) {t->type = TOKEN_FUNC;continue;}
        KEYWORD("if", TOKEN_IF)
        KEYWORD("else", TOKEN_ELSE)
        KEYWORD("var", TOKEN_VAR)
        KEYWORD("while", TOKEN_WHILE)
        KEYWORD("return", TOKEN_RETURN);
#undef KEYWORD
    }
    
    return list;
}