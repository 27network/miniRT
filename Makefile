# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/06 21:19:50 by kiroussa          #+#    #+#              #
#    Updated: 2024/02/22 02:42:38 by kiroussa         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			=	miniRT	

BUILD_DIR		=	build

SRC_DIR			=	src
SRC				=	main.c
SRC				:=	$(addprefix $(SRC_DIR)/, $(SRC))

OBJ_DIR			=	$(BUILD_DIR)/obj
OBJ				=	$(subst $(SRC_DIR)/,,$(SRC:.c=.o))
OBJ				:=	$(addprefix $(OBJ_DIR)/, $(OBJ))

INCLUDE_DIR		= 	include

MLX_DIR			=	MacroLibX
MLX				=	$(MLX_DIR)/libmlx.so

LIBFT_DIR		=	libft
LIBFT			=	$(LIBFT_DIR)/build/output/libft.so

CC				=	clang
CFLAGS			= 	-Wall -Wextra -Werror
ifeq ($(FUNMODE), 1)
	CFLAGS		+=	-O3
endif
COPTS			= 	-I $(INCLUDE_DIR) -I $(MLX_DIR)/$(INCLUDE_DIR)s -I $(LIBFT_DIR)/$(INCLUDE_DIR)

MAKE_CMD		=	make

RM				=	rm -rf

VALGRIND		=	valgrind
VALGRIND_OPTS	=	--suppressions=$(MLX_DIR)/valgrind.supp --leak-check=full --show-leak-kinds=all --track-origins=yes -s --verbose --log-file=valgrind-out.txt
VALGRIND_PROGRAM_ARGS	=	./scenes/valid/basic.rt

#
# Rules
#

all:			$(NAME)

$(NAME):		$(LIBFT) $(MLX) $(OBJ)
	$(CC) $(CFLAGS) $(COPTS) -o $(NAME) $(OBJ) $(LIBFT) $(MLX) -lSDL2 -lm $(LINKER_ARGS)

bonus:			COPTS += -DMINIRT_BONUS
bonus:			remake

rt:				COPTS += -DRT_MODE
rt:				remake

$(NAME_BONUS):	$(LIBFT) $(MLX) $(OBJ_BONUS)
	$(CC) $(CFLAGS) $(COPTS) -o $(NAME_BONUS) $(OBJ_BONUS) $(LIBFT) $(MLX) -lSDL2 -lm $(LINKER_ARGS)

$(LIBFT):
	$(MAKE_CMD) -j -C $(LIBFT_DIR) CFLAGS="$(CFLAGS)" all

$(MLX):
	$(MAKE_CMD) -j -C $(MLX_DIR) all

$(OBJ_DIR)/%.o:	$(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(COPTS) -c $< -o $@

remake:			oclean all

valgrind:
	$(VALGRIND) $(VALGRIND_OPTS) ./$(NAME) $(VALGRIND_PROGRAM_ARGS)

clean:			oclean
	make -C $(LIBFT_DIR) clean
	make -C $(MLX_DIR) clean

oclean:
	$(RM) $(OBJ_DIR) 

fclean:			clean
	make -C $(LIBFT_DIR) fclean
	make -C $(MLX_DIR) fclean
	$(RM) $(NAME) $(NAME_BONUS)

re:				fclean all

.PHONY:			all bonus clean oclean fclean remake re
