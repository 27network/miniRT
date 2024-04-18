# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/06 21:19:50 by kiroussa          #+#    #+#              #
#    Updated: 2024/04/18 14:50:15 by rgramati         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			=	miniRT	
DEBUG			?=	0

BUILD_DIR		=	build

SRC				=	error/rt_err.c					\
					error/rt_errd.c					\
					error/rt_error_print.c			\
					error/rt_ok.c					\
					parse/scene/rt_parse_line.c		\
					parse/scene/rt_parse_scene.c	\
					parse/rt_parse_error.c 			\
					render/rt_render.c				\
					render/rt_hooks.c				\
					render/rt_rendering.c			\
					rt_scene_free.c					\
					rt_scene_init.c					\
					main.c

SRC_DIR			=	src
SRC				:=	$(addprefix $(SRC_DIR)/, $(SRC))

OBJ_DIR			=	$(BUILD_DIR)/obj
OBJ				=	$(subst $(SRC_DIR)/,,$(SRC:.c=.o))
OBJ				:=	$(addprefix $(OBJ_DIR)/, $(OBJ))

INCLUDE_DIR		= 	include

MLX_DIR			=	third-party/MacroLibX
MLX				=	$(MLX_DIR)/libmlx.so

LIBFT_DIR		=	third-party/libft
LIBFT			=	$(LIBFT_DIR)/libft.so

CC				=	clang
CFLAGS			= 	-Wall -Wextra -Werror
ifeq ($(FUNMODE), 1)
	CFLAGS		+=	-O3
endif
ifeq ($(DEBUG), 1)
	CFLAGS		+=	-g3
endif
COPTS			= 	-I $(INCLUDE_DIR) -I $(MLX_DIR)/$(INCLUDE_DIR)s -I $(LIBFT_DIR)/$(INCLUDE_DIR)

MAKE_CMD		=	make

RM				=	rm -rf

VALGRIND		=	valgrind
VALGRIND_OPTS	=	--suppressions=$(MLX_DIR)/valgrind.supp --leak-check=full --show-leak-kinds=all --track-origins=yes -q 
VALGRIND_PROGRAM_ARGS	=	./scenes/valid/minimalist.rt

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
	$(MAKE_CMD) -j -C $(LIBFT_DIR) CFLAGS="$(CFLAGS)" all DEBUG="$(DEBUG)"

$(MLX):
ifeq ($(DEBUG), 1)
	$(MAKE_CMD) -j -C $(MLX_DIR) DEBUG="true" all
else
	$(MAKE_CMD) -j -C $(MLX_DIR) all
endif

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
