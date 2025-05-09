# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: saalarco <saalarco@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/04 17:50:16 by saalarco          #+#    #+#              #
#    Updated: 2025/05/09 19:15:39 by saalarco         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

####
# VARIABLES
####

NAME		:= pipex
CC			:= cc
CFLAGS    	:= -Wall -Wextra -Werror
# AddressSanitizer, UndefinedBehaviorSanitizer and -g3 for debugging
SAN_FLAGS	:= -fsanitize=address,undefined -g3
# Fuzzing flags. Include or exclude code in source files using #ifdef FUZZING
FUZZ_FLAGS	:= -fsanitize=fuzzer,address -DFUZZING
# -fprofile-arcs -ftest-coverage for code coverage  (execution count and generate .gcno and gcda fles
COV_FLAGS	:= -fprofile-arcs -ftest-coverage -g
# criterion flags (inside container)
CRIT_CFLAGS := $(shell pkg-config --cflags criterion 2>/dev/null)
CRIT_LIBS   := $(shell pkg-config --libs   criterion 2>/dev/null)
# criterion AND debugging tools
DBG_FLAGS     := -g3 -O0 -DCRITERION_DISABLE_FORKING
# Headers
HEADERS		:= -I ./include
# Libraries
LIBFT_DIR	:= libft
LIBFT_LIB	:= $(LIBFT_DIR)/libft.a
LIBS		:=  -lm ${LIBFT_LIB}
# Pipex bin
PIPEX_BIN	:= ./$(NAME)
# no print directory
MAKEFLAGS += --no-print-directory

# Verbose mode (make VERBOSE=1)
ifdef VERBOSE
    Q :=
else
    Q := @
endif

####
# COLOR SETTINGS
####

