/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 03:20:07 by kiroussa          #+#    #+#             */
/*   Updated: 2024/04/18 19:50:04 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SPHERE_H
# define SPHERE_H

# include <ft/math/vector.h>

typedef struct s_rt_obj_sphere
{
	double	diameter;
}	t_rt_obj_sphere;

t_rt_obj_sphere	*rt_obj_sphere_init(double diameter);

#endif // SPHERE_H
