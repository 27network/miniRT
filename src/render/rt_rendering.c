/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_rendering.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 14:43:52 by rgramati          #+#    #+#             */
/*   Updated: 2024/04/30 14:39:49 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <ft/math.h>
#include <rt/renderer.h>
#include <rt/object/light.h>
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
	static t_color	pixels[1920];
	t_color			color;
	t_rt_ray		ray;
	t_rt_hit		hit;

	if (!(x % scene->pratio) && !(y % scene->pratio))
	{
		rt_ray_init(scene, &ray, (t_vec2i){.x = x, .y = y});
		hit = (t_rt_hit) {(t_vec3d) {0.0f, 0.0f, 0.0f}, NULL, false, INFINITY};
		rt_ray_cast(scene, &ray, &hit);
		color = rt_color(0xFF000000); // TO REMOVE -> BACKGROUND COLOR IF NO HIT
		if (hit.hit)
			color = rt_obj_color(scene, hit, ray, hit.hit_object->norm(ray, hit));
		pixels[y / scene->pratio] = color; 
		if (scene->rt_flags & RT_RAY_DEBUG && hit.hit_object && hit.hit_object->type != RT_OBJ_PLANE)
			rt_render_ray(scene, ray, hit, color);
		return (color);
	}
	else
		return (pixels[y / scene->pratio]);
}	

void	rt_render_scene(t_rt_renderer *renderer)
{
	t_rt_mlx_data	mlx;
	t_color			pixel;
	t_vec2i			coords;
	unsigned int	image[2073600];
// 
	mlx = *renderer->mlx;
	coords = (t_vec2i) {-1, -1};
	while (++coords.x < renderer->scene->width)
	{
		coords.y = -1;
		while (++coords.y < renderer->scene->height)
		{
			pixel = rt_get_color(renderer->scene, coords.x, coords.y);
			image[coords.x * renderer->scene->width + coords.y] = rt_color_argb(pixel);
		}
	}
	coords = ft_vec2i(-1, -1);
	while (++coords.x < renderer->scene->width)
	{
		coords.y = -1;
		while (++coords.y < renderer->scene->height)
			mlx_set_image_pixel(mlx.rt_mlx, mlx.rt_imgs[0], coords.x, coords.y, image[coords.x * renderer->scene->width + coords.y]);
	}
	mlx_put_image_to_window(mlx.rt_mlx, mlx.rt_win, mlx.rt_imgs[0], 0, 0);
	if ((renderer->scene->rt_flags & RT_RAY_DEBUG))
	{
		rt_trace_line(renderer->scene, (t_vec2i){0, 0}, (t_vec2i){renderer->scene->width / 4, 0}, rt_color(0xFFFFFFFF));
		rt_trace_line(renderer->scene, (t_vec2i){renderer->scene->width / 4, 0}, (t_vec2i){renderer->scene->width / 4, renderer->scene->height / 4}, rt_color(0xFFFFFFFF));
		rt_trace_line(renderer->scene, (t_vec2i){0, renderer->scene->height / 4}, (t_vec2i){renderer->scene->width / 4, renderer->scene->height / 4}, rt_color(0xFFFFFFFF));
		rt_trace_line(renderer->scene, (t_vec2i){0, 0}, (t_vec2i){0, renderer->scene->height / 4}, rt_color(0xFFFFFFFF));
		rt_trace_line(renderer->scene, (t_vec2i){0, renderer->scene->height / 4 - renderer->scene->width / 8}, (t_vec2i){renderer->scene->width / 8, renderer->scene->height / 4}, rt_color(0xFFFFFFFF));
		rt_trace_line(renderer->scene, (t_vec2i){renderer->scene->width / 8, renderer->scene->height / 4}, (t_vec2i){renderer->scene->width / 4, renderer->scene->height / 4 - renderer->scene->width / 8}, rt_color(0xFFFFFFFF));
		mlx_put_image_to_window(mlx.rt_mlx, mlx.rt_win, mlx.rt_imgs[1], renderer->scene->width - (renderer->scene->width / 4) - 1, 0);
	}
}

void	rt_render_editor(__attribute_maybe_unused__ t_rt_renderer *renderer)
{
	return ;
}
