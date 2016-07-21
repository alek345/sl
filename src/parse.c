#include "parse.h"
#include <stdio.h>
#include <stdlib.h>

static Token *current;
static TokenArray *tokens;
static int token_offset;

void next()
{
    token_offset++;
    if(token_offset >= tokens->len) {
        current = 0;
        return;
    }
    
    current = tokens->tokens[token_offset];
}

int accept(TokenType type)
{
    if(type == current->type) {
        next();
        return 1;
    }
    return 0;
}

int expect(TokenType type)
{
    if(accept(type)) {
        return 1;
    }
    
    printf("%d:%d: Expected %s\n", current->line, current->x, token_to_string(type));
    exit(-1);
    return 0;
}

Node* expression();

Node* factor()
{
    Token *start = current;
    if(accept(TOKEN_IDENT)) {
        
        if(accept(TOKEN_LEFTSQUARE)) {
            
            Node *expr = expression();
            expect(TOKEN_RIGHTSQUARE);
            
            return make_table_read_node(make_variable_node(start->val), expr);
            
        } else if(accept(TOKEN_LEFTPAR)) {
            
            NodeArray *args = nodearray_new();
            
            do {
                nodearray_add(args, expression());
            } while(accept(TOKEN_COMMA));
            
            expect(TOKEN_RIGHTPAR);
            
            return make_funccall_node(start->val, args);
            
        } else {
            return make_variable_node(start->val);
        }
        
    } else if(accept(TOKEN_NUMBER)) {
        return make_constant_node(CONSTANT_NUMBER, start->val);
    } else if (accept(TOKEN_LEFTPAR)) {
        Node *expr = expression();
        expect(TOKEN_RIGHTPAR);
        return expr;
    } else if (accept(TOKEN_LEFTSQUARE)) {
        expect(TOKEN_RIGHTSQUARE);
        return make_constant_node(CONSTANT_TABLE, "empty_table");
    } else if(accept(TOKEN_STRING)) {
        return make_constant_node(CONSTANT_STRING, start->val);
    } else {
        printf("%d:%d: Expected ident, number, string or '('\n", current->line, current->x);
        exit(-1);
        return NULL;
    }
}

Node* term()
{
    Node *lhs = factor();
    
    while(current->type == TOKEN_ASTRIX || current->type == TOKEN_SLASH) {
        Token *op = current;
        next();
        Node *rhs = factor();
        if(op->type == TOKEN_ASTRIX) {
            lhs = make_binop_node(BINOP_MUL, lhs, rhs);
        } else {
            lhs = make_binop_node(BINOP_DIV, lhs, rhs);
        }
    }
    
    return lhs;
}

Node* expression()
{
    // -1 no, 0 plus, 1 minus
    int plusorminus = -1;
    if(current->type == TOKEN_PLUS) {
        next();
        plusorminus = 0;
    } else if (current->type == TOKEN_MINUS) {
        next();
        plusorminus = 1;
    }
    
    Node *lhs = term();
    while(current->type == TOKEN_PLUS || current->type == TOKEN_MINUS) {
        Token *op = current;
        next();
        Node *rhs = term();
        
        if(op->type == TOKEN_PLUS) {
            lhs = make_binop_node(BINOP_ADD, lhs, rhs);
        } else {
            lhs = make_binop_node(BINOP_SUB, lhs, rhs);
        }
    }
    
    if(plusorminus == -1 || plusorminus == 0) {
        return lhs;
    } else {
        return make_negation_node(lhs);
    }
}

Node* condition()
{
    Node *lhs = expression();
    
    if(current->type == TOKEN_EQUALS ||
       current->type == TOKEN_NEQUALS ||
       current->type == TOKEN_LT ||
       current->type == TOKEN_GT ||
       current->type == TOKEN_LTE ||
       current->type == TOKEN_GTE ) {
        Token *op = current;
        next();
        Node *rhs = expression();
        
        if(op->type == TOKEN_EQUALS) {
            return make_condition_node(CONDITION_EQUALS, lhs, rhs);
        } else if(op->type == TOKEN_NEQUALS) {
            return make_condition_node(CONDITION_NEQUALS, lhs, rhs);
        } else if(op->type == TOKEN_LT) {
            return make_condition_node(CONDITION_LT, lhs, rhs);
        } else if(op->type == TOKEN_GT) {
            return make_condition_node(CONDITION_GT, lhs, rhs);
        } else if(op->type == TOKEN_LTE) {
            return make_condition_node(CONDITION_LTE, lhs, rhs);
        } else if(op->type == TOKEN_GTE) {
            return make_condition_node(CONDITION_GTE, lhs, rhs);
        } else {
            printf("!!!!!\nCompiler error!\nparse.c:condition(): Token was allowed as condition operator but was not included in check later!\n!!!!!\n");
            exit(-1);
            return NULL;
        }
        
    } else {
        printf("%d:%d: Error in condition. Invalid operator!\n", current->line, current->x);
        exit(-1);
        return NULL;
    }
}

