#include "ast.h"
#include <stdlib.h>
#include <string.h>

NodeArray* nodearray_new()
{
    NodeArray *array = malloc(sizeof(NodeArray));
    
    array->nodes = 0;
    array->len = 0;
    
    return array;
}

void nodearray_add(NodeArray *array, Node *node)
{
    array->len++;
    array->nodes = realloc(array->nodes, sizeof(Node)*array->len);
    array->nodes[array->len-1] = node;
}

Node* make_variable_node(char *name)
{
    Node *n = malloc(sizeof(Node));
    
    n->type = NODE_VARIABLE;
    n->variable.name = strdup(name);
    n->variable.is_table = 0;
    
    return n;
}


Node* make_variable_table_node(char *name)
{
    Node *n = malloc(sizeof(Node));
    
    n->type = NODE_VARIABLE;
    n->variable.name = strdup(name);
    n->variable.is_table = 1;
    
    return n;
}

Node* make_constant_node(ConstantType type, char *val)
{
    Node *n = malloc(sizeof(Node));
    
    n->type = NODE_CONSTANT;
    n->constant.type = type;
    n->constant.val = strdup(val);
    
    return n;
}

Node* make_binop_node(BinOpType type, Node *lhs, Node *rhs)
{
    Node *n = malloc(sizeof(Node));
    
    n->type = NODE_BINOP;
    n->binop.type = type;
    n->binop.lhs = lhs;
    n->binop.rhs = rhs;
    
    return n;
}

Node* make_condition_node(ConditionType type, Node *lhs, Node *rhs)
{
    Node *n = malloc(sizeof(Node));
    
    n->type = NODE_CONDITION;
    n->condition.type = type;
    n->condition.lhs = lhs;
    n->condition.rhs = rhs;
    
    return n;
}

Node* make_negation_node(Node *expr)
{
    Node *n = malloc(sizeof(Node));
    
    n->type = NODE_NEGATION;
    n->negatation.expr = expr;
    
    return n;
}

Node* make_assignement_node(Node *variable, Node *expr)
{
    Node *n = malloc(sizeof(Node));
    
    n->type = NODE_ASSIGNMENT;
    n->assignment.variable = variable;
    n->assignment.expr = expr;
    
    return n;
}

Node* make_if_node(Node *condition, NodeArray *stmts, NodeArray *else_stmts)
{
    Node *n = malloc(sizeof(Node));
    
    n->type = NODE_IF;
    n->if_node.condition = condition;
    n->if_node.stmts = stmts;
    n->if_node.else_stmts = else_stmts;
    
    return n;
}

Node* make_decl_node(Node *name)
{
    Node *n = malloc(sizeof(Node));
    
    n->type = NODE_DECL;
    n->decl.name = name;
    
    return n;
}

Node* make_decl_assignment_node(Node *name, Node *expr)
{
    Node *n = malloc(sizeof(Node));
    
    n->type = NODE_DECLASSIGNMENT;
    n->decl_assignment.name = name;
    n->decl_assignment.expr = expr;
    
    return n;
}

Node* make_funcdecl_node(char *name, NodeArray *args, NodeArray *stmts)
{
    Node *n = malloc(sizeof(Node));
    
    n->type = NODE_FUNCDECL;
    n->func_decl.name = strdup(name);
    n->func_decl.args = args;
    n->func_decl.stmts = stmts;
    
    return n;
}

Node* make_funccall_node(char *name, NodeArray *args)
{
    Node *n = malloc(sizeof(Node));
    
    n->type = NODE_FUNCCALL;
    n->func_call.name = strdup(name);
    n->func_call.args = args;
    
    return n;
}

Node* make_table_read_node(Node *variable, Node *expr)
{
    Node *n = malloc(sizeof(Node));
    
    n->type = NODE_TABLE_READ;
    n->table_read.variable = variable;
    n->table_read.expr = expr;
    
    return n;
}

Node* make_basic_return_node()
{
    Node *n = malloc(sizeof(Node));
    
    n->type = NODE_BASIC_RETURN;
    
    return n;
}

Node* make_return_node(Node *expr)
{
    Node *n = malloc(sizeof(Node));
    
    n->type = NODE_RETURN;
    n->return_node.expr = expr;
    
    return n;
}