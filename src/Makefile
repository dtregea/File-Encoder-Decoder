#
# Created by gmakemake (Ubuntu Jul 25 2014) on Thu Apr 28 13:22:42 2022
#

#
# Definitions
#

.SUFFIXES:
.SUFFIXES:	.a .o .c .C .cpp .s .S
.c.o:
		$(COMPILE.c) $<
.C.o:
		$(COMPILE.cc) $<
.cpp.o:
		$(COMPILE.cc) $<
.S.s:
		$(CPP) -o $*.s $<
.s.o:
		$(COMPILE.cc) $<
.c.a:
		$(COMPILE.c) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.C.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.cpp.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%

CC =		gcc
CXX =		g++

RM = rm -f
AR = ar
LINK.c = $(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)
LINK.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS)
COMPILE.c = $(CC) $(CFLAGS) $(CPPFLAGS) -c
COMPILE.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) -c
CPP = $(CPP) $(CPPFLAGS)
########## Flags from header.mak

#
# NOTE: Makefile does not support ~/ for -L or -I; using $(HOME) 
#
#
# CPPFLAGS= -I $(INCLUDEPATH)
CFLAGS= -std=c99 -ggdb -Wall -Wextra -pedantic
#
# public project2 archive
#

#


########## End of flags from header.mak


CPP_FILES =	
C_FILES =	HeapDT.c decode.c encode.c packman.c packman_utils.c utilities.c
PS_FILES =	
S_FILES =	
H_FILES =	HeapDT.h decode.h encode.h packman_utils.h utilities.h
SOURCEFILES =	$(H_FILES) $(CPP_FILES) $(C_FILES) $(S_FILES)
.PRECIOUS:	$(SOURCEFILES)
OBJFILES =	HeapDT.o decode.o encode.o packman_utils.o utilities.o 

#
# Main targets
#

packman:	packman.o $(OBJFILES)
	$(CC) $(CFLAGS) -o packman packman.o $(OBJFILES)


#
# Dependencies
#

HeapDT.o:	HeapDT.h
decode.o:	decode.h packman_utils.h utilities.h
encode.o:	HeapDT.h encode.h packman_utils.h utilities.h
packman.o:	HeapDT.h decode.h encode.h packman_utils.h utilities.h
packman_utils.o:	packman_utils.h
utilities.o:	packman_utils.h utilities.h

#
# Housekeeping
#

Archive:	archive.tgz

archive.tgz:	$(SOURCEFILES) Makefile
	tar cf - $(SOURCEFILES) Makefile | gzip > archive.tgz

clean:
	-/bin/rm -f $(OBJFILES) packman.o test-rw-treefile.o core

realclean:        clean
	-/bin/rm -f packman test-rw-treefile 