CLR_RMV   := \033[0m
RED       := \033[1;31m
GREEN     := \033[1;32m
YELLOW    := \033[1;33m
BLUE      := \033[1;34m
CYAN      := \033[1;36m
BPURPLE   := \033[1;35m

####
# PROGRAM'S SRCS
####

SRCS_PROD := src/main.c \
    src/child1/child1_cmd1.c \
    src/child2/child2_cmd2.c \
    src/utils/cmdpath.c

OBJS_PROD := $(SRCS_PROD:.c=.o)

# core objects without main for testing unit
SRCS_CORE  := $(filter-out src/main.c,$(SRCS_PROD))
OBJS_CORE  := $(SRCS_CORE:.c=.o)

####
# TEST DIRECTORIES & FILES
####

TEST_INPUT_DIR  := tests/input
TEST_OUTPUT_DIR := tests/output

####
# DEFAULT RULES
####

.PHONY: all debug fsanitize fuzz valgrind coverage format lint doc \
        test test_clean clean fclean re help

all: $(NAME)

# -O0 for debugging (disables optimization wich mmkes debugging easier)
debug: CFLAGS += -g3 -O0
debug: $(NAME)

# instant valgrind, UndefinedBehaviorSanitizer (UBSan) and AddressSanitizer (ASan)	
fsanitize: CFLAGS += $(SAN_FLAGS)
fsanitize: $(NAME)

# LDFLAGS is standard Makefile variable for linker flags (for coverage reports)
coverage: CFLAGS += $(COV_FLAGS)
coverage: LDFLAGS += $(COV_FLAGS)
coverage: $(NAME)
	$(Q)lcov --capture --directory . --output-file coverage.info
	$(Q)genhtml coverage.info --output-directory coverage_html
	@echo "$(GRN)[coverage] open coverage_html/index.html$(CLR)"

# Compile the main executable 
$(NAME): $(OBJS_PROD) $(LIBFT_LIB) 
	@echo "$(CYAN)[pipex]$(CLR_RMV) $@"
	$(Q)$(CC) $(CFLAGS) $^ $(LIBS) $(HEADERS) $(LDFLAGS) -o $@

# Generic rule to build object files from .c
%.o: %.c
	@echo "$(CYAN)[pipex]$(CLR_RMV) $<"
	$(Q)$(CC) $(CFLAGS) -c $< -o $@ $(HEADERS)

####
# LIBRARIES
####

$(LIBFT_LIB):
	@echo "$(YELLOW)[libft]$(CLR_RMV) building"
	$(Q)$(MAKE) -C $(LIBFT_DIR)

####
# TESTS
####

# general tests
test: unit
	$(Q)mkdir -p $(TEST_OUTPUT_DIR)
	@echo "$(CYAN)[test]$(CLR_RMV) bash tests/test_runner.sh"
	$(Q)bash tests/test_runner.sh "$(PIPEX_BIN)" "$(TEST_INPUT_DIR)" "$(TEST_OUTPUT_DIR)"

# unit tests objects
UNIT_SRCS := $(wildcard tests/unit/*.c)
UNIT_OBJS := $(UNIT_SRCS:.c=.o)
UNIT_BIN  := unit_tests

unit: $(UNIT_BIN)

# Compile the unit test executable (core + unit tests)
$(UNIT_BIN): $(UNIT_OBJS) $(OBJS_CORE) $(LIBFT_LIB)
	$(Q)$(CC) $(CFLAGS) $(CRIT_CFLAGS) \
		$(UNIT_OBJS) $(OBJS_CORE) $(LIBFT_LIB) $(CRIT_LIBS) \
		-o $@
	./$@ --color --fail-fast

UNIT_BIN_DBG  := unit_tests_dbg
TEST ?=
PORT ?=1234
unit-debug: CFLAGS += $(DBG_FLAGS)
unit-debug: $(UNIT_BIN_DBG)
	@./$(UNIT_BIN_DBG) --debug=gdb --debug-transport=tcp:$(PORT) \
	    $(if $(TEST),--filter $(TEST))

$(UNIT_BIN_DBG): $(UNIT_OBJS) $(OBJS_CORE) $(LIBFT_LIB)
	$(CC) $(CFLAGS) $(CRIT_CFLAGS) \
	     $^ $(CRIT_LIBS) -o $@

# script to target remote :1234 from gdbserver
attach-gdb:
	@tools/attach_gdb.sh $(PORT)

valgrind: all
	@echo "$(CYAN)[valgrind]$(CLR_RMV) running"
	$(Q)bash tools/valgrind_suite.sh "$(PIPEX_BIN)"

fuzz-run: fuzz
	$(Q)$(PIPEX_BIN)_fuzz

test_clean:
	$(Q)rm -rf $(TEST_OUTPUT_DIR)

####
# LINT/FORMAT/DOCS
####

format:
	@echo "$(BLUE)[clang‑format]$(CLR_RMV)"
	$(Q)find src include -name '*.[ch]' | xargs clang-format -i

lint: format
	@echo "$(BLUE)[norminette]$(CLR_RMV)"
	$(Q)norminette src include

doc:
	@echo "$(BLUe)[doxygen]$(CLR_RMV)"
	$(Q)doxygen docs/Doxyfile

####
# CLEANING RULES
####

clean:
	@echo "$(RED)[Cleaning objects]$(CLR_RMV)"
	$(Q)rm -rf $(OBJS_PROD)
	$(Q)$(MAKE) -C $(LIBFT_DIR) clean


fclean: clean
	@echo "$(RED)[Removing executables]$(CLR_RMV)"
	$(Q)rm -f $(NAME) *.gcno *.gcda coverage.info
	$(Q)rm -rf coverage_html
	$(Q)rm -f $(UNIT_BIN) $(UNIT_OBJS)
	$(Q)rm -f $(UNIT_BIN_DBG)
	$(Q)$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

####
# HELP
####

help:
	@echo "Targets:"
	@echo "  all           – release build (default)"
	@echo "  debug         – -g3 -O0 build"
	@echo "  unit-debug    – debug build TEST=some_suite/some_test"
	@echo "  unit		   – unit tests"
	@echo "  fsanitize     – address+UB sanitizers"
	@echo "  fuzz          – libFuzzer instrumentation"
	@echo "  valgrind      – run Valgrind test suite"
	@echo "  coverage      – GCov + HTML report"
	@echo "  test          – run bash test_runner.sh"
	@echo "  format / lint – clang‑format + norminette"
	@echo "  doc           – generate Doxygen"
	@echo "  clean / fclean / re"
