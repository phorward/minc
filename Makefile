#-------------------------------------------------------------------------------
# MinC
# Copyright (C) 2008 by Phorward Software Technologies, Jan Max Meyer
# http://www.phorward-software.com ++ mail@phorward-software.com
#
# Make-File:	Makefile
# Author:		Jan Max Meyer
#-------------------------------------------------------------------------------

include		../../include/Make.inc

#PROGRAM		=	$(RUN_DIR)$(PATH_SEP)$(MIN_LALR1)$(EXEEXT)
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
				proto.h \
				$(LLIST_LIB_H)

#-------------------------------------------------------------------------------

all: $(PROGRAM)
	@echo
	@echo --- Compilation succeeded! ---
	
$(PROGRAM): $(SRC) $(HEADERS) Makefile
	$(CC) $(CEXEOPTS) $(SRC)
	$(LLINK) $(LINKOPTS)$@ $(OBJ) $(LIBFILES)

$(PARSER):	$(PARSER_SRC)
	$(UNICC) -s -v -w -o $@ $(PARSER_SRC)

proto.h:	$(SRC)
	echo "">$@
	cproto_all "$(SRC)" $(INCLUDE_DIR) proto.h

clean_obj:
	-@$(RM) $(OBJ)

clean: clean_obj
	-@$(RM) $(PROGRAM)
	-@$(RM) $(PARSER)
