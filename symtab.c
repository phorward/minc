/* -MODULE----------------------------------------------------------------------
MinC
Copyright (C) 2008 by Phorward Software Technologies, Jan Max Meyer
http://www.phorward-software.com ++ mail@phorward-software.com

File:	symtab.c
Author:	Jan Max Meyer
Usage:	Symbol table management
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
symbol*	symtab		[ HASH_SIZE ];

/*
 * Functions
 */

/* Hash index computation */
static int hash_symbol( uchar* name )
{
	int		v = 0;
	uchar*	p;
	for( p = name; *name; name++ )
		v += (int)*name;

	return v % HASH_SIZE;
}

/* Allocate new symbol */
symbol* new_symbol( uchar* name, short sym_type )
{
	symbol*		sym;
	
	if( !( sym = (symbol*)MALLOC( sizeof( symbol ) ) ) )
		return (symbol*)NULL;

	memset( sym, 0, sizeof( symbol ) );
	sym->name = name;
	sym->sym_type = sym_type;

	return sym;
}

/* Free symbol memory */
symbol* free_symbol( symbol* sym )
{
	FREE( sym->name );
	FREE( sym );

	return (symbol*)NULL;
}

/* Get a symbol pointer from the symbol table */
symbol* find_symbol( uchar* name )
{
	symbol*		link;

	if( !( link = symtab[ hash_symbol( name ) ] ) )
		return (symbol*)NULL;

	for( ; link; link = link->next )
	{
		if( !strcmp( link->name, name ) )
			return link;
	}

	return (symbol*)NULL;
}

/* Remove a symbol pointer from the symbol table */
BOOLEAN remove_symbol( symbol* sym )
{
	symbol*		prev	= (symbol*)NULL;
	symbol*		link;
	int			idx;

	idx = hash_symbol( sym->name );

	if( !( link = symtab[ idx ] ) )
		return FALSE;

	for( ; link; link = link->next, prev = link )
	{
		if( link == sym )
		{
			if( !prev )
				symtab[ idx ] = link->next;
			else
				prev->next = link->next;

			return TRUE;
		}
	}

	return FALSE;
}

/* Insert symbol into symbol table */
void add_symbol( symbol* sym )
{
	symbol*		link;
	symbol*		last;
	int			idx;

	idx = hash_symbol( sym->name );

	/* If no symbol table exists, yet... */
	if( !( link = symtab[ idx ] ) )
	{
		symtab[ idx ] = sym;
		return;
	}

	/* Insert symbol with same name before existing item, if available! */
	for( last = (symbol*)NULL; link; link = link->next )
	{
		if( !strcmp( link->name, sym->name ) )
		{
			sym->next = link;

			if( last )
				last->next = sym;
			else
				symtab[ idx ] = sym;

			return;
		}

		last = link;
	}

	/* Ok, then add this to the last one! */
	last->next = sym;
}

/* Append symbol to scope-chain */
symbol* scope_append( symbol* begin, symbol* engage )
{
	symbol*		sym;

	if( !begin )
		begin = engage;
	else
	{
		for( sym = begin; sym->scope_next; sym = sym->scope_next )
			;

		sym->scope_next = engage;
	}

	return begin;
}

void free_scope( symbol* start )
{
	symbol*	del;

	while( ( del = start ) )
	{
		start = start->scope_next;

		remove_symbol( del );
		free_symbol( del );
	}
}

/*
void compute_scope_addr( symbol* start, symbol* prev_scope )
{
	int cnt	= scope_size( prev_scope );

	for( ; start; start = start->scope_next, cnt++ )
		start->offset = cnt;
}
*/

int dt_size( short dt )
{
	int		size	= 0;

	/* Get size for type */
	if( dt == DT_INT )
		size = (int)sizeof( int );
	else if( dt == DT_FLOAT )
		size = (int)sizeof( double );
	else if( dt == DT_STRING )
		size = (int)sizeof( char* );
	/* printf( "%d = %d\n", dt, size ); */

	return size;
}

int scope_size( symbol* start )
{
	int	size	= 0;

	for( ; start; start = start->scope_next )
		size += dt_size( start->data_type );

	return size;
}

/* Dump the table content */
void dump_symbols( void )
{
	int 	i;
	symbol*	link;

	fprintf( stderr, "--- Symbol Table ---\n" );

	for( i = 0; i < HASH_SIZE; i++ )
	{
		if( !symtab[i] )
			continue;

		fprintf( stderr, "Index %02d: ", i );
		for( link = symtab[i]; link; link = link->next )
		{
			if( link != symtab[i] )
				fprintf( stderr, "          " );

			fprintf( stderr, "%s ", ( link->sym_type == SYM_VAR ?
										"global" : "      " ) );
			fprintf( stderr, "%s [%s]", link->name,
						GET_DT_NAME( link->data_type ) );

			if( link->sym_type == SYM_VAR && link->init_val )
			{
				fprintf( stderr, " = %s", get_value( link->init_val ) );
			}

			fprintf( stderr, "\n" );
		}
	}

	fprintf( stderr, "--------------------\n" );
}

uchar* get_value( value* val )
{
	static	uchar	tmp[ 20 + 1 ];

	switch( val->type )
	{
		case DT_INT:
			sprintf( tmp, "%d", val->value._int );
			return tmp;
		case DT_FLOAT:
			sprintf( tmp, "%f", val->value._float );
			return tmp;
		case DT_STRING:
			return get_string( val->value._string );

		default:
			break;
	}

	return "[unable to display]";
}

