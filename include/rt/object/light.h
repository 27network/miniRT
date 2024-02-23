/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 03:20:58 by kiroussa          #+#    #+#             */
/*   Updated: 2024/02/23 03:44:54 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIGHT_H
# define LIGHT_H

# include <rt/scene/parser.h>

typedef struct s_rt_light_params
{
	float	brightness;
	t_vec3i	color;
}	t_rt_light_params;

#endif // LIGHT_H
