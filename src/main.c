#include <stdio.h>
#include <stdlib.h>
#include "lex.h"
#include "parse.h"

char* read_file(char *path)
{
    FILE *f = fopen(path, "rb");
    
    fseek(f, 0, SEEK_END);
    int len = ftell(f);
    rewind(f);
    
    char *buffer = malloc(sizeof(char)*(len+1));
    buffer[len] = 0;
    
    fread(buffer, sizeof(char), len, f);
    fclose(f);
    
    return buffer;
}

static int indent = 0;

void print_indents()
{
    int i = indent;
    while(i > 0) {
        i--;
        printf("  ");
    }
}

void print_node(Node *n)
{
    switch(n->type) {
        case NODE_BINOP: {
            print_indents();
            printf("BinOp: (%d)\n", n->binop.type);
            print_indents();
            printf("lhs:\n");
            indent++;
            print_node(n->binop.lhs);
            indent--;
            print_indents();
            printf("rhs:\n");
            indent++;
            print_node(n->binop.rhs);
            indent--;
        } break;
        
        case NODE_CONSTANT: {
            print_indents();
            printf("Constant: (%d)\n", n->constant.type);
            indent++;
            print_indents();
            printf("Val: %s\n", n->constant.val);
            indent--;
        } break;
        
        case NODE_VARIABLE: {
            print_indents();
            printf("Variable: %s\n", n->variable.name);
        } break;
        
        case NODE_CONDITION: {
            print_indents();
            printf("Condition: (%d)\n", n->condition.type);
            print_indents();
            printf("Lhs:\n");
            indent++;
            print_node(n->condition.lhs);
            indent--;
            print_indents();
            printf("Rhs:\n");
            indent++;
            print_node(n->condition.rhs);
            indent--;
        } break;
        
        case NODE_NEGATION: {
            print_indents();
            printf("Negation:\n");
            print_indents();
            printf("Expr:\n");
            indent++;
            print_node(n->negatation.expr);
            indent--;
        } break;
        
        case NODE_ASSIGNMENT: {
            print_indents();
            printf("Assignment:\n");
            print_indents();
            printf("Var:\n");
            indent++;
            print_node(n->assignment.variable);
            indent--;
            print_indents();
            printf("Expr:\n");
            indent++;
            print_node(n->assignment.expr);
            indent--;
        } break;
        
        case NODE_IF: {
            print_indents();
            printf("If:\n");
            print_indents();
            printf("Condition:\n");
            indent++;
            print_node(n->if_node.condition);
            indent--;
            print_indents();
            printf("True stmts:\n");
            indent++;
            int i;
            for(i = 0; i < n->if_node.stmts->len; i++) {
                print_node(n->if_node.stmts->nodes[i]);
            }
            indent--;
            print_indents();
            printf("False stmts:\n");
            indent++;
            for(i = 0; i < n->if_node.else_stmts->len; i++) {
                print_node(n->if_node.else_stmts->nodes[i]);
            }
            indent--;
        } break;
        
        case NODE_DECL: {
            print_indents();
            printf("Decl:\n");
            indent++;
            print_node(n->decl.name);
            indent--;
        } break;
        
        case NODE_DECLASSIGNMENT: {
            print_indents();
            printf("Decl assignement:\n");
            print_indents();
            printf("Name:\n");
            indent++;
            print_node(n->decl_assignment.name);
            indent--;
            print_indents();
            printf("Expr:\n");
            indent++;
            print_node(n->decl_assignment.expr);
            indent--;
        } break;
        
        case NODE_FUNCDECL: {
            print_indents();
            printf("Func: %s\n", n->func_decl.name);
            print_indents();
            printf("Args:\n");
            indent++;
            int i;
            for(i = 0; i < n->func_decl.args->len; i++) {
                print_node(n->func_decl.args->nodes[i]);
            }
            indent--;
            print_indents();
            printf("Stmts:\n");
            indent++;
            for(i = 0; i < n->func_decl.stmts->len; i++) {
                print_node(n->func_decl.stmts->nodes[i]);
            }
            indent--;
        } break;
        
        case NODE_FUNCCALL: {
            print_indents();
            printf("Func call: %s\n", n->func_call.name);
            print_indents();
            printf("Args:\n");
            indent++;
            int i;
            for(i = 0; i < n->func_decl.args->len; i++) {
                print_node(n->func_decl.args->nodes[i]);
            }
            indent--;
        } break;
        
        case NODE_TABLE_READ: {
            print_indents();
            printf("Table read:\n");
            print_indents();
            printf("Variable\n");
            indent++;
            print_node(n->table_read.variable);
            indent--;
            print_indents();
            printf("Expr:\n");
            indent++;
            print_node(n->table_read.expr);
            indent--;
        } break;
        
        case NODE_RETURN: {
            print_indents();
            printf("Return\n");
            print_indents();
            printf("Expr:\n");
            indent++;
            print_node(n->return_node.expr);
            indent--;
        } break;
        
        case NODE_BASIC_RETURN: {
            print_indents();
            printf("Basic return\n");
        } break;
        
        case NODE_TABLEASSIGNMENT: {
            print_indents();
            printf("Table assignment:\n");
            print_indents();
            printf("Variable\n");
            indent++;
            print_node(n->table_assignment.variable);
            indent--;
            print_indents();
            printf("Table expr\n");
            indent++;
            print_node(n->table_assignment.table_expr);
            indent--;
            print_indents();
            printf("Expr\n");
            indent++;
            print_node(n->table_assignment.expr);
            indent--;
        } break;
        
        case NODE_WHILE: {
            print_indents();
            printf("While:\n");
            print_indents();
            printf("Condition:\n");
            indent++;
            print_node(n->while_node.condition);
            indent--;
            print_indents("Stmts:\n");
            indent++;
            int i;
            for(i = 0; i < n->while_node.stmts->len; i++) {
                print_node(n->while_node.stmts->nodes[i]);
            }
            indent--;
        } break;
    }
}

int main(int argc, char **argv)
{
    char *src = read_file("lex-test.sl");
    
    TokenArray *list = lex(src);
    
    int i;
    for(i = 0; i < list->len; i++) {
        Token *t = list->tokens[i];
        printf("%s", token_to_string(t->type));
        
        if(t->type == TOKEN_IDENT) {
            printf(": %s\n", t->val);
        } else if(t->type == TOKEN_NUMBER) {
            printf(": %s\n", t->val);
        } else if(t->type == TOKEN_STRING) {
            printf(": \"%s\"\n", t->val);
        } else {
            printf("\n");
        }
    }
    
    parser_init(list);
    NodeArray *nodes = parser_parse();
    
    for(i = 0; i < nodes->len; i++) {
        print_node(nodes->nodes[i]);
    }
    
    return 0;
}
