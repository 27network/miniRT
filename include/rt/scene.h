/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 02:33:41 by kiroussa          #+#    #+#             */
/*   Updated: 2024/04/20 23:12:20 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCENE_H
# define SCENE_H

# include <ft/math/vector.h>
# include <rt/error.h>
# include <stddef.h>
# include <stdint.h>

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

typedef union u_color
{
    struct {
        uint8_t a;
        uint8_t r;
        uint8_t g;
        uint8_t b;
    };
    uint32_t argb;
    uint8_t array[4];
}   t_color;

typedef struct s_rt_object
{
	t_rt_object_type	type;
	t_vec3d				position;
	t_vec3d				rotation;
	t_color				color;
	void				*options;
}	t_rt_object;

typedef struct s_rt_scene
{
	int			width;
	int			height;
	t_rt_object	camera;
	t_rt_object	*objects;
	t_rt_object	*lights;
	size_t		objects_size;
	size_t		lights_size;
}	t_rt_scene;

t_rt_object	*rt_object_init(t_rt_object *obj, t_rt_object_type type);

t_rt_error	rt_scene_init(t_rt_scene *scene);

void		rt_scene_free(t_rt_scene *scene);

t_rt_error	rt_empty_scene(t_rt_scene *scene);

void		rt_obj_move(t_rt_object *obj, t_vec3d move);

void		rt_obj_set_pos(t_rt_object *obj, double x, double y, double z);

void		rt_obj_set_rot(t_rt_object *obj, double x, double y, double z);

#endif // SCENE_H
