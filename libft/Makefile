# ******** VARIABLES ********

NAME = libft.a
CC = gcc
CFLAGS = -g -Wall -Wextra -Werror
AR = ar rcs
INCLUDE = -Iinclude
SRC := src/ft_atoi.c \
		src/ft_bzero.c \
		src/ft_calloc.c \
       	src/ft_isalnum.c \
       	src/ft_isalpha.c \
       	src/ft_isascii.c \
       	src/ft_isdigit.c \
       	src/ft_isprint.c \
		src/ft_itoa.c \
		src/ft_memchr.c \
		src/ft_memcmp.c \
		src/ft_memcpy.c \
		src/ft_memmove.c \
		src/ft_memset.c \
		src/ft_putchar_fd.c \
		src/ft_putendl_fd.c \
		src/ft_putnbr_fd.c \
		src/ft_putstr_fd.c \
		src/ft_split.c \
		src/ft_strchr.c \
		src/ft_strdup.c \
		src/ft_striteri.c \
		src/ft_strjoin.c \
		src/ft_strlcat.c \
		src/ft_strlcpy.c \
		src/ft_strlen.c \
		src/ft_strmapi.c \
		src/ft_strncmp.c \
		src/ft_strnstr.c \
		src/ft_strrchr.c \
		src/ft_strtrim.c \
		src/ft_substr.c \
		src/ft_tolower.c \
		src/ft_toupper.c \
		src/lists/ft_lstadd_back_bonus.c \
		src/lists/ft_lstadd_front_bonus.c \
		src/lists/ft_lstclear_bonus.c \
		src/lists/ft_lstdelone_bonus.c \
		src/lists/ft_lstiter_bonus.c \
		src/lists/ft_lstlast_bonus.c \
		src/lists/ft_lstmap_bonus.c \
		src/lists/ft_lstnew_bonus.c \
		src/lists/ft_lstsize_bonus.c \
		src/ft_printf/ft_printf.c \
		src/ft_printf/ft_putchar_count.c \
		src/ft_printf/ft_putnbr_count.c \
		src/ft_printf/ft_putstr_count.c \
		src/ft_printf/ft_putbase_count.c \
		src/get_next_line/get_next_line.c \
		src/get_next_line/get_next_line_utils.c \
		src/get_file/ft_file_to_list.c \
		src/get_file/ft_list_to_line_array.c \
		src/get_file/ft_list_to_buffer.c \
		src/get_file/ft_line_count.c \
		src/get_file/ft_line_array_to_map.c \
		src/get_file/ft_file_read_all.c \
		src/get_file/ft_file_print_lines.c \
		src/get_file/ft_file_print_buffer.c \
		src/get_file/ft_node_text_free.c \
		src/get_file/ft_file_free.c \
		src/get_file/ft_get_line_at.c \
		src/get_file/ft_file_get_line.c

OBJ_DIR = obj
TEST_DIR     = tests
OBJ = $(SRC:%.c=$(OBJ_DIR)/%.o)

# Test source files
TEST_LIBFT_SRCS       	= $(TEST_DIR)/test_libft.c
TEST_GET_FILE_SRCS  	= $(TEST_DIR)/test_get_file_module.c
# Binaries for tests
TEST_LIBFT_BIN     = ./tests/test_libft
TEST_GET_FILE_BIN  = ./tests/test_get_file_module

# ******** REGLAS ********

.PHONY: all re clean fclean

all : $(NAME)

$(NAME): $(OBJ)
	@$(AR) $(NAME) $(OBJ)
	@printf "$(YELLOW)$(BLINK)the libftbrary is open$(RESET)\n"

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(OBJ_DIR):
	mkdir -p \
	$(OBJ_DIR)/src \
	$(OBJ_DIR)/src/lists \
	$(OBJ_DIR)/src/ft_printf \
	$(OBJ_DIR)/src/get_next_line \
	$(OBJ_DIR)/src/get_file

# Compile the test executables, linking against your library
test: $(NAME) $(TEST_LIBFT_BIN) $(TEST_GET_FILE_BIN)

$(TEST_LIBFT_BIN): $(TEST_LIBFT_SRCS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(NAME)

$(TEST_GET_FILE_BIN): $(TEST_GET_FILE_SRCS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(NAME)

# Optionally run the tests automatically
run_tests: test
	@echo "Running tests..."
	@./$(TEST_LIBFT_BIN)
	@./$(TEST_GET_FILE_BIN)

# If you want a separate rule to run Valgrind on your tests:
valgrind: test
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TEST_GET_FILE_BIN)

# Test from codespaces: /home/codespace/francinette/tester.sh -in --strict

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME) $(TEST_LIBFT_BIN) $(TEST_GET_FILE_BIN)

re: fclean all