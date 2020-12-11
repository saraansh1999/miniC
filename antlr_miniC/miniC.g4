grammar miniC;

prog: 'Main' block EOF
    ;

stmt: var_decl ';'		#stmt2var_decl
	| var_assgn ';'		#stmt2var_assgn
	| 'if' '(' expr ')' block ('elif' '(' expr ')' block)* ('else' block)?		#stmt2ifelse
	| 'for' '(' mul_var_assgn ';' (expr)? ';' mul_var_assgn ')' block	#stmt2for
	| 'while' '(' (expr)? ')' block		#stmt2while
	| block		#stmt2block
	| 'break' ';'	#stmt2break
	| 'continue' ';'	#stmt2continue
	| 'return' (expr)? ';'	#stmt2return
	| 'input' ('->' var)+ ';'	#stmt2input
	| 'output' ('<-' (expr))+ ';'	#stmt2output
	| func_decl		#stmt2func_decl
	| func_call ';'		#stmt2func_call
	;

func_decl: type ID '(' (type var (',' type var)*)? ')' block
;

func_call: ID '(' (expr (',' expr)*)? ')' 
;

block: '{' (stmt)* '}'
	;

var_decl: type var (',' var)*
;

var_assgn: var assgn_op (var_assgn | expr)		
;

mul_var_assgn: (var_assgn (',' var_assgn)*)?
;

var: ID		#var2ID
	| ID '[' expr ']'	#var2IDexpr
	| ID '[' expr ']' '[' expr ']'	#var2IDexprexpr
	;

expr: var 		#expr2var
	| value		#expr2value
	| func_call 	#expr2func_call
	| un_op expr    #expr2un_op
	| expr a_op1 expr  	#expr2bin_op
	| expr a_op2 expr   #expr2bin_op
	| expr r_op1 expr   #expr2bin_op
	| expr r_op2 expr   #expr2bin_op
	| expr r_op3 expr   #expr2bin_op
	| expr b_op1 expr   #expr2bin_op
	| expr b_op2 expr   #expr2bin_op
	| expr l_op1 expr   #expr2bin_op
	| expr l_op2 expr   #expr2bin_op
	| '(' expr ')'      #expr2paran
	| expr '?' expr ':' expr   #expr2condop
 	;


value: INT 
	|  UINT
	|  CHAR
	|  BOOL
	|  STR 
	;

/*Tokens*/

type: 'int'
	| 'uint'
	| 'char'
	| 'bool'
	;
BOOL: 'true'
	| 'false'
	;
un_op: '!'
	|  '-'
	;
a_op1: '%'
	| '/'
	| '*'
	;
a_op2: '+'
	| '-'
	;
r_op1: '<'
	| '<='
	;
r_op2: '>'
	| '>='
	;
r_op3: '=='
	| '!='
	;
b_op1: '&'
	;
b_op2: '|'
	;
l_op1: '&&'
	;
l_op2: '||'
	;
assgn_op: '=';
ID: [a-zA-Z_][a-zA-Z0-9_]*;
INT: [0-9]+;
UINT: [0-9]+;
CHAR: '\''[\\]?([\u0000-\u0256])?'\'';
STR : '"' (~'"'|'\'')* '"';
COMMENT : '//' ~[\r\n]*->skip; 
NS : [ \t\n] -> skip;
