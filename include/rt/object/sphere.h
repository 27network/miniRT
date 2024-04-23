/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 03:20:07 by kiroussa          #+#    #+#             */
/*   Updated: 2024/04/22 19:20:54 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SPHERE_H
# define SPHERE_H

# include <ft/math/vector.h>
# include <rt/renderer.h>

typedef struct s_rt_obj_sphere
{
	double	diameter;
}	t_rt_obj_sphere;

t_rt_obj_sphere	*rt_obj_sphere_init(double diameter);

bool	rt_obj_sphere_intersect(t_rt_ray ray, t_rt_object *obj, t_rt_hit *hit);

t_vec3d	rt_obj_sphere_norm(t_rt_ray ray, t_rt_hit hit);

#endif // SPHERE_H
