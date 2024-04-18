/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_rendering.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 14:43:52 by rgramati          #+#    #+#             */
/*   Updated: 2024/04/18 19:52:44 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <rt/renderer.h>
#include <stdio.h>

void	rt_render_home(t_rt_renderer *renderer)
{
	t_rt_scene		*scene;
	t_rt_mlx_data	*mlx;

	scene = renderer->scene;
	mlx = renderer->mlx;
	mlx_set_font_scale(mlx->rt_mlx, mlx->rt_win, RENDER_FONT, 150.0f);
	mlx_string_put(mlx->rt_mlx, mlx->rt_win,
		mlx->rt_width / 2 - 330, 200, 0xff80b0ff, "MINI RT");
}

void	rt_render_scene(__attribute_maybe_unused__ t_rt_renderer *renderer)
{
	return ;
}

void	rt_render_editor(__attribute_maybe_unused__ t_rt_renderer *renderer)
{
	return ;
}
