/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 03:20:58 by kiroussa          #+#    #+#             */
/*   Updated: 2024/04/21 17:42:59 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIGHT_H
# define LIGHT_H

# include <ft/math/vector.h>
# include <rt/renderer.h>

typedef struct s_rt_obj_light
{
	float	brightness;
}	t_rt_obj_light;

t_rt_obj_light	*rt_obj_light_init(double brightness);

bool	rt_obj_light_intersect(t_rt_ray ray, t_rt_object *obj, t_rt_hit *hit);

#endif // LIGHT_H
