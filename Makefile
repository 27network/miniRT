# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/06 21:19:50 by kiroussa          #+#    #+#              #
#    Updated: 2024/06/03 19:02:09 by kiroussa         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#
# Colors
#

BLACK		=	\033[30m
RED			=	\033[31m
GREEN		=	\033[32m
YELLOW		=	\033[33m
BLUE		=	\033[34m
MAGENTA		=	\033[35m
CYAN		=	\033[36m
WHITE		=	\033[37m

BOLD		=	\033[1m
ITALIC		=	\033[3m

RESET		=	\033[0m
LINE_CLR	=	\33[2K\r

#
# Vars
#

NAME			=	miniRT	
DEBUG			?=	0

BUILD_DIR		=	build

SRC				=	error/rt_err.c						\
					error/rt_errd.c						\
					error/rt_error_print.c				\
					error/rt_ok.c						\
					parse/scene/rt_parse_line.c			\
					parse/scene/rt_parse_scene.c		\
					parse/rt_parse_error.c 				\
					parse/cub/rt_parse_cub.c			\
					parse/cub/rt_parse_cub_file.c		\
					parse/cub/rt_cub_converter.c		\
					render/color/rt_color.c				\
					render/color/rt_color_argb.c		\
					render/objects/rt_obj_box.c			\
					render/objects/rt_obj_camera.c		\
					render/objects/rt_obj_cone.c		\
					render/objects/rt_obj_quad.c		\
					render/objects/rt_obj_cylinder.c	\
					render/objects/rt_obj_light.c		\
					render/objects/rt_obj_plane.c		\
					render/objects/rt_obj_sphere.c		\
					render/rays/rt_rays.c				\
					render/rays/rt_ray_process.c		\
					render/rt_colors.c					\
					render/events/rt_hooks.c			\
					render/events/rt_cam_transform.c	\
					render/rt_render.c					\
					render/rt_render_utils.c			\
					render/rt_rendering.c				\
					scene/rt_objects_init.c				\
					scene/rt_scene_free.c				\
					scene/rt_scene_init.c				\
					scene/rt_scene_transform.c			\
					octree/rt_octree_utils.c			\
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
LIBFT			=	$(LIBFT_DIR)/build/output/libft.so

TOC_DIR			=	third-party/tocard-ui
TOC				=	$(TOC_DIR)/build/libtocard.so

CC				=	clang
CFLAGS			= 	-Wall -Wextra -Werror -g3 #-static-libsan -fsanitize=address -fno-omit-frame-pointer
ifeq ($(FUNMODE), 1)
	CFLAGS		+=	-O3
endif
COPTS			= 	-I $(INCLUDE_DIR) -I $(MLX_DIR)/$(INCLUDE_DIR)s -I $(LIBFT_DIR)/$(INCLUDE_DIR) -I $(TOC_DIR)/$(INCLUDE_DIR)
LDFLAGS			=	-lSDL2 -lm

ifeq ($(DEBUG), 1)
CFLAGS			+=	-gdwarf-4 -g3
LDFLAGS			+=	-rdynamic
endif

FILE			=	$(shell ls -lR $(SRC_DIR) | grep -F .c | wc -l)
CMP				=	1

MAKE_CMD		=	make

RM				=	rm -rf

VALGRIND		=	valgrind
VALGRIND_OPTS	=	--suppressions=$(MLX_DIR)/valgrind.supp --leak-check=full --show-leak-kinds=all --track-origins=yes -q 
VALGRIND_PROGRAM_ARGS	=	./scenes/valid/minimalist.rt

#
# Rules
#

all:			$(NAME)

$(NAME):		$(LIBFT) $(MLX) $(TOC) $(OBJ)
	@$(CC) $(CFLAGS) $(COPTS) -o $(NAME) $(OBJ) $(LIBFT) $(MLX) $(TOC) $(LDFLAGS)
	@printf "$(LINE_CLR)  ⭐$(BWHITE) $(NAME):\t PROJECT READY !$(RESET)\n\n"

bonus:			COPTS += -DMINIRT_BONUS
bonus:			remake

rt:				COPTS += -DRT_MODE
rt:				remake

$(NAME_BONUS):	$(LIBFT) $(MLX) $(TOC) $(OBJ_BONUS)
	@$(CC) $(CFLAGS) $(COPTS) -o $(NAME_BONUS) $(OBJ_BONUS) $(LIBFT) $(MLX) $(TOC) $(LDFLAGS)
	@printf "$(LINE_CLR)  ⭐$(BOLD)$(WHITE) $(NAME):\t BONUS READY !$(RESET)\n\n"

$(LIBFT):
	$(MAKE_CMD) -j$(shell nproc) -C $(LIBFT_DIR) CFLAGS="$(CFLAGS)" all DEBUG="$(DEBUG)"

$(MLX):
ifeq ($(DEBUG), 1)
	$(MAKE_CMD) -j$(shell nproc) -C $(MLX_DIR) DEBUG="true" all
else
	$(MAKE_CMD) -j$(shell nproc) -C $(MLX_DIR) all
endif

$(TOC):
	$(MAKE_CMD) -j$(shell nproc) -C $(TOC_DIR) all MLX_DIR=../MacroLibX

$(OBJ_DIR)/%.o:	$(SRC_DIR)/%.c
	@mkdir -p $(@D)
	if [ $(CMP) -eq '1' ]; then \
		printf "\n"; \
	fi;
	printf "$(LINE_CLR)$(WHITE)  🔄 $(NAME): $(CMP)/$(FILE)\t$(GREEN)compiling...$(RESET)\t$(BOLD)$(WHITE)$<$(RESET)"
	@$(CC) $(CFLAGS) $(COPTS) -c $< -o $@
	@$(eval CMP=$(shell echo $$(($(CMP)+1))))
	if [ $(CMP) -gt $(FILE) ]; then \
		printf "$(LINE_CLR)$(WHITE)  🔄 $(NAME): $$(($(CMP)-1))/$(FILE)\n$(LINE_CLR)$(BOLD)$(GREEN)  ✅ Compilation done !$(RESET)\n"; \
	fi \

remake:			oclean all

valgrind:
	$(VALGRIND) $(VALGRIND_OPTS) ./$(NAME) $(VALGRIND_PROGRAM_ARGS)

clean:			oclean
	@printf "$(BWHITE)  🚫 $(NAME):\t$(BRED) .o files deleted.$(RESET)\n"
	make -C $(LIBFT_DIR) clean
	make -C $(MLX_DIR) clean
	make -C $(TOC_DIR) clean

oclean:
	@printf "$(BWHITE)  🚫 $(NAME):\t$(BRED) objs dir deleted.$(RESET)\n"
	$(RM) $(OBJ_DIR) 

fclean:			clean
	@printf "$(BWHITE)  🚫 $(NAME):\t$(BRED) binary deleted.$(RESET)\n"
	make -C $(LIBFT_DIR) fclean
	make -C $(MLX_DIR) fclean
	make -C $(TOC_DIR) fclean
	$(RM) $(NAME) $(NAME_BONUS)

re:				fclean all

.PHONY:			all bonus clean oclean fclean remake re
.SILENT:
