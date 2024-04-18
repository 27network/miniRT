/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 03:20:58 by kiroussa          #+#    #+#             */
/*   Updated: 2024/04/18 19:48:56 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIGHT_H
# define LIGHT_H

typedef struct s_rt_obj_light
{
	float	brightness;
}	t_rt_obj_light;

t_rt_obj_light	*rt_obj_light_init(double brightness);

#endif // LIGHT_H
