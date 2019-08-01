/* -MODULE----------------------------------------------------------------------
MinC
Copyright (C) 2008 by Phorward Software Technologies, Jan Max Meyer
http://www.phorward-software.com ++ mail@phorward-software.com

File:	util.c
Author:	Jan Max Meyer
Usage:	Utility functions
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

/*
 * Functions
 */

/* Make a dynamic copy of a pointer */
void* memdup( void* ptr, size_t size )
{
	void* p;

	if( !( p = MALLOC( size ) ) )
		return (void*)NULL;

	memcpy( p, ptr, size );

	return p;
}

/* Map a file and return memory address */
uchar* map_file( uchar* path )
{
	uchar*	content;
	uchar*	p;
	FILE*	f;

	if( !( f = fopen( path, "rb" ) ) )
		return (uchar*)NULL;

	fseek( f, 0L, SEEK_END );
	
	content = (uchar*)MALLOC( ( ftell( f ) + 1 ) * sizeof( uchar ) );
	OUTOFMEM( content );

	fseek( f, 0L, SEEK_SET );

	for( p = content; !feof( f ); p++ )
		*p = (uchar)fgetc( f );

	*(p-1) = '\0';
	fclose( f );

	return content;
}

