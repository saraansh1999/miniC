grammar Expr;

prog: (e1 ';') + EOF
    ;

e1:	e1 '==' e2
	| e1 '!=' e2
	| e2
	;

e2:	e2 '>' e3
	| e2 '>=' e3
	| e3
	;

e3:	e3 '<' e4
	| e3 '<=' e4
	| e4
	;

e4: e4 '+' e5
    | e4 '-' e5
    | e5
    ;
    
e5: e5 '*' e6
	| e5 '/' e6
	| e5 '%' e6
	| e6    
    ;


e6: '-'?'(' e1 ')'
	| '-'?ID
	| literal
	;

literal: INT
	| FLOAT
	;

/*Tokens*/

ID  : [a-z][a-zA-Z0-9_]*;
INT : [-]?[0-9]+;
FLOAT: [-]?[0-9]+'.'[0-9]*;
COMMENT : '//' ~[\r\n]*->skip; 
NS : [ \t\n]+ -> skip; 


