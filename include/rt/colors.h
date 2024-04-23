/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 18:37:09 by rgramati          #+#    #+#             */
/*   Updated: 2024/04/23 18:52:37 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLORS_H
# define COLORS_H

# include <stdint.h>

typedef struct s_color
{
    uint8_t a;
    uint8_t r;
    uint8_t g;
    uint8_t b;
}   t_color;

typedef struct s_color_norm
{
    double	a;
    double	r;
    double	g;
    double	b;
}   t_color_norm; 

t_color			rt_color(uint32_t argb);

uint32_t		rt_color_argb(t_color color);

t_color			rt_color_from_norm(t_color_norm color);

t_color_norm	rt_color_to_norm(t_color color);

#endif