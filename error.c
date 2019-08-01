/* -MODULE----------------------------------------------------------------------
MinC
Copyright (C) 2008 by Phorward Software Technologies, Jan Max Meyer
http://www.phorward-software.com ++ mail@phorward-software.com

File:	error.c
Author:	Jan Max Meyer
Usage:	Error reporting functions
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

void error( int flags, char* fmt, ... )
{
	va_list	va;

	va_start( va, fmt );
	fprintf( stderr, "Error: " );
	vfprintf( stderr, fmt, va );
	fprintf( stderr, "\n" );
	va_end( va );
}

