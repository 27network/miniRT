/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 03:20:21 by kiroussa          #+#    #+#             */
/*   Updated: 2024/04/30 16:39:26 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CYLINDER_H
# define CYLINDER_H

# include <ft/math/vector.h>
# include <rt/renderer.h>

typedef struct s_rt_obj_cylinder
{
	double	diameter;
	double	height;
	t_vec3d	norm;
	bool	last_hit_on_edge;
}	t_rt_obj_cylinder;

t_rt_obj_cylinder	*rt_obj_cylinder_init(
						double diameter,
						double height,
						t_vec3d norm);

bool				rt_obj_cylinder_intersect(
						t_rt_ray ray,
						t_rt_object *obj,
						t_rt_hit *hit);

t_vec3d				rt_obj_cylinder_norm(
						t_rt_ray ray,
						t_rt_hit hit);

#endif // CYLINDER_H
