/* -MODULE----------------------------------------------------------------------
MinC
Copyright (C) 2008 by Phorward Software Technologies, Jan Max Meyer
http://www.phorward-software.com ++ mail@phorward-software.com

File:	asm.c
Author:	Jan Max Meyer
Usage:	Assembly code generator
----------------------------------------------------------------------------- */

/*
 * Includes
 */
#include "minc.h"
#include "proto.h"

/*
 * Defines
 */
#define GET_TYPE_SPEC( type ) ( ( ( (type) == DT_INT ) ? "dd" : \
								( ( (type) == DT_FLOAT ) ? "dd" : \
								  ( (type) == DT_STRING ) ? "dw" : "" ) ) )

#define GET_TYPE_SPEC_BSS( type ) ( ( ( (type) == DT_INT ) ? "resd" : \
								( ( (type) == DT_FLOAT ) ? "resd" : \
								  ( (type) == DT_STRING ) ? "resw" : "" ) ) )

#define STR_CONST_PREFIX	"STR"

/*
 * Global variables
 */
extern	mcode*		prog;
extern	int			prog_cnt;

extern	uchar**		strtab;
extern	int			strtab_cnt;

extern	symbol*		symtab[];
extern	symbol*		global_scope;

/*
 * Functions
 */

static void gen_dump_string( FILE* stream, uchar* str  )
{
	fprintf( stream, "'%s'", str );
}

static char* gen_label( int addr )
{
	static	int 	next	= 0;
	static	char	name	[ 32 + 1 ];

	if( addr == -1 )
		sprintf( name, "L%d", next++ );
	else
		sprintf( name, "A%d", addr );

	return name;
}

static void gen_variable_declaration( FILE* stream )
{
	int		i, j;
	symbol*	sym;

	/* Build string constants */
	fprintf( stream, "\t\nSECTION .data\n" );

	for( i = 0; i < strtab_cnt; i++ )
	{
		fprintf( stream, "%s%d:\t%s\t",
			STR_CONST_PREFIX, i, GET_TYPE_SPEC( DT_STRING ) );
		gen_dump_string( stream, get_string( i ) );
		fprintf( stream, ",0\n" );
	}

	/* Build variables */
	fprintf( stream, "\t\nSECTION .bss\n" );

	for( sym = global_scope; sym; sym = sym->scope_next )
	{
		fprintf( stream, "%s:\t%s\t1\n",
				sym->name, GET_TYPE_SPEC_BSS( sym->data_type ) );
	}
}

static void gen_load_value( FILE* stream, value* val )
{
	switch( val->type )
	{
		case DT_INT:
			fprintf( stream, "\tmov eax,%d\n", val->value._string );
			break;
		case DT_STRING:
			fprintf( stream, "\tmov eax,%s%i\n", STR_CONST_PREFIX, 
						val->value._string );
			break;
		case DT_FLOAT:
			/*TODO*/
			break;
	}
}

static void gen_global_variable_assign( FILE* stream )
{
	symbol*	sym;

	/* Assign variables */
	for( sym = global_scope; sym; sym = sym->scope_next )
	{
		if( ! sym->init_val )
			continue;

		gen_load_value( stream, sym->init_val );
		fprintf( stream, "\tmov [%s], eax\n", sym->name );
	}
}

