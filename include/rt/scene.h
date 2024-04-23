/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 02:33:41 by kiroussa          #+#    #+#             */
/*   Updated: 2024/04/23 21:57:29 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCENE_H
# define SCENE_H

# include <ft/math/vector.h>
# include <rt/error.h>
# include <stddef.h>
# include <rt/colors.h>

typedef struct s_rt_ray t_rt_ray;

typedef struct s_rt_object t_rt_object;

typedef struct s_rt_hit t_rt_hit;

typedef enum e_rt_flag
{
	RT_RAY_DEBUG = 0x001,
	RT_COL_GAMMA = 0x010,
}	t_rt_flag;

typedef enum e_rt_object_type
{
	RT_OBJ_BOX,
	RT_OBJ_SPHERE,
	RT_OBJ_CYLINDER,
	RT_OBJ_PLANE,
	RT_OBJ_TRIANGLE,
	RT_OBJ_CONE,
	RT_OBJ_LIGHT,
	RT_OBJ_CAMERA
}	t_rt_object_type;

typedef bool	t_intersect_fn(t_rt_ray ray, t_rt_object *obj, t_rt_hit *hit);

typedef t_vec3d	t_hit_norm_fn(t_rt_ray ray, t_rt_hit hit);

typedef struct s_rt_object
{
	t_rt_object_type	type;
	t_vec3d				position;
	t_vec3d				rotation;
	t_color				color;
	t_intersect_fn		*intersect;
	t_hit_norm_fn		*norm;
	void				*options;
}	t_rt_object;

typedef struct s_rt_scene
{
	int			width;
	int			height;
	t_rt_object	camera;
	t_rt_object	ambient;
	t_rt_object	*objects;
	t_rt_object	*lights;
	size_t		objects_size;
	size_t		lights_size;
	uint8_t		rt_flags;
	int			pratio;
}	t_rt_scene;

t_rt_object	*rt_object_init(t_rt_object *obj, t_rt_object_type type);

t_rt_error	rt_scene_init(t_rt_scene *scene);

t_rt_error	rt_scene_guard(t_rt_scene *scene);

void		rt_scene_free(t_rt_scene *scene);

t_rt_error	rt_scene_example(t_rt_scene *scene);

t_color		rt_obj_color(t_rt_scene *scene, t_rt_hit hit, t_vec3d norm);

t_color		rt_color(uint32_t argb);

uint32_t	rt_color_argb(t_color color);

/* TRANSFORM **************************************************************** */

void		rt_scene_translate(t_rt_scene *scene, t_vec3d move);

void		rt_obj_set_pos(t_rt_object *obj, double x, double y, double z);

void		rt_obj_set_rot(t_rt_object *obj, double x, double y, double z);

/* DEBUGGER ***************************************************************** */

void		rt_render_ray(t_rt_scene *scene, t_rt_ray ray, t_rt_hit hit, t_color color);

#endif // SCENE_H
