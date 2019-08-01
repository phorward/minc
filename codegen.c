/* -MODULE----------------------------------------------------------------------
MinC
Copyright (C) 2008 by Phorward Software Technologies, Jan Max Meyer
http://www.phorward-software.com ++ mail@phorward-software.com

File:	codegen.c
Author:	Jan Max Meyer
Usage:	Code generation functions
----------------------------------------------------------------------------- */

/*
 * Includes
 */
#include "minc.h"
#include "proto.h"

/*
 * Defines
 */

/*
 * Global variables
 */
mcode*			prog		= (mcode*)NULL;
int				prog_cnt	= 0;

static BOOLEAN	use_last	= FALSE;

/*
 * Functions
 */

uchar* instr_name( instr i )
{
	static uchar*	ins	;

	switch( i )
	{
		case NOP:
			ins = "NOP";
			break;
		case ADD:
			ins = "ADD";
			break;
		case CONCAT:
			ins = "CONCAT";
			break;
		case SUB:
			ins = "SUB";
			break;
		case MUL:
			ins = "MUL";
			break;
		case DIV:
			ins = "DIV";
			break;
		case MOD:
			ins = "MOD";
			break;
		case AND:
			ins = "AND";
			break;
		case OR:
			ins = "OR";
			break;
		case XOR:
			ins = "XOR";
			break;
		case SHIFT_LEFT:
			ins = "SHIFT_LEFT";
			break;
		case SHIFT_RIGHT:
			ins = "SHIFT_RIGHT";
			break;
		case NOT:
			ins = "NOT";
			break;
		case NEG:
			ins = "NEG";
			break;
		case LOG_AND:
			ins = "LOG_AND";
			break;
		case LOG_OR:
			ins = "LOG_OR";
			break;
		case EQU:
			ins = "EQU";
			break;
		case NOT_EQU:
			ins = "NOT_EQU";
			break;
		case GREATER:
			ins = "GREATER";
			break;
		case LOWER:
			ins = "LOWER";
			break;
		case GREATER_EQUAL:
			ins = "GREATER_EQUAL";
			break;
		case LOWER_EQUAL:
			ins = "LOWER_EQUAL";
			break;
		case TO_STRING:
			ins = "TO_STRING";
			break;
		case TO_FLOAT:
			ins = "TO_FLOAT";
			break;
		case TO_INT:
			ins = "TO_INT";
			break;
		case SWAP:
			ins = "SWAP";
			break;

		case CONST:
			ins = "CONST";
			break;

		case LOAD:
			ins = "LOAD";
			break;
		case STORE:
			ins = "STORE";
			break;

		case JUMP_TRUE:
			ins = "JUMP_TRUE";
			break;
		case JUMP_FALSE:
			ins = "JUMP_FALSE";
			break;
		case JUMP:
			ins = "JUMP";
			break;
		case GET:
			ins = "GET";
			break;
		case UNGET:
			ins = "UNGET";
			break;
		case LOAD_LOCAL:
			ins = "LOAD_LOCAL";
			break;
		case STORE_LOCAL:
			ins = "STORE_LOCAL";
			break;

		default:
			ins = "[undefined]";	
			break;
	}

	return ins;
}

void print_prog( void )
{
	int i;

	for( i = 0; i < prog_cnt; i++ )
	{
		fprintf( stderr, "%04d: %s ", i, instr_name( prog[i].cmd ) ); 

		if( prog[i].cmd >= REQ_ADDR )
			fprintf( stderr, "%04d", prog[i].param.address );
		else if( prog[i].cmd >= REQ_VAR )
			fprintf( stderr, "%s [%s]", prog[i].param.symbol->name,
				GET_DT_NAME( prog[i].param.symbol->data_type ) );
		else if( prog[i].cmd >= REQ_VAL )
		{
			fprintf( stderr, "%s", get_value( &( prog[i].param.value ) ) );
			fprintf( stderr, " [%s]", GET_DT_NAME( prog[i].param.value.type ) );
		}

		fprintf( stderr, "\n" );
	}
}

static int new_address( void )
{
	if( use_last )
	{
		prog_cnt--;
		use_last = FALSE;
	}
	else if( !( prog_cnt % MALLOC_STEP ) )
	{
		if( !prog )
			prog = (mcode*)MALLOC( MALLOC_STEP * sizeof( mcode ) );
		else
			prog = (mcode*)REALLOC( (mcode*)prog,
						( ( prog_cnt / MALLOC_STEP + 1 ) * MALLOC_STEP )
							* sizeof( mcode ) );
		OUTOFMEM( prog );
	}

	memset( &( prog[ prog_cnt ] ), 0, sizeof( mcode ) );
	
	return prog_cnt++;
}

int current_address( void )
{
	if( prog_cnt == 0 )
		return 0;

	return prog_cnt - 1;
}

int next_address( void )
{
	int 	addr = new_address();

	use_last = TRUE;

	return addr;
}

mcode* get_address( int idx )
{
	if( idx < 0 && idx >= prog_cnt )
		return (mcode*)NULL;

	return &( prog[idx] );
}

int gen( instr cmd, char* param )
{
	int		off;
	mcode*	ret;

	off = new_address();
	ret = &( prog[ off ] );
	ret->cmd = cmd;

	/* Evaluate parameter for different types */
	if( cmd >= REQ_ADDR )
		ret->param.address = (int)param;
	else if( cmd >= REQ_VAR )
		ret->param.symbol = (symbol*)param;
	else if( cmd >= REQ_VAL )
		memcpy( &( ret->param.value ), (value*)param, sizeof( value ) );

	return off;
}

short promote_gen( instr cmd, char* param,
	short first, short second )
{
	short	promote		= first;
	instr	convcmd		= NOP;

	/*
	 * Promotion table:
	 *
	 * int int => int
	 * float float => float
	 * string string => string
	 * int float => float
	 * int string => string
	 * float string => string
	 */
	if( cmd != STORE && cmd != STORE_LOCAL )
	{
		if( promote == DT_STRING || second == DT_STRING )
			promote = DT_STRING;
		else if( promote == DT_FLOAT || second == DT_FLOAT )
			promote = DT_FLOAT;
	}
	else
	{
		if( promote != DT_STRING && second == DT_STRING )
		{
			error( ERR_POS, "Can't store string to %s variable",
					GET_DT_NAME( promote ) );
			return;
		}
	}

	if( promote == DT_STRING )
	{
		switch( cmd )
		{
			case ADD:
				cmd = CONCAT;
			case EQU:
			case NOT_EQU:
			case STORE:
			case STORE_LOCAL:
				break;

			default:
				error( ERR_POS, "Can't use operator on string" );
				return;
		}

		convcmd = TO_STRING;
	}
	else if( promote == DT_FLOAT )
		convcmd = TO_FLOAT;
	else if( promote == DT_INT )
		convcmd = TO_INT;

	if( ( second != promote ) )
		gen( convcmd, NONE );
	else if( ( first != promote ) )
	{
		gen( SWAP, NONE );
		gen( convcmd, NONE );
		gen( SWAP, NONE );
	}

	gen( cmd, param );

	return promote;
}

