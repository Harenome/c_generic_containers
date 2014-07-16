################################################################################
# Paths
################################################################################

PATH_SRC = src
PATH_OBJ = obj
PATH_DOC = doc
PATH_LIB = lib
PATH_BIN = bin
PATH_TESTS = tests
PATH_INCLUDE = include

################################################################################
# Files
################################################################################

vpath %.h $(PATH_INCLUDE) $(PATH_TESTS)/include $(PATH_INCLUDE)/cgc
vpath %.c $(PATH_SRC) $(PATH_TESTS)/src $(PATH_SRC)/cgc
vpath %.o $(PATH_OBJ)
vpath %.a $(PATH_LIB)
vpath %.so $(PATH_LIB)

################################################################################
# Flags, first pass.
################################################################################

FLAGS_CC_DEBUG = -g
FLAGS_CC_WARNINGS = -Wall -Wextra -Wfloat-equal -Wdouble-promotion \
	-Wswitch-default -Winit-self -Wshadow -Wbad-function-cast -Wcast-qual \
	-Wcast-align -Wconversion -Wlogical-op -Wstrict-prototypes -Wnested-externs
FLAGS_CC_OPTIMIZATIONS = -O0
FLAGS_CC_INCLUDE = -I$(PATH_INCLUDE)

################################################################################
# Conventionnal variables
################################################################################

# These are the only variables the user is *allowed* to override.

## Shell
SHELL = /bin/sh

## Programs
# Implicitly defined
CC = gcc
AR = ar
RM = rm -rf

# Extras (not really conventionnal)
DOC = doxygen
MKDIR = mkdir -p
CP = cp
ECHO = echo

# Install
INSTALL = install
INSTALL_PROGRAM = $(INSTALL)
INSTALL_DATA = $(INSTALL) -m 644

## Flags
ARFLAGS = crvs
CFLAGS = $(FLAGS_CC_DEBUG) $(FLAGS_CC_WARNINGS) $(FLAGS_CC_OPTIMIZATION)
LDFLAGS = -L$(PATH_LIB)
LDLIBS = -lcgc

## Directories
# In most situations, changing only PREFIX is enough.
#
# 		make install PREFIX=/opt/cgc
PREFIX = /usr/local
EXEC_PREFIX = $(PREFIX)
BINDIR = $(EXEC_PREFIX)/bin
SBINDIR = $(EXEC_PREFIX)/sbin
LIBEXECDIR = $(EXEC_PREFIX)/libexec
DATAROOTDIR = $(PREFIX)/share
DATADIR = $(DATAROOTDIR)
SYSCONFDIR = $(PREFIX)/etc
SHAREDSTATEDIR = $(PREFIX)/com
LOCALSTATEDIR = $(PREFIX)/var
RUNSTATEDIR = $(LOCALSTATEDIR)/run
INCLUDEDIR = $(PREFIX)/include
DOCDIR = $(DATAROOTDIR)/doc/cgc
LIBDIR = $(EXEC_PREFIX)/lib
MANDIR = $(DATAROOTDIR)/man

################################################################################
# Flags, second pass
################################################################################

# The user is allowed to override CFLAGS. But there are minimal requirements.
# Ensure these requirements are set even if CFLAGS is empty.
FLAGS_CC = -std=c99 -pedantic $(FLAGS_CC_INCLUDE) $(CFLAGS)
FLAGS_CC_LINK = $(LDFLAGS) $(LDLIBS)
FLAGS_CC_UNIT_TESTS = -lcunit $(FLAGS_CC_LINK)

################################################################################
# Compiling
################################################################################

all : libcgc.a

%.o: %.c | obj_dir
	$(CC) $(FLAGS_CC) -o $(PATH_OBJ)/$@ -c $<

## CGC
list.o: list.c list.h types.h common.h
queue.o: queue.c queue.h types.h common.h list.h
stack.o: stack.c stack.h types.h common.h list.h
vector.o: vector.c vector.h types.h common.h
string_vector.o: string_vector.c string_vector.h types.h common.h

libcgc.a: list.o vector.o string_vector.o queue.o stack.o | lib_dir
	$(AR) $(ARFLAGS) $(PATH_LIB)/libcgc.a $(PATH_OBJ)/list.o $(PATH_OBJ)/vector.o \
		$(PATH_OBJ)/string_vector.o $(PATH_OBJ)/queue.o \
		 $(PATH_OBJ)/stack.o

## Tests
test_list.o: test_list.c list.h
test_vector.o: test_vector.c vector.h
test_string_vector.o: test_string_vector.c string_vector.h

test_list: test_list.o libcgc.a | bin_dir
	$(CC) -o $(PATH_BIN)/test_list $(PATH_OBJ)/test_list.o $(FLAGS_CC_LINK)

test_vector: test_vector.o libcgc.a  | bin_dir
	$(CC) -o $(PATH_BIN)/test_vector $(PATH_OBJ)/test_vector.o $(FLAGS_CC_LINK)

test_string_vector: test_string_vector.o libcgc.a | bin_dir
	$(CC) -o $(PATH_BIN)/test_string_vector $(PATH_OBJ)/test_string_vector.o $(FLAGS_CC_LINK)

tests: test_list test_vector test_string_vector libcgc.a | bin_dir

################################################################################
# Directories
################################################################################

doc:
	@$(DOC)

obj_dir:
	@$(MKDIR) $(PATH_OBJ)

lib_dir:
	@$(MKDIR) $(PATH_LIB)

bin_dir:
	@$(MKDIR) $(PATH_BIN)

################################################################################
# (Un)Installing
################################################################################

# DESTDIR: Support staged (un)installs.
# DESTDIR should be defined as a command line argument (thus it is empty by
# default).
#
# 		$ make install DESTDIR=/tmp/stage
# 		$ make uninstall DESTDIR=/tmp/stage
#
# Note: DESTDIR is different from PREFIX!

install: libcgc.a
	@$(MKDIR) $(DESTDIR)$(INCLUDEDIR) \
		&& $(CP) -R $(PATH_INCLUDE)/cgc $(DESTDIR)$(INCLUDEDIR)/cgc \
		&& echo "install: $(DESTDIR)$(INCLUDEDIR)/cgc"
	@$(INSTALL_DATA) -D $(PATH_LIB)/libcgc.a $(DESTDIR)$(LIBDIR)/libcgc.a \
		&& echo "install: $(DESTDIR)$(LIBDIR)/libcgc.a"

uninstall:
	@$(RM) $(DESTDIR)$(INCLUDEDIR)/cgc \
		&& echo "uninstall: $(DESTDIR)$(INCLUDEDIR)/cgc"
	@$(RM) $(DESTDIR)$(LIBDIR)/libcgc.a \
		&& echo "uninstall: $(DESTDIR)$(LIBDIR)/libcgc.a"

################################################################################
# Cleaning
################################################################################

clean:
	@$(RM) $(PATH_BIN) $(PATH_OBJ) $(PATH_LIB)
	@$(ECHO) "Clean."

cleandoc: clean_doc
clean_doc:
	@$(RM) $(PATH_DOC)

cleanall: clean_all
clean_all: clean clean_doc
	@$(ECHO) "Super clean."
