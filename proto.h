/* parse.c */
int parse( uchar* src );

/* main.c */

/* symtab.c */
symbol* new_symbol( uchar* name, short sym_type );
symbol* free_symbol( symbol* sym );
symbol* find_symbol( uchar* name );
BOOLEAN remove_symbol( symbol* sym );
void add_symbol( symbol* sym );
symbol* scope_append( symbol* begin, symbol* engage );
void free_scope( symbol* start );
int dt_size( short dt );
int scope_size( symbol* start );
void dump_symbols( void );
uchar* get_value( value* val );

/* strtab.c */
uchar* get_string( int idx );
int put_string( uchar* str );

/* codegen.c */
uchar* instr_name( instr i );
void print_prog( void );
int current_address( void );
int next_address( void );
mcode* get_address( int idx );
int gen( instr cmd, char* param );
short promote_gen( instr cmd, char* param, short first, short second );

/* asm.c */
void gen_x86( FILE* stream );

/* error.c */
void error( int flags, char* fmt, ... );

/* util.c */
void* memdup( void* ptr, size_t size );
uchar* map_file( uchar* path );

