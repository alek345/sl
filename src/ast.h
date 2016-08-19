#ifndef AST_H
#define AST_H

typedef enum {
    NODE_BINOP = 0,
    NODE_CONSTANT,
    NODE_VARIABLE,
    NODE_CONDITION,
    NODE_NEGATION,  // Rename to unary
    NODE_ASSIGNMENT,
    NODE_IF,
    NODE_DECLASSIGNMENT,
    NODE_DECL,
    NODE_FUNCDECL,
    NODE_FUNCCALL,
    NODE_TABLE_READ,
    NODE_RETURN,
    NODE_BASIC_RETURN,
    NODE_TABLEASSIGNMENT,
    NODE_WHILE,
} NodeType;

typedef enum {
    BINOP_ADD = 0,
    BINOP_SUB,
    BINOP_MUL,
    BINOP_DIV,
    /* Add bitwise or, and, etc. */
} BinOpType;

typedef enum {
    CONSTANT_NUMBER = 0,
    CONSTANT_STRING,
    CONSTANT_TABLE, // This is a empty table with no value
} ConstantType;

typedef enum {
    CONDITION_EQUALS = 0,
    CONDITION_NEQUALS,
    CONDITION_LT,
    CONDITION_GT,
    CONDITION_LTE,
    CONDITION_GTE,
} ConditionType;

typedef struct NodeArray NodeArray;
typedef struct Node Node;
struct Node {
    NodeType type;
    
    union {
        struct {
            BinOpType type;
            Node *lhs;
            Node *rhs;
        } binop;
        
        struct {
            ConstantType type;
            char *val;
        } constant;
        
        struct {
            char *name;
        } variable;
        
        struct {
            ConditionType type;
            Node *lhs;
            Node *rhs;
        } condition;
        
        struct {
            Node *expr;
        } negatation;
        
        struct {
            Node *variable;
            Node *expr;
        } assignment;
        
        struct {
            Node *condition;
            NodeArray *stmts;
            NodeArray *else_stmts;
        } if_node;
        
        struct {
            Node *name;
        } decl;
        
        struct {
            Node *name;
            Node *expr;
        } decl_assignment;
        
        struct {
            char *name;
            NodeArray *args;
            NodeArray *stmts;
        } func_decl;
        
        struct {
            char *name;
            NodeArray *args;
        } func_call;
        
        struct {
            Node *variable; // Can be another table read
            Node *expr;
        } table_read;
        
        struct {
            
        } basic_return;
        
        struct {
            Node *expr;
        } return_node;
        
        struct {
            Node *variable;
            Node *table_expr;
            Node *expr;
        } table_assignment;
        
        struct {
            Node *condition;
            NodeArray *stmts;
        } while_node;
    };
};

struct NodeArray {
    Node **nodes;
    int len;
};

NodeArray* nodearray_new();
void nodearray_add();

Node* make_variable_node(char *name);
Node* make_constant_node(ConstantType type, char *val);
Node* make_binop_node(BinOpType type, Node *lhs, Node *rhs);
Node* make_condition_node(ConditionType type, Node *lhs, Node *rhs);
Node* make_negation_node(Node *expr);
Node* make_assignement_node(Node *variable, Node *expr);
Node* make_if_node(Node *condition, NodeArray *stmts, NodeArray *else_stmts);
Node* make_decl_node(Node *var);
Node* make_decl_assignment_node(Node *var, Node *expr);
Node* make_funcdecl_node(char *name, NodeArray *args, NodeArray *stmts);
Node* make_funccall_node(char *name, NodeArray *args);
Node* make_table_read_node(Node *variable, Node *expr);
Node* make_basic_return_node();
Node* make_return_node(Node *expr);
Node* make_table_assignment_node(Node *variable, Node *table_expr, Node *expr);
Node* make_while_node(Node *condition, NodeArray *stmts);

#endif /* AST_H */