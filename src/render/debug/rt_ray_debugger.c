/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_ray_debugger.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 14:08:19 by rgramati          #+#    #+#             */
/*   Updated: 2024/05/28 14:15:56 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft/print.h>
#include <rt/renderer.h>

int	mapx(t_vec3d cam, double x, int start, int end)
{
	return (((x + 10.0f - cam.x) / 20.0f) * (end - start) / 4.0f);
}

int	mapy(t_vec3d cam, double y, int start, int end)
{
	return (((16.0f - (y - cam.z)) / 16.0f) * (end - start) / 4.0f);
}

void	rt_ray_render(t_rt_renderer *renderer, t_rt_ray ray, t_rt_hit hit, t_color color)
{
	t_toc_vec2i	start;
	t_toc_vec2i end;

	start = (t_toc_vec2i){WIDTH / 8, HEIGHT / 4};
	if (ray.bounces == 1 && renderer->scene->rt_flags & RT_RAY_DEBUG && hit.hit && hit.obj)
	{
		end = (t_toc_vec2i){.x = mapx(renderer->scene->camera.pos, hit.pos.x, 0, WIDTH),
						.y = mapy(renderer->scene->camera.pos, hit.pos.z, 0, HEIGHT)};
		rt_trace_line(renderer, end, start, color);
	}
}