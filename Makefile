# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: saalarco <saalarco@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/04 17:50:16 by saalarco          #+#    #+#              #
#    Updated: 2025/04/20 13:55:55 by saalarco         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

###############################################################################
#                              PROJECT SETTINGS                               #
###############################################################################

NAME		:= pipex

# Compiler & Flags
CC		:= cc
FLAGS    	:= -g -Wall -Wextra -Werror
MAKEFLAGS += --no-print-directory

# Headers
HEADERS	= -I ./include

# Libraries
LIBFT		= ./libft
LIBFT_LIB  := $(LIBFT)/libft.a
LIBS	=  -lm ${LIBFT}/libft.a 

ifdef VERBOSE
    SILENT :=
else
    SILENT := @
endif

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

SRCS		= src/main.c \
	src/child1/child1_cmd1.c \
	src/child2/child2_cmd2.c
                       
OBJS        := $(SRCS:.c=.o)

###############################################################################
#                              TEST DIRECTORIES & FILES                       #
###############################################################################

TEST_INPUT_DIR := tests/input
TEST_OUTPUT_DIR := tests/output
PIPEX_BIN := ./pipex

###############################################################################
#                              DEFAULT RULES                                  #
###############################################################################

.PHONY: all clean fclean re test test_clean

all: $(NAME)

# Compile the main executable 
$(NAME): $(OBJS) $(LIBFT_LIB) 
	@echo "$(BLUE)[Linking]$(CLR_RMV) $@"
	$(SILENT)$(CC) $(FLAGS) $^ $(LIBS) $(HEADERS) -o $@

# Generic rule to build object files from .c
%.o: %.c
	@echo "$(GREEN)[Compiling]$(CLR_RMV) $<"
	$(SILENT)$(CC) $(FLAGS) -c $< -o $@ $(HEADERS)

###############################################################################
#                              LIBRARIES                                      #
###############################################################################

$(LIBFT_LIB):
	@echo "Building libft..."
	@$(MAKE) -C $(LIBFT)

###############################################################################
#                              TESTS                                          #
###############################################################################

test: all
	@mkdir -p $(TEST_OUTPUT_DIR)
	@echo "$(BLUE)[Running test]$(CLR_RMV)"
	@bash tests/test_runner.sh "$(PIPEX_BIN)" "$(TEST_INPUT_DIR)" "$(TEST_OUTPUT_DIR)"


###############################################################################
#                              CLEAN RULES                                    #
###############################################################################

clean:
	@echo "$(RED)[Cleaning objects]$(CLR_RMV)"
	$(SILENT)rm -rf $(OBJS)
	$(SILENT)make -C $(LIBFT) clean


fclean: clean
	@echo "$(RED)[Removing executables]$(CLR_RMV)"
	$(SILENT)make -C $(LIBFT) fclean
	$(SILENT)rm -rf $(NAME)

test_clean:
	@echo "$(RED)[Cleaning test outputs]$(CLR_RMV)"
	@rm -rf $(TEST_OUTPUT_DIR)

re: fclean all
