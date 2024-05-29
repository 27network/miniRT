/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 02:33:41 by kiroussa          #+#    #+#             */
/*   Updated: 2024/05/25 15:36:08 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCENE_H
# define SCENE_H

# ifdef WIDTH
#  undef WIDTH
# endif
# ifndef WIDTH
#  define WIDTH 1366
# endif

# ifdef HEIGHT
#  undef HEIGHT
# endif
# ifndef HEIGHT
#  define HEIGHT 768
# endif

# include <ft/math/vector.h>
# include <rt/error.h>
# include <stddef.h>
# include <rt/color.h>
# include <tocard/ui.h>

typedef struct s_rt_ray		t_rt_ray;
typedef struct s_rt_object	t_rt_object;
typedef struct s_rt_hit		t_rt_hit;

typedef enum e_rt_flag
{
	RT_RAY_DEBUG = 0b0001,
	RT_COL_GAMMA = 0b0010,
	RT_NO_RENDER = 0b0100,
	RT_SEQ_RENDER = 0b1000,
}	t_rt_flag;

typedef enum e_rt_object_type
{
	RT_OBJ_BOX,
	RT_OBJ_SPHERE,
	RT_OBJ_CYLINDER,
	RT_OBJ_PLANE,
	RT_OBJ_QUAD,
	RT_OBJ_CONE,
	RT_OBJ_TRIANGLE,
	RT_OBJ_LIGHT,
	RT_OBJ_CAMERA,
	RT_OBJ_CUBE
}	t_rt_object_type;

typedef bool				t_intersect_fn(t_rt_ray ray, t_rt_object *obj,
								t_rt_hit *hit);
typedef t_vec3d				t_hit_norm_fn(t_rt_ray ray, t_rt_hit hit);

typedef struct s_rt_mat
{
	t_color_norm	obj_color;
	t_color_norm	emi_color;
	double			emi_strength;
	t_color_norm	spe_color;
	double			spe_prob;
	double			smoothness;
}	t_rt_mat;

typedef struct s_rt_skybox
{
	t_vec3d	usky;
	t_vec3d dsky;
	t_vec3d gcol;
}	t_rt_skybox;

typedef struct s_rt_object
{
	t_rt_object_type	type;
	t_vec3d				pos;
	t_vec3d				rotation;
	t_rt_mat			mat;
	t_intersect_fn		*intersect;
	t_hit_norm_fn		*norm;
	void				*options;
}	t_rt_object;

typedef struct s_rt_scene
{
	t_rt_object	*objects;
	t_rt_object	*lights;
	uint8_t		rt_flags;
	int			pratio;
	int			width;
	int			height;
	size_t		objects_size;
	size_t		lights_size;
	t_rt_object	camera;
	t_rt_object	ambient;
	t_rt_skybox	skybox;
}	t_rt_scene;

t_rt_object	*rt_object_init(t_rt_object *obj, t_rt_object_type type);

t_rt_error	rt_scene_init(t_rt_scene *scene);

t_rt_error	rt_scene_guard(t_rt_scene *scene);

void		rt_scene_free(t_rt_scene *scene);

t_rt_error	rt_scene_example(t_rt_scene *scene);

t_color		rt_obj_color(t_rt_scene *scene, t_rt_hit hit, t_rt_ray ray, t_vec3d norm);

/* TRANSFORM **************************************************************** */

void		rt_scene_translate(t_rt_scene *scene, t_vec3d move);

void		rt_obj_set_pos(t_rt_object *obj, double x, double y, double z);

void		rt_obj_set_rot(t_rt_object *obj, double x, double y, double z);

/* DEBUGGER ***************************************************************** */

void		rt_swap(double *a, double *b);

float	ft_random_value(long long *state);

void	rt_color_ambient(t_rt_scene *scene, t_color_norm *c);

#endif // SCENE_H
