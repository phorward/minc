/* -HEADER----------------------------------------------------------------------
MinC
Copyright (C) 2008 by Phorward Software Technologies, Jan Max Meyer
http://www.phorward-software.com ++ mail@phorward-software.com

File:	minc.h
Author:	Jan Max Meyer
Usage:	General MinC includes
----------------------------------------------------------------------------- */

#ifndef __MINC_H
#define __MINC_H

/*
 * Includes
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <uchar.h>
#include <dmem.h>
#include <boolean.h>

/*
 * Typedefs
 */
typedef struct _symbol		symbol;
typedef struct _value		value;
typedef struct _mcode		mcode;
typedef struct _geninfo 	geninfo;
typedef struct _scopeinfo	scopeinfo;
typedef enum _instr			instr;

/*
 * Defines
 */
#define DBG( txt )	printf( "minc: DBG %s\n", txt ), fflush( stdout )
#define MALLOC_STEP	128
#define HASH_SIZE	64

#define SYM_VAR		0
#define SYM_VAR_L	1
#define SYM_FUNC	2

#define DT_INT		0
#define DT_FLOAT	1
#define DT_STRING	2

#define GET_DT_NAME( dt ) \
						( ( (dt) == DT_INT ) ? "int" : \
							( (dt) == DT_FLOAT ) ? "float" : "string" )

#define OUTOFMEM( p ) if( !( p ) ) \
						fprintf( stderr, "minc: Ran out of memory.\n" ), \
						exit( EXIT_FAILURE )

#define REQ_NONE	0
#define REQ_VAL		30
#define REQ_VAR		35
#define REQ_ADDR	40

#define	NONE		(char*)NULL

#define ERR_POS		1

#define LABEL_LEN	32

/*
 * Enums
 */
enum _instr
{
	NOP				= REQ_NONE,		/* No operation */
	ADD,							/* Addition */
	CONCAT,							/* Concatenation */
	SUB,							/* Subtraction */
	MUL,							/* Multiplication */
	DIV,							/* Division */
	MOD,							/* Modulus */
	AND,							/* Bitwise AND */
	OR,								/* Bitwise OR */
	SHIFT_LEFT,						/* Bitwise left-shift */
	SHIFT_RIGHT,					/* Bitwise right-shift */
	XOR,							/* Exclusive OR */
	NOT,							/* Not */
	NEG,							/* Negation */
	LOG_AND,						/* Logical AND */
	LOG_OR,							/* Logical OR */
	EQU,							/* Check for equal */
	NOT_EQU,						/* Check for not-equal */
	GREATER,						/* Check for greater */
	LOWER,							/* Check for lower */
	GREATER_EQUAL,					/* Check for greater-equal */
	LOWER_EQUAL,					/* Check for lower-equal */
	TO_STRING,						/* Convert to string */
	TO_FLOAT,						/* Convert to float */
	TO_INT,							/* Convert to int */
	SWAP,							/* Swap first two stack elements */

	CONST			= REQ_VAL,		/* Put constant */

	LOAD			= REQ_VAR,		/* Put variable */
	STORE,							/* Store into variable */

	JUMP_TRUE		= REQ_ADDR,		/* Jump on true condition */
	JUMP_FALSE,						/* Jump on false condition */
	JUMP,							/* Immediate jump */
	GET,							/* Allocate memory on stack */
	UNGET,							/* Disallocate memory on stack */
	LOAD_LOCAL,						/* Put local variable */
	STORE_LOCAL,					/* Store into local variable */
};

/*
 * Structs
 */
struct _value
{
	short	type;
	union
	{
		int		_int;
		float	_float;
		int		_string;
	} value;
};

struct _symbol
{
	uchar*	name;
	short	sym_type;
	short	data_type;

	value*	init_val;

	int		size;
	int		offset;

	symbol*	next;
	symbol*	scope_next;
};

struct _mcode
{
	instr	cmd;

	union
	{
		value	value;
		symbol*	symbol;
		int		address;
	} param;
};

struct _geninfo
{
	short	data_type;
	symbol*	scope_begin;
};

struct _scopeinfo
{
	int		code_begin;
	symbol*	scope_begin;
};

#endif