static void gen_code( FILE* stream )
{
	int 	i, j;
	char	lbl	[2][LABEL_LEN+1];

	for( i = 0; i < prog_cnt; i++ )
	{
		fprintf( stream, "\n" );
		fprintf( stream, "%s:", gen_label( i ) );
		fprintf( stream, "\t;%s\n", instr_name( prog[i].cmd ) );
		switch( prog[i].cmd )
		{
			case NOP:
				break;
			case ADD:
				fprintf( stream, "\tpop ebx\n" );
				fprintf( stream, "\tpop eax\n" );
				fprintf( stream, "\tadd eax,ebx\n" );
				fprintf( stream, "\tpush eax\n" );
				break;
			case SUB:
				fprintf( stream, "\tpop ebx\n" );
				fprintf( stream, "\tpop eax\n" );
				fprintf( stream, "\tsub eax,ebx\n" );
				fprintf( stream, "\tpush eax\n" );
				break;
			case MUL:
				fprintf( stream, "\tpop ebx\n" );
				fprintf( stream, "\tpop eax\n" );
				fprintf( stream, "\timul ebx\n" );
				fprintf( stream, "\tpush eax\n" );
				break;
			case DIV:
				fprintf( stream, "\tpop ebx\n" );
				fprintf( stream, "\tpop eax\n" );
				fprintf( stream, "\tidiv ebx\n" );
				fprintf( stream, "\tpush eax\n" );
				break;
			case MOD:
				fprintf( stream, "\tpop ebx\n" );
				fprintf( stream, "\tpop eax\n" );
				fprintf( stream, "\tidiv ebx\n" );
				fprintf( stream, "\tpush edx\n" );
				break;
			case AND:
			case LOG_AND:
				fprintf( stream, "\tpop ebx\n" );
				fprintf( stream, "\tpop eax\n" );
				fprintf( stream, "\tand eax,ebx\n" );
				fprintf( stream, "\tpush eax\n" );
				break;
			case OR:
			case LOG_OR:
				fprintf( stream, "\tpop ebx\n" );
				fprintf( stream, "\tpop eax\n" );
				fprintf( stream, "\tor eax,ebx\n" );
				fprintf( stream, "\tpush eax\n" );
				break;
			case XOR:
				fprintf( stream, "\tpop ebx\n" );
				fprintf( stream, "\tpop eax\n" );
				fprintf( stream, "\txor eax,ebx\n" );
				fprintf( stream, "\tpush eax\n" );
				break;
			case SHIFT_LEFT:
				fprintf( stream, "\tpop ebx\n" );
				fprintf( stream, "\tpop eax\n" );
				fprintf( stream, "\tshl eax,ebx\n" );
				fprintf( stream, "\tpush eax\n" );
				break;
			case SHIFT_RIGHT:
				fprintf( stream, "\tpop ebx\n" );
				fprintf( stream, "\tpop eax\n" );
				fprintf( stream, "\tshr eax,ebx\n" );
				fprintf( stream, "\tpush eax\n" );
				break;
			case NOT:
				fprintf( stream, "\tpop eax\n" );
				fprintf( stream, "\tnot eax\n" );
				fprintf( stream, "\tpush eax\n" );
				break;
			case NEG:
				fprintf( stream, "\tpop eax\n" );
				fprintf( stream, "\tneg eax\n" );
				fprintf( stream, "\tpush eax\n" );
				break;
			case EQU:
				fprintf( stream, "\tpop ebx\n" );
				fprintf( stream, "\tpop eax\n" );
				fprintf( stream, "\tsub eax,ebx\n" );
				fprintf( stream, "\tpush eax\n" );
				break;
			case NOT_EQU:
				fprintf( stream, "\tpop ebx\n" );
				fprintf( stream, "\tpop eax\n" );
				fprintf( stream, "\tsub eax,ebx\n" );
				fprintf( stream, "\tpush eax\n" );
				fprintf( stream, "\tnot eax\n" );
				fprintf( stream, "\tpush eax\n" );
				break;
			case GREATER:
			case LOWER:
			case GREATER_EQUAL:
			case LOWER_EQUAL:
				fprintf( stream, "\tpop ebx\n" );
				fprintf( stream, "\tpop eax\n" );

				if( prog[i].cmd == LOWER
						|| prog[i].cmd == LOWER_EQUAL )
					fprintf( stream, "\tcmp ebx,eax\n" );
				else
					fprintf( stream, "\tcmp eax,ebx\n" );

				strcpy( lbl[0], gen_label( -1 ) );

				if( prog[i].cmd == GREATER_EQUAL
						|| prog[i].cmd == LOWER_EQUAL )
					fprintf( stream, "\tjae %s\n", lbl[0] );
				else
					fprintf( stream, "\tja %s\n", lbl[0] );

				fprintf( stream, "\tmov eax, 0\n" );
				strcpy( lbl[1], gen_label( -1 ) );
				fprintf( stream, "\tjmp %s\n", lbl[1] );
				fprintf( stream, "%s:\tmov eax, 1\n", lbl[0] );
				fprintf( stream, "%s:\tpush eax\n", lbl[1] );
				break;
			case TO_STRING:
				break;
			case TO_FLOAT:
				break;
			case TO_INT:
				break;
			case SWAP:
				fprintf( stream, "\tpop ebx\n" );
				fprintf( stream, "\tpop eax\n" );
				fprintf( stream, "\tpush ebx\n" );
				fprintf( stream, "\tpush eax\n" );
				break;

			case CONST:
				gen_load_value( stream, &( prog[i].param.value ) );
				fprintf( stream, "\tpush eax\n" );
				break;

			case LOAD:
				fprintf( stream, "\tmov eax,[%s]\n", prog[i].param.address );
				fprintf( stream, "\tpush eax\n" );
				break;
			case STORE:
				break;

			case JUMP_TRUE:
			case JUMP_FALSE:
				fprintf( stream, "\tpop eax\n" );
				fprintf( stream, "\tcmp eax, 0\n" );

				if( prog[i].cmd == JUMP_FALSE )
					fprintf( stream, "\tje %s\n",
							gen_label( prog[i].param.address ) );
				else
					fprintf( stream, "\tjne %s\n",
							gen_label( prog[i].param.address ) );
				break;

			case JUMP:
				fprintf( stream, "\tjmp %s\n",
						gen_label( prog[i].param.address ) );
				break;
			case GET:
				fprintf( stream, "\tsub esp,%d\n", prog[i].param.address );
				/*
				fprintf( stream, "\tmov eax, 0\n" );
				for( j = 0; j < prog[i].param.address; j+=4 )
					fprintf( stream, "\tpush eax\n" );
				*/
				break;
			case UNGET:
				fprintf( stream, "\tadd esp,%d\n", prog[i].param.address );
				break;
			case LOAD_LOCAL:
				fprintf( stream, "\tmov eax,[ebp+%d]\n",
					prog[i].param.address );
				fprintf( stream, "\tpush eax\n" );
				break;
			case STORE_LOCAL:
				fprintf( stream, "\tpop eax\n" );
				fprintf( stream, "\tmov [ebp+%d],eax\n",
					prog[i].param.address );
				break;

			default:
				break;
		}
	}
}

void gen_x86( FILE* stream )
{
	gen_variable_declaration( stream );

	fprintf( stream, "\t\nSECTION .text\n" );
	fprintf( stream, "\tglobal _start\n" );
	fprintf( stream, "_start:\n" );

	/* Skipping the argc and *argv ... */
	fprintf( stream, "\tpush ebp\n" );
	fprintf( stream, "\tmov ebp,esp\n\n" );

	gen_global_variable_assign( stream );
	gen_code( stream );

	fprintf( stream, "\n\tmov esp,ebp\n" );
	fprintf( stream, "\tpop ebp\n" );
}
