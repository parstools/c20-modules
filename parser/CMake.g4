/*
Copyright (c) 2018  zbq.

License for use and distribution: Eclipse Public License

CMake language grammar reference:
https://cmake.org/cmake/help/v3.12/manual/cmake-language.7.html

*/

grammar CMake;

options { caseInsensitive = true; }

file_
	: command+ EOF
	;


command
    : block_command
    | command_invocation
    ;

block_command
    : BLOCK compound_argument command* 'ENDBLOCK' empty_argument
    ;

if_command
    : 'IF' condition command* ('ELSEIF' condition command*)*
            ('ELSE' empty_argument command*)?
            'ENDIF' empty_argument
    ;

condition
    : LPARENT unary_condition (('AND'|'OR') unary_condition)* RPARENT
    ;

unary_condition
    : binary_test
    | 'NOT' unary_condition
    ;

binary_test
    : unary_test (binary_test_operator unary_test)*
    ;

binary_test_operator
    : 'EQUAL'
    | 'LESS'
    | 'LESS_EQUAL'
    | 'GREATER'
    | 'GREATER_EQUAL'
    | 'STREQUAL'
    | 'STRLESS'
    | 'STRLESS_EQUAL'
    | 'STRGREATER'
    | 'STRGREATER_EQUAL'
    | 'VERSION_EQUAL'
    | 'VERSION_LESS'
    | 'VERSION_LESS_EQUAL'
    | 'VERSION_GREATER'
    | 'VERSION_GREATER_EQUAL'
    | 'PATH_EQUAL'
    | 'MATCHES'
    ;

unary_test
    : basic_expression
    | unary_test_operator unary_test
    | condition
    ;

unary_test_operator
    :  'EXISTS'
    |  'COMMAND'
    |  'DEFINED'
    ;

basic_expression
    : constant|variable|Quoted_argument|Unquoted_argument
    ;

Unquoted_argument
	: (~[ \t\r\n()#"\\] | Escape_sequence)+
	;

constant
    : Number
    ;

variable
    : Identifier
    ;

empty_argument
    : LPARENT RPARENT
    ;

command_invocation
	: Identifier compound_argument
	;

single_argument
	: Identifier | Bracket_argument | Quoted_argument
	;

compound_argument
	: LPARENT (single_argument|compound_argument)* RPARENT
	;

get_value
    : '$' '{' variable '}'
    ;

Number
    : [0-9.]+
	;

Escape_sequence
	: Escape_Identifierity | Escape_encoded | Escape_semicolon
	;


BLOCK
    : 'BLOCK'
    ;

Identifier
	: [A-Z_][A-Z0-9_]*
	;

fragment
Escape_Identifierity
	: '\\' ~[A-Z0-9;]
	;

fragment
Escape_encoded
	: '\\t' | '\\r' | '\\n'
	;

fragment
Escape_semicolon
	: '\\;'
	;

Quoted_argument
	: '"' (~[\\"] | Escape_sequence | Quoted_cont)* '"'
	;

fragment
Quoted_cont
	: '\\' ('\r' '\n'? | '\n')
	;

Bracket_argument
	: '[' Bracket_arg_nested ']'
	;

fragment
Bracket_arg_nested
	: '=' Bracket_arg_nested '='
	| '[' .*? ']'
	;

Bracket_comment
	: '#[' Bracket_arg_nested ']'
	-> skip
	;

Line_comment
	: '#' (  // #
	  	  | '[' '='*   // #[==
		  | '[' '='* ~('=' | '[' | '\r' | '\n') ~('\r' | '\n')*  // #[==xx
		  | ~('[' | '\r' | '\n') ~('\r' | '\n')*  // #xx
		  ) ('\r' '\n'? | '\n' | EOF)
    -> skip
	;

Newline
	: ('\r' '\n'? | '\n')+
	-> skip
	;

Space
	: [ \t]+
	-> skip
	;

LPARENT : '(';
RPARENT : ')';
