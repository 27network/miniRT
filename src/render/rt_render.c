/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_render.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 07:51:17 by kiroussa          #+#    #+#             */
/*   Updated: 2024/05/31 20:36:07 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"
#include <errno.h>
#include <string.h>
#include <ft/print.h>
#include <ft/mem.h>
#include <rt/error.h>
#include <rt/renderer.h>

void	rt_start_rendering(t_rt_mlx_data *m, t_rt_renderer *r)
{
	mlx_on_event(m->rt_mlx, m->rt_win,
		MLX_WINDOW_EVENT, rt_window_event, r);
	mlx_on_event(m->rt_mlx, m->rt_win,
		MLX_KEYDOWN, rt_keydown_event, r);
	mlx_on_event(m->rt_mlx, m->rt_win,
		MLX_KEYUP, rt_keyup_event, r);
	mlx_on_event(m->rt_mlx, m->rt_win,
		MLX_MOUSEDOWN, rt_mousedown_event, r);
	mlx_on_event(m->rt_mlx, m->rt_win,
		MLX_MOUSEUP, rt_mouseup_event, r);
	mlx_loop_hook(m->rt_mlx, rt_render_update, r);
	mlx_loop(m->rt_mlx);
	mlx_destroy_image(m->rt_mlx, m->rt_imgs[0]);
	mlx_destroy_image(m->rt_mlx, m->rt_imgs[1]);
	mlx_destroy_window(m->rt_mlx, m->rt_win);
	mlx_destroy_display(m->rt_mlx);
	free(r->calc);
	free(r->image);
}

void	rt_do_rendering(t_rt_renderer *renderer)
{
	mlx_clear_window(renderer->mlx->rt_mlx, renderer->mlx->rt_win);
	if (renderer->status == RT_RS_HOME)
		rt_render_home(renderer);
	else if (renderer->status == RT_RS_SCENE)
		rt_render_scene(renderer);
}

t_rt_error	rt_mlx_init(t_rt_scene *s, t_rt_mlx_data *m)
{
	m->rt_mlx = mlx_init();
	if (!m->rt_mlx)
		return (rt_errd(RT_ERROR_MLX, "MLX internal application failed."));
	m->rt_win = mlx_new_window(m->rt_mlx,
			s->width, s->height, "(mini)rt");
	if (!m->rt_win)
	{
		mlx_destroy_display(m->rt_mlx);
		return (rt_errd(RT_ERROR_MLX, "MLX window failed."));
	}
	m->rt_imgs[0] = mlx_new_image(m->rt_mlx, s->width, s->height);
	m->rt_imgs[1] = mlx_new_image(m->rt_mlx, s->width, s->height);
	for (int i = 0; i < s->width; i++) {
		for (int j = 0; j < s->height; j++) {
			mlx_set_image_pixel(m->rt_mlx, m->rt_imgs[0], i, j, 0);
		}
	}
	if (!m->rt_imgs[0] || !m->rt_imgs[1])
	{
		mlx_destroy_window(m->rt_mlx, m->rt_win);
		mlx_destroy_display(m->rt_mlx);
		return (rt_errd(RT_ERROR_MLX, "MLX image allocation failed."));
	}
	return (rt_ok());
}

t_rt_error	rt_renderer_init(t_rt_scene *s, t_rt_mlx_data *m, t_rt_renderer *r)
{
	ft_memset(r->input_map, 0, sizeof(r->input_map));
	r->scene = s;
	r->mlx = m;
	r->status = RT_RS_HOME;
	r->rendered = 0;
	r->image = ft_calloc(s->width * s->height , sizeof(t_color_norm));
	if (!r->image)
		return (rt_errd(RT_ERROR_ALLOC, strerror(errno)));
	r->calc = ft_calloc(s->width * s->height , sizeof(t_color_norm));
	if (!r->calc)
		return (rt_errd(RT_ERROR_ALLOC, strerror(errno)));
	ft_printf("%d %d\n", s->height, s->width);
	return (rt_ok());
}

t_rt_error	rt_render(t_rt_scene *scene, t_toc_vec2i size)
{
	t_rt_mlx_data	mlx_data;
	t_rt_renderer	renderer;
	t_rt_error		err;

	scene->width = size.x;
	scene->height = size.y;
	err = rt_scene_example(scene);
	if (err.type == RT_OK)
		err = rt_renderer_init(scene, &mlx_data, &renderer);
	if (err.type == RT_OK)
		err = rt_mlx_init(scene, &mlx_data);
	if (err.type == RT_OK)
		rt_start_rendering(&mlx_data, &renderer);
	return (err);
}
