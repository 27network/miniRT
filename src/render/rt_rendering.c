/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_rendering.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 14:43:52 by rgramati          #+#    #+#             */
/*   Updated: 2024/04/21 20:06:18 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <rt/renderer.h>
#include <ft/print.h>

void	rt_render_home(t_rt_renderer *renderer)
{
	t_rt_scene		*scene;
	t_rt_mlx_data	*mlx;

	scene = renderer->scene;
	mlx = renderer->mlx;
	mlx_set_font_scale(mlx->rt_mlx, mlx->rt_win, RENDER_FONT, 150.0f);
	mlx_string_put(mlx->rt_mlx, mlx->rt_win,
		scene->width / 2 - 330, 200, 0xff80b0ff, "MINI RT");
}

t_color	rt_get_color(t_rt_scene *scene, int x, int y)
{
	t_color			color;
	t_rt_ray		ray;
	static t_color	pixels[1920];

	// if (!x && !y)
	// 	ft_printf("LES RAYOOOOONS\n");
	if (!(x % scene->pratio) && !(y % scene->pratio))
	{
		rt_init_ray(scene, &ray, x, y);
		color = rt_get_ray(scene, ray);
		pixels[y / scene->pratio] = color; 
		return (color);
	}
	else
		return (pixels[y / scene->pratio]);
}	

void	rt_render_scene(t_rt_renderer *renderer)
{
	t_rt_mlx_data	mlx;
	t_color			pixel;
	int				x;
	int				y;

	mlx = *renderer->mlx;
	x = 0;
	while (x < renderer->scene->width)
	{
		y = 0;
		while (y < renderer->scene->height)
		{
			pixel = rt_get_color(renderer->scene, x, y);
			mlx_set_image_pixel(mlx.rt_mlx, mlx.rt_imgs[0], x, y, pixel.argb);
			y++;
		}
		x++;
	}
	if (!(renderer->scene->rt_flags & RT_RAY_DEBUG))
		mlx_put_image_to_window(mlx.rt_mlx, mlx.rt_win, mlx.rt_imgs[0], 0, 0);
	else
		mlx_put_image_to_window(mlx.rt_mlx, mlx.rt_win, mlx.rt_imgs[1], 0, 0);
	return ;
}

void	rt_render_editor(__attribute_maybe_unused__ t_rt_renderer *renderer)
{
	return ;
}
