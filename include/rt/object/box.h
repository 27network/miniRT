/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   box.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 19:15:48 by rgramati          #+#    #+#             */
/*   Updated: 2024/04/18 19:49:10 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOX_H
# define BOX_H

# include <ft/math/vector.h>

typedef struct s_rt_obj_box
{
	t_vec3d	size;
}	t_rt_obj_box;

t_rt_obj_box	*rt_obj_box_init(double sx, double sy, double sz);

#endif // BOX_H