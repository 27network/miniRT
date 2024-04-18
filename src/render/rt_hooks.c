/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_hooks.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 11:10:38 by rgramati          #+#    #+#             */
/*   Updated: 2024/04/18 18:14:01 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft/print.h>
#include <rt/renderer.h>

int	rt_render_update(void *render)
{
	t_rt_renderer	*renderer;

	renderer = (t_rt_renderer *) render;
	mlx_clear_window(renderer->mlx->rt_mlx, renderer->mlx->rt_win);
	rt_do_rendering(renderer);
	return (0);
}

int	rt_window_event(int key, void *mlx)
{
	t_rt_mlx_data	*mlx_data;

	mlx_data = (t_rt_mlx_data *) mlx;
	if (!key)
	{
		mlx_loop_end(mlx_data->rt_mlx);
		return (0);
	}
	return (0);
}

int	rt_keydown_event(int key, void *render)
{
	t_rt_renderer	*renderer;

	renderer = (t_rt_renderer *) render;
	ft_printf("key = %d, %d\n", key, *renderer->status);
	if (*renderer->status == RT_RS_NONE)
		*renderer->status = RT_RS_HOME;
	if (*renderer->status == RT_RS_HOME && key == 40)
	{
		mlx_clear_window(renderer->mlx->rt_mlx, renderer->mlx->rt_win);
		*renderer->status = RT_RS_SCENE;
	}
	if (key == 41)
	{
		mlx_loop_end(renderer->mlx->rt_mlx);
		return (0);
	}
	return (0);
}
