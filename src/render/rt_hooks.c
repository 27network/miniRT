/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_hooks.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 11:10:38 by rgramati          #+#    #+#             */
/*   Updated: 2024/04/25 18:20:17 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft/print.h>
#include <rt/renderer.h>
#include <SDL2/SDL_scancode.h>

int	rt_render_update(void *render)
{
	t_rt_renderer	*renderer;

	renderer = (t_rt_renderer *)render;
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

void	rt_clear_image(void *mlx, void *img, t_rt_scene *scene)
{
	int	x;
	int	y;

	x = 0;
	while (x < scene->width)
	{
		y = 0;
		while (y < scene->height)
			mlx_set_image_pixel(mlx, img, x, y++, 0xFF303030);
		x++;
	}
}

int	rt_keydown_event(int key, void *render)
{
	t_rt_renderer	*renderer;

	renderer = (t_rt_renderer *)render;
	rt_clear_image(renderer->mlx->rt_mlx, renderer->mlx->rt_imgs[1], renderer->scene);
	if (renderer->status == RT_RS_NONE)
		renderer->status = RT_RS_HOME;
	if (renderer->status == RT_RS_HOME && key == 40)
	{
		mlx_clear_window(renderer->mlx->rt_mlx, renderer->mlx->rt_win);
		renderer->status = RT_RS_SCENE;
	}
	if (key == 41)
	{
		mlx_loop_end(renderer->mlx->rt_mlx);
		return (0);
	}

	//pixels !
	if (key == SDL_SCANCODE_PAGEUP)
		renderer->scene->pratio += (renderer->scene->pratio < 32);
	if (key == SDL_SCANCODE_PAGEDOWN)
		renderer->scene->pratio -= (renderer->scene->pratio != 1);

	//move
	if (key == SDL_SCANCODE_D)
		rt_scene_translate(renderer->scene, (t_vec3d){-0.1f, 0.0f, 0.0f});
	if (key == SDL_SCANCODE_A)
		rt_scene_translate(renderer->scene, (t_vec3d){0.1f, 0.0f, 0.0f});
	if (key == SDL_SCANCODE_W)
		rt_scene_translate(renderer->scene, (t_vec3d){0.0f, -0.1f, 0.0f});
	if (key == SDL_SCANCODE_S)
		rt_scene_translate(renderer->scene, (t_vec3d){0.0f, 0.1f, 0.0f});
	if (key == SDL_SCANCODE_X)
		rt_scene_translate(renderer->scene, (t_vec3d){0.0f, 0.0f, -0.1f});
	if (key == SDL_SCANCODE_C)
		rt_scene_translate(renderer->scene, (t_vec3d){0.0f, 0.0f, 0.1f});

	//rotate cam TODO

	//debug rays
	if (key == SDL_SCANCODE_R)
	{
		renderer->scene->rt_flags ^= RT_RAY_DEBUG;
	}
	if (key == SDL_SCANCODE_G)
		renderer->scene->rt_flags ^= RT_COL_GAMMA;
	return (0);
}

bool g_debug = false;

t_color	rt_get_random_color(int toclose);

int rt_mousedown_event(int key, void *render)
{
	t_rt_renderer	*renderer;
	t_rt_ray		dray;
	t_rt_hit		dhit;
	int				params[2];

	renderer = (t_rt_renderer *)render;
	mlx_mouse_get_pos(renderer->mlx->rt_mlx, params, params + 1);
	if (key == 1)
	{
		rt_ray_init(renderer->scene, &dray, (t_vec2i){.x = *params, .y = *(params + 1)});
		for (int i = 0; i < 211; i++)
			ft_printf("-");
		ft_printf("\nDEBUG RAY:\n");
		for (int i = 0; i < 211; i++)
			ft_printf("-");
		g_debug = true;
		rt_ray_cast_debug(renderer->scene, &dray, &dhit);
		for (int i = 0; i < 211; i++)
			ft_printf("-");
		ft_printf("\nSHADOW RAY:\n");
		for (int i = 0; i < 211; i++)
			ft_printf("-");
		rt_color_occlusion(renderer->scene, dhit, ft_vec3d_sub(((t_rt_object) renderer->scene->lights[0]).position, dhit.position));
		g_debug = false;
		dhit.hit_object->color = rt_get_random_color(0);
	}
	return (0);
}
