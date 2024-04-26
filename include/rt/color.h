/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 18:37:09 by rgramati          #+#    #+#             */
/*   Updated: 2024/04/26 16:57:28 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLOR_H
# define COLOR_H
# undef COLOR_H
# ifndef __RT_COLOR_H__
#  define __RT_COLOR_H__

#  include <stdint.h>

typedef union u_color
{
	uint32_t	argb;
	struct
	{
		uint8_t	b;
		uint8_t	g;
		uint8_t	r;
		uint8_t	a;
	};
}	t_color;

typedef struct s_color_norm
{
	double	a;
	double	r;
	double	g;
	double	b;
}	t_color_norm;

t_color			rt_color(uint32_t argb);
uint32_t		rt_color_argb(t_color color);

t_color			rt_color_from_norm(t_color_norm color);
t_color_norm	rt_color_to_norm(t_color color);

# endif // __RT_COLOR_H__
#endif // COLOR_H
