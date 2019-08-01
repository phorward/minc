#-------------------------------------------------------------------------------
# MinC
# Copyright (C) 2008 by Phorward Software Technologies, Jan Max Meyer
# http://www.phorward-software.com ++ mail@phorward-software.com
#
# Make-File:	Makefile
# Author:		Jan Max Meyer
#-------------------------------------------------------------------------------

PROGRAM		=	minc

PARSER		=	parse.c

PARSER_SRC	=	minc.par

SRC			=	$(PARSER) \
				main.c \
				symtab.c \
				strtab.c \
				codegen.c \
				asm.c \
				error.c \
				util.c

OBJ			=	parse$(OBJEXT) \
				main$(OBJEXT) \
				symtab$(OBJEXT) \
				strtab$(OBJEXT) \
				codegen$(OBJEXT) \
				asm$(OBJEXT) \
				error$(OBJEXT) \
				util$(OBJEXT) \

LIBFILES	=	

HEADERS		=	minc.h \
				proto.h

#-------------------------------------------------------------------------------

all: $(PROGRAM)
	@echo
	@echo --- Compilation succeeded! ---
	
$(PROGRAM): $(SRC) $(HEADERS) Makefile
	$(CC) -o $@ $(SRC)

$(PARSER):	$(PARSER_SRC)
	unicc -svwo parse $(PARSER_SRC)

proto.h:	$(SRC)
	pproto $(SRC) | sed -e "/int _/d" >$@

clean_obj:
	-@$(RM) $(OBJ)

clean: clean_obj
	-@$(RM) $(PROGRAM)
	-@$(RM) $(PARSER)
