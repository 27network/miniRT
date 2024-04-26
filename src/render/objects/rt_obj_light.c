/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_obj_light.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 14:49:44 by rgramati          #+#    #+#             */
/*   Updated: 2024/04/25 16:32:26 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <ft/print.h>
#include <ft/math.h>
#include <rt/object/light.h>

bool	rt_obj_light_intersect(t_rt_ray ray, t_rt_object *obj, t_rt_hit *hit)
{
	(void) ray;
	(void) obj;
	(void) hit;
	return (false);
	// t_vec3d			c_to_r;
	// double			params[3];
	// double			delta;
	// double			t;

	// params[0] = ft_vec3d_dot(ray.direction, ray.direction);
	// c_to_r = ft_vec3d_sub(ray.origin, obj->position);
	// params[1] = 2 * ft_vec3d_dot(c_to_r, ray.direction);
	// params[2] = ft_vec3d_dot(c_to_r, c_to_r) - ft_fpow(0.0, 2);
	// delta = ft_fpow(params[1], 2) - (4 * params[0] * params[2]);
	// if (delta < 0 || fabs(params[0]) < EPSILON)
	// 	return (false);
	// hit->hit_object = obj;
	// if (fabs(delta) < EPSILON)
	// {
	// 	hit->position = ft_vec3d_add(ray.origin,
	// 			ft_vec3d_mult(ray.direction, -params[2] / (2 * params[0])));
	// 	return (true);
	// }
	// t = ((-params[1] + (sqrt(delta))) / (2 * params[0]));
	// if (t > ((-params[1] - (sqrt(delta))) / (2 * params[0])))
	// 	t = ((-params[1] - (sqrt(delta))) / (2 * params[0]));
	// hit->position = ft_vec3d_add(ray.origin, ft_vec3d_mult(ray.direction, t));
	// return (true);
}