Node* statement()
{
    Token *start = current;
    if(accept(TOKEN_IDENT)) {
        if(accept(TOKEN_EQUAL)) {
            Node *variable = make_variable_node(start->val);
            Node *expr = expression();
            expect(TOKEN_SEMICOLON);
            
            return make_assignement_node(variable, expr);
        } else if(accept(TOKEN_LEFTPAR)) {
            NodeArray *args = nodearray_new();
            
            do {
                nodearray_add(args, expression());
            } while(accept(TOKEN_COMMA));
            
            expect(TOKEN_RIGHTPAR);
            expect(TOKEN_SEMICOLON);
            
            return make_funccall_node(start->val, args);
            
        } else if(accept(TOKEN_LEFTSQUARE)) {
            
            Node *table_expr = expression();
            expect(TOKEN_RIGHTSQUARE);
            
            if(accept(TOKEN_EQUAL)) {
                Node *expr = expression();
                expect(TOKEN_SEMICOLON);
                
                return make_table_assignment_node(make_variable_node(start->val), table_expr, expr);
            } else {
                printf("%d:%d: Expected '=' after '[]' specifier!\n", current->line, current->x);
                exit(-1);
                return NULL;
            }
            
        } else {
            printf("%d:%d: Expected '=' or '(' after ident!\n", current->line, current->x);
            exit(-1);
            return NULL;
        }
    } else if(accept(TOKEN_IF)) {
        Node *cond = condition();
        
        NodeArray *stmts = nodearray_new();
        
        expect(TOKEN_LEFTCURLY);
        if(accept(TOKEN_RIGHTCURLY)) {
        } else {
            while(current->type != TOKEN_RIGHTCURLY) {
                nodearray_add(stmts, statement());
            }
            expect(TOKEN_RIGHTCURLY);
        }
        
        NodeArray *else_stmts = nodearray_new();
        
        if(accept(TOKEN_ELSE)) {
            if(accept(TOKEN_LEFTCURLY)) {
                
            } else {
                expect(TOKEN_LEFTCURLY);
                while(current->type != TOKEN_RIGHTCURLY) {
                    nodearray_add(else_stmts, statement());
                }
                expect(TOKEN_RIGHTCURLY);
            }
        }
        
        return make_if_node(cond, stmts, else_stmts);
    } else if(accept(TOKEN_WHILE)) {
        
        Node *cond = condition();
        expect(TOKEN_LEFTCURLY);
        
        NodeArray *stmts = nodearray_new();
        if(accept(TOKEN_RIGHTCURLY)) {
            
        } else {
            while(current->type != TOKEN_RIGHTCURLY) {
                nodearray_add(stmts, statement());
            }
            expect(TOKEN_RIGHTCURLY);
        }
        
        return make_while_node(cond, stmts);
        
    } else if(accept(TOKEN_VAR)) {
        
        if(current->type == TOKEN_IDENT) {
            
            Node *variable = make_variable_node(current->val);
            next();
            
            if(accept(TOKEN_EQUAL)) {
                Node *expr = expression();
                expect(TOKEN_SEMICOLON);
                
                return make_decl_assignment_node(variable, expr);
            } else {
                expect(TOKEN_SEMICOLON);
                return make_decl_node(variable);
            }
            
        } else {
            printf("%d:%d: Expected ident after 'var'\n", current->line, current->x);
            exit(-1);
            return NULL;
        }
        
    } else if(accept(TOKEN_RETURN)) {
        
        if(accept(TOKEN_SEMICOLON)) {
            return make_basic_return_node();
        }
        
        Node *expr = expression();
        
        return make_return_node(expr);
        
    } else {
        printf("%d:%d: Expected statement!\n", current->line, current->x);
        exit(-1);
        return NULL;
    }
}

Node* program()
{
    if(accept(TOKEN_FUNC)) {
        Token *ident = current;
        
        if(current->type == TOKEN_IDENT) {
            next();
        } else {
            printf("%d:%d: Expected ident after 'func'!\n", current->line, current->x);
            exit(-1);
            return NULL;
        }
        
        expect(TOKEN_LEFTPAR);
        
        NodeArray *args = nodearray_new();
        if(accept(TOKEN_RIGHTPAR)) {
            
        } else {
            do {
                Node *variable = make_variable_node(current->val);
                next();
                
                nodearray_add(args, variable);
            } while(accept(TOKEN_COMMA));
            
            expect(TOKEN_RIGHTPAR);
        }
        
        expect(TOKEN_LEFTCURLY);
        
        NodeArray *stmts = nodearray_new();
        if(accept(TOKEN_RIGHTCURLY)) {
            
        } else {
            while(current->type != TOKEN_RIGHTCURLY) {
                nodearray_add(stmts, statement());
            }
            expect(TOKEN_RIGHTCURLY);
        }
        
        return make_funcdecl_node(ident->val, args, stmts);
        
    } else if(current->type == TOKEN_VAR) {
        return statement();
    }
    
    return NULL;
}

void parser_init(TokenArray *array)
{
    if(array->len == 0) {
        printf("Failed to init parser. Array is empty!\n");
        exit(-1);
        return;
    }
    
    tokens = array;
    token_offset = 0;
    current = tokens->tokens[token_offset];
}

NodeArray* parser_parse()
{
    NodeArray *nodes = nodearray_new();
    
    Node *node = program();
    while(node != NULL) {
        nodearray_add(nodes, node);
        node = program();
    }
    
    return nodes;
}
