/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_ray_debugger.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 14:08:19 by rgramati          #+#    #+#             */
/*   Updated: 2024/04/27 14:07:01 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft/print.h>
#include <rt/renderer.h>

int	mapx(t_vec3d cam, double x, int start, int end)
{
	return (((x + 7.1f - cam.x) / 14.2f) * (end - start) / 4.0f);
}

int	mapy(t_vec3d cam, double y, int start, int end)
{
	return (((8.0f - (y - cam.z)) / 8.0f) * (end - start) / 4.0f);
}

void	*rt_getmlx(int what, t_rt_mlx_data *data)
{
	static t_rt_mlx_data	*mlx;

	if (data || !mlx)
		mlx = data;
	if (what == 0b0001)
		return mlx->rt_mlx;
	else if (what == 0b0010)
		return mlx->rt_win;
	else if (what == 0b0100)
		return mlx->rt_imgs[0];
	else if (what == 0b1000)
		return mlx->rt_imgs[1];
	return ((void *) mlx);
}

void	rt_trace_line(t_rt_scene *scene, t_vec2i start, t_vec2i end, t_color color)
{
	t_vec2i	deltas;
	t_vec3d	inc;
	t_vec3d	pos;
	double	steps;
	double	i;

	deltas = (t_vec2i){end.x - start.x, end.y - start.y};
	if (deltas.x > deltas.y)
		steps = deltas.x;
	else
		steps = deltas.y;
	inc.x = (double) deltas.x / steps;
	inc.y = (double) deltas.y / steps;
	i = 0.0f;
	pos = (t_vec3d){start.x, start.y, 0.0f};
	while (i < steps)
	{
		if (pos.x >= 0 && pos.x < scene->width && pos.y >= 0 && pos.y < scene->height && color.a)
		{
			if (pos.x > 0 && pos.x < scene->width - 1 && pos.y > 0 && pos.y < scene->height - 1)
					mlx_set_image_pixel(rt_getmlx(0b0001, NULL), rt_getmlx(0b1000, NULL), start.x, start.y, 0xFFFFFFFF);
			mlx_set_image_pixel(rt_getmlx(0b0001, NULL), rt_getmlx(0b1000, NULL), pos.x, pos.y, color.argb);
		}
		pos.x += inc.x;
		pos.y += inc.y;
		i += 1.0f;
	}
}

void	rt_render_ray(t_rt_scene *scene, t_rt_ray ray, t_rt_hit hit, t_color color)
{
	t_vec2i	start;
	t_vec2i end;

	start = (t_vec2i){scene->width / 8, scene->height / 4};
	if (hit.hit)
		end = (t_vec2i){.x = mapx(scene->camera.position, hit.position.x, 0, scene->width),
						.y = mapy(scene->camera.position, hit.position.z, 0, scene->height)};
	else
	{
		end = (t_vec2i){.x = mapx(scene->camera.position, (8.0f / ray.direction.x) * ray.direction.y, 0, scene->width - 1),
						.y = 0};
		color = rt_color(0xFF000000);
	}
	rt_trace_line(scene, end, start, color);
}