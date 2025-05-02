# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: saalarco <saalarco@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/04 17:50:16 by saalarco          #+#    #+#              #
#    Updated: 2025/05/02 12:30:06 by saalarco         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

###############################################################################
#                              PROJECT SETTINGS                               #
###############################################################################

NAME		:= pipex
CC			:= cc
CFLAGS    	:= -Wall -Wextra -Werror
# AddressSanitizer, UndefinedBehaviorSanitizer and -g3 for debugging
SAN_FLAGS	:= -fsanitize=address,undefined -g3
# Fuzzing flags. Include or exclude code in source files using #ifdef FUZZING
FUZZ_FLAGS	:= -fsanitize=fuzzer,address -DFUZZING
# -fprofile-arcs -ftest-coverage for code coverage  (execution count and generate .gcno and gcda fles
COV_FLAGS	:= -fprofile-arcs -ftest-coverage -g
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

.PHONY: all debug fsanitize fuzz valgrind coverage format lint doc \
        test test_clean clean fclean re help


###############################################################################
#                              COLOR SETTINGS                                 #
###############################################################################

CLR_RMV   := \033[0m
RED       := \033[1;31m
GREEN     := \033[1;32m
YELLOW    := \033[1;33m
BLUE      := \033[1;34m
CYAN      := \033[1;36m
BPURPLE   := \033[1;35m

################################################################################
#                                 PROGRAM'S SRCS                               #
################################################################################

SRCS	:= src/main.c \
			src/child1/child1_cmd1.c \
			src/child2/child2_cmd2.c      
OBJS	:= $(SRCS:.c=.o)

###############################################################################
#                              TEST DIRECTORIES & FILES                       #
###############################################################################

TEST_INPUT_DIR  := tests/input
TEST_OUTPUT_DIR := tests/output

###############################################################################
#                              DEFAULT RULES                                  #
###############################################################################

all: $(NAME)

# -O0 for debugging (disables optimization wich mmkes debugging easier)
debug: CFLAGS += -g3 -O0
debug: $(NAME)

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
$(NAME): $(OBJS) $(LIBFT_LIB) 
	@echo "$(CYAN)[pipex]$(CLR_RMV) $@"
	$(Q)$(CC) $(CFLAGS) $^ $(LIBS) $(HEADERS) $(LDFLAGS) -o $@

# Generic rule to build object files from .c
%.o: %.c
	@echo "$(CYAN)[pipex]$(CLR_RMV) $<"
	$(Q)$(CC) $(CFLAGS) -c $< -o $@ $(HEADERS)

###############################################################################
#                              LIBRARIES                                      #
###############################################################################

$(LIBFT_LIB):
	@echo "$(YELLOW)[libft]$(CLR_RMV) building"
	$(Q)$(MAKE) -C $(LIBFT_DIR)

###############################################################################
#                              TESTS                                          #
###############################################################################

test: all
	$(Q)mkdir -p $(TEST_OUTPUT_DIR)
	@echo "$(CYAN)[test]$(CLR_RMV) bash tests/test_runner.sh"
	$(Q)bash tests/test_runner.sh "$(PIPEX_BIN)" "$(TEST_INPUT_DIR)" "$(TEST_OUTPUT_DIR)"

valgrind: all
	@echo "$(CYAN)[valgrind]$(CLR_RMV) running"
	$(Q)bash tools/valgrind_suite.sh "$(PIPEX_BIN)"

fuzz-run: fuzz
	$(Q)$(PIPEX_BIN)_fuzz

test_clean:
	$(Q)rm -rf $(TEST_OUTPUT_DIR)

###############################################################################
#                           LINT/FORMAT/DOCS                                  #
###############################################################################

format:
	@echo "$(BLUE)[clang‑format]$(CLR_RMV)"
	$(Q)find src include -name '*.[ch]' | xargs clang-format -i

lint: format
	@echo "$(BLUE)[norminette]$(CLR_RMV)"
	$(Q)norminette src include

doc:
	@echo "$(BLUe)[doxygen]$(CLR_RMV)"
	$(Q)doxygen docs/Doxyfile

###############################################################################
#                              CLEAN RULES                                    #
###############################################################################

clean:
	@echo "$(RED)[Cleaning objects]$(CLR_RMV)"
	$(Q)rm -rf $(OBJS)
	$(Q)$(MAKE) -C $(LIBFT_DIR) clean


fclean: clean
	@echo "$(RED)[Removing executables]$(CLR_RMV)"
	$(Q)rm -f $(NAME) *.gcno *.gcda coverage.info
	$(Q)rm -rf coverage_html
	$(Q)$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

help:
	@echo "Targets:"
	@echo "  all           – release build (default)"
	@echo "  debug         – -g3 -O0 build"
	@echo "  fsanitize     – address+UB sanitizers"
	@echo "  fuzz          – libFuzzer instrumentation"
	@echo "  valgrind      – run Valgrind test suite"
	@echo "  coverage      – GCov + HTML report"
	@echo "  test          – run bash test_runner.sh"
	@echo "  format / lint – clang‑format + norminette"
	@echo "  doc           – generate Doxygen"
	@echo "  clean / fclean / re"
