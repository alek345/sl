/*
var a = "constant_string";
var b = 32;
var c = a + b;

## Instruction Set ##
   It's a start!

PUSHT - Pushes a empty table to the stack
PUSHC n - Pushes constant n to the stack
LOAD n - Pushes local n, if n is positive. Pushed arg n+2 if n i negative
STORE n - Same as above expect pop the stack into memory


#### Data ####

CONSTANT_TABLE:
0 - Type: string, Val: "constant_string"
1 - Type: integer, Val: 32

CODE:

PUSHC 0 ; Push constant 0 to the stack
STORE 1 ; Store the value at the top of the stack in local variable 1
PUSHC 1 ; Push constant 1 to the stack
STORE 2 ; Store the value at the top of the stack in local variable 2
LOAD 1  ; Push local variable 1 to the stack
LOAD 2  ; Push local variable 2 to the stack
STR_CAT ; Concatonate the two values at the top of the stack
STORE 3 ; Store the value at the top of the stack in local variable 3
*/

/*
BINARY:
*/

typedef struct {
	u8 type;
	u8 *data;
} SLConstant;

typedef struct {
	char header[2];
	u8 version;
	u16 num_constants;
	SLConstant *constants;
	u32 code_length;
	u8 *code;
} SLBinary;

/*
## The stack ##
*/

typedef enum {
	VALUE_NIL = 0,
	VALUE_INTEGER,
	VALUE_STRING,
	VALUE_FLOAT,
} ValueType;

#include <stdint.h>
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;

typedef struct {
	ValueType type;
	union {
		struct {
			u32 value;
		} integer_type;
		
		struct {
			char *string;
			int len;
			// precomputed_hash;
		} string_type;
		
		struct {
			float value;
		} float_type;
	};
} SLValue;

typedef struct {
	int total_size;
	int increment_size;
	int stack_pointer;
	SLValue *values;
} Stack;

Stack* stack_new(int init_size, int increment_size)
{
	Stack *s = malloc(sizeof(Stack));
	
	s->increment_size = increment_size;
	s->total_size = init_size;
	s->stack_pointer = 0;
	s->values = malloc(sizeof(SLValue)*s->total_size);

	return s;
}

void stack_push(Stack *s, SLValue *v)
{
	if(s->stack_pointer+1 == s->total_size-1) {
		s->total_size++;
		s->values = realloc(s->values, sizeof(SLValue)*s->total_size);
	}
	s->stack_pointer++;
	s->values[s->stack_pointer] = v;
}

SLValue* stack_pop(Stack *s)
{
    if(s->stack_pointer == 0) {
        return NULL;
    }
    SLValue *result = s->values[s->stack_pointer];
    s->stack_pointer--;
    return result;
}

void stack_free(Stack *s)
{
    free(s->values);
    free(s);
}

/*
## More examples ##

var a = 3 + 2 * 5;

0 - Type: integer, Val: 3
1 - Type: integer, Val: 2
2 - Type: integer, Val: 5

Opcodes:                                                      Stack:

PUSHC 2 ; Push constant 2 onto stack                          [ 5 ]
PUSHC 1 ; Push constant 1 onto stack                          [ 5, 2 ]
IMUL    ; Multiply the stack and push the result on the stack [ 10 ]
PUSHC 0 ; Push constant 0 onto stack                          [ 10, 3 ]
IADD    ; Add the stack and push the result on the stack      [ 13 ]
STORE 1 ; Store at local variable 1                           [   ]
/*

/* 
## Another example ##

func add(a, b) {
    return a + b;
}

func main() {
    var c = add(4, 5);
}

Constant table:

0 - Type: integer, Val: 4
1 - Type: integer, Val: 5

Function table:

add - Address: 0, Args: 2, Locals: 0
main - Address: 2, Args: 0, Locals: 1

Opcodes:

;func add(a, b)
IMUL
RET

;func main()
PUSHC 1 ; Push in reverse order
PUSHC 0
CALL 0 ; Call function at address 0, different from a jump, sets up call stack
STORE 1 ; Store result in local 1
RET ; Always return from a function

*/