################################################################################
# Programs
################################################################################

CC = gcc

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
# Flags
################################################################################

FLAGS_CC_DEBUG = -g
FLAGS_CC_WARNINGS = -Wall -Wextra -Wfloat-equal -Wdouble-promotion \
	-Wswitch-default -Winit-self -Wshadow -Wbad-function-cast -Wcast-qual \
	-Wcast-align -Wconversion -Wlogical-op -Wstrict-prototypes -Wnested-externs
FLAGS_CC_INCLUDE = -I$(PATH_INCLUDE)
FLAGS_CC = $(FLAGS_CC_INCLUDE) -std=c99 -pedantic -O0 $(FLAGS_CC_WARNINGS) $(FLAGS_CC_DEBUG)
FLAGS_CC_LIB = -L$(PATH_LIB)
FLAGS_CC_LINK = $(FLAGS_CC_LIB) -lcgc
FLAGS_CC_UNIT_TESTS = -lcunit $(FLAGS_CC_LINK)

################################################################################
# Files
################################################################################

vpath %.h $(PATH_INCLUDE) $(PATH_TESTS)/include
vpath %.c $(PATH_SRC) $(PATH_TESTS)/src
vpath %.o $(PATH_OBJ)
vpath %.a $(PATH_LIB)
vpath %.so $(PATH_LIB)

################################################################################
# Compiling
################################################################################

all : tests

%.o: %.c | obj_dir
	$(CC) $(FLAGS_CC) -o $(PATH_OBJ)/$@ -c $<

## CGC
cgc/list.o: cgc/list.c cgc/list.h cgc/types.h cgc/common.h

libcgc.a: cgc/list.o | lib_dir
	ar crvs $(PATH_LIB)/libcgc.a $(PATH_OBJ)/cgc/list.o

## Tests
test_list.o: test_list.c cgc/list.h

tests: test_list.o libcgc.a | bin_dir
	$(CC) -o $(PATH_BIN)/test_list $(PATH_OBJ)/test_list.o \
		$(PATH_OBJ)/cgc/list.o $(FLAGS_CC_LINK)

################################################################################
# Directories
################################################################################

doc: clean_doc
	@doxygen

obj_dir:
	@mkdir -p $(PATH_OBJ)
	@mkdir -p $(PATH_OBJ)/cgc

lib_dir:
	@mkdir -p $(PATH_LIB)

bin_dir:
	@mkdir -p $(PATH_BIN)

################################################################################
# Cleaning
################################################################################

clean:
	@rm -rf $(PATH_BIN) $(PATH_OBJ) $(PATH_LIB)
	@echo "Clean."

cleandoc: clean_doc
clean_doc:
	@rm -rf $(PATH_DOC)

cleanall: clean_all
clean_all: clean clean_doc
	@echo "Super clean."
