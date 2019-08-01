/* -MODULE----------------------------------------------------------------------
MinC
Copyright (C) 2008 by Phorward Software Technologies, Jan Max Meyer
http://www.phorward-software.com ++ mail@phorward-software.com

File:	strtab.c
Author:	Jan Max Meyer
Usage:	String table management
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
uchar**		strtab			= (uchar**)NULL;
int			strtab_cnt		= 0;

/*
 * Functions
 */

/* Get string from table via index */
uchar* get_string( int idx )
{
	if( idx >= strtab_cnt ) 
		return "";

	return strtab[ idx ];
}

/* Add string to table, if required, else reuse the string */
int put_string( uchar* str )
{
	int i;

	for( i = 0; i < strtab_cnt; i++ )
		if( !strcmp( strtab[ i ], str ) )
			return i;

	if( !( strtab_cnt % MALLOC_STEP ) )
		strtab = (uchar**)REALLOC( (uchar**)strtab,
					( strtab_cnt / MALLOC_STEP + 1 ) * MALLOC_STEP
						* sizeof( uchar* ) );
	
	strtab[ strtab_cnt++ ] = str;
	return strtab_cnt - 1;
}

