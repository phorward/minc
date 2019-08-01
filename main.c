/* -MODULE----------------------------------------------------------------------
MinC
Copyright (C) 2008 by Phorward Software Technologies, Jan Max Meyer
http://www.phorward-software.com ++ mail@phorward-software.com

File:	util.c
Author:	Jan Max Meyer
Usage:	Main function
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

int main( int argc, char** argv )
{
	char*	src;

	if( argc < 2 )
	{
		fprintf( stderr, "minc: No input file.\n" );
		return EXIT_FAILURE;
	}

	if( !( src = map_file( argv[1] ) ) )
	{
		fprintf( stderr, "minc: File no found: %s\n", argv[1] );
		return EXIT_FAILURE;
	}

	printf( "%s\n--\n", src );
	parse( src );
	gen_x86( stdout );

	FREE( src );

	return EXIT_SUCCESS;
}
