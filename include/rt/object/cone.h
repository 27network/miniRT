/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 03:20:21 by kiroussa          #+#    #+#             */
/*   Updated: 2024/04/26 16:32:30 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONE_H
# define CONE_H

# include <ft/math/vector.h>
# include <rt/renderer.h>

typedef struct s_rt_obj_cone
{
	double	diameter;
	double	height;
	t_vec3d	norm;
	double  theta;
	bool	last_hit_on_edge;
}	t_rt_obj_cone;

t_rt_obj_cone       *rt_obj_cone_init(
						double diameter,
						double height,
						t_vec3d norm,
						double theta);

bool				rt_obj_cone_intersect(
						t_rt_ray ray,
						t_rt_object *obj,
						t_rt_hit *hit);

t_vec3d				rt_obj_cone_norm(
						t_rt_ray ray,
						t_rt_hit hit);

#endif // CONE_H
