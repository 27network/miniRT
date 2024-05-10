/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_rendering.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 14:43:52 by rgramati          #+#    #+#             */
/*   Updated: 2024/05/10 15:42:42 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <ft/math.h>
#include <ft/mem.h>
#include <ft/string/parse.h>
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

static t_color_norm	rt_get_environment(t_rt_renderer *renderer, t_rt_ray *ray)
{
	const t_vec3d	upsky_color = ft_vec3d(1.0f, 1.0f, 1.0f);
	const t_vec3d	downsky_color = ft_vec3d(0x87 / 255., 0xCE / 255., 0xEB / 255.);
	const t_vec3d	ground_color = ft_vec3d(0x8C / 255., 0x83 / 255., 0x91 / 255.);

	double 	ground_to_sky;
	t_vec3d	sky;
	double	sun;
	double SUN_FOCUS = 0.5;
	double SUN_INTENSITY = 1.;
	t_vec3d	composite;

	ground_to_sky = ft_smoothstep(-.01, 0, ray->direction.y);
	sky = ft_vec3d_lerp(upsky_color, downsky_color, pow(ft_smoothstep(0, 0.4, ray->direction.y), 0.35));
	sun = ft_fpow(ft_fmax(0, ft_vec3d_dot(ray->direction, renderer->scene->ambient.position)), SUN_FOCUS) * SUN_INTENSITY;
	composite = ft_vec3d_add(ft_vec3d_lerp(ground_color, sky, ground_to_sky), ft_vec3d_mult(ft_vec3d(sun, sun, sun), (ground_to_sky >= 1)));
	return ((t_color_norm){.a = 1., .r = composite.x, .g = composite.y, .b = composite.z});
}

static void	rt_ray_colorize(t_rt_ray *ray, t_rt_hit *hit, t_color_norm *light)
{
	const t_rt_material		hit_mat = hit->hit_object->material;
	const t_color_norm		emittedlight = rt_color_fact(hit_mat.emi_color, hit_mat.emi_strength);
	const t_color_norm		raylight = rt_color_mult(ray->color, emittedlight);

	*light = rt_color_add(*light, raylight);
	ray->color = rt_color_mult(ray->color, hit_mat.obj_color);
}

static void	rt_ray_update(t_rt_renderer *renderer, t_rt_ray *ray, t_rt_hit *hit)
{
	static long long	rngState = 0;
	const size_t		frame = renderer->rendered;
	const t_vec3d		rand_dir = ft_vec3d_random(&rngState);
	const t_vec3d		new_dir = ft_vec3d_add(rand_dir, hit->hit_object->norm(*ray, *hit));

	if (rngState == 0)
		rngState = ft_atoll(__TIME__);
	rngState = rngState + frame * 1471343;
	ray->direction = ft_vec3d_norm(new_dir);
	ray->origin = ft_vec3d_add(hit->position, ft_vec3d_mult(ray->direction, 0.0001f));
}

static t_color_norm	rt_ray_loop(t_rt_renderer *renderer, t_rt_ray ray)
{
	t_rt_hit			hit;
	t_color_norm		light;

	light = (t_color_norm){.a = 1., .r = 0., .g = 0., .b = 0.};
	while (ray.bounces++ < MAX_BOUNCES)
	{
		hit = (t_rt_hit) {(t_vec3d) {0.0f, 0.0f, 0.0f}, NULL, false, INFINITY};
		rt_ray_cast(renderer->scene, &ray, &hit);
		if (hit.hit && hit.hit_object && hit.dist < 1000.)
		{
			rt_ray_render(renderer->scene, ray, hit, rt_color_from_norm(hit.hit_object->material.obj_color));
			rt_ray_update(renderer, &ray, &hit);
			rt_ray_colorize(&ray, &hit, &light);
		}
		else
			light = rt_color_add(light, rt_color_mult(ray.color, rt_get_environment(renderer, &ray)));
		if (!hit.hit || !hit.hit_object || hit.hit_object->material.emi_strength >= 1.)
			break ;
	}
	return (light);
}

t_color_norm	rt_get_color(t_rt_renderer *renderer, t_vec2i coords)
{
	static t_color_norm	pixels[1920];
	t_color_norm		light;
	t_rt_ray			ray;
	t_rt_scene			*scene;

	scene = renderer->scene;
	if (!(coords.x % scene->pratio) && !(coords.y % scene->pratio))
	{
		ray.bounces = 0;
		ray.color = (t_color_norm){1., 1., 1., 1.};
		rt_ray_init(scene, &ray, (t_vec2i){coords.x, HEIGHT - coords.y});
		light = rt_ray_loop(renderer, ray);
		pixels[coords.y / scene->pratio] = light;
		return (light);
	}
	return (pixels[coords.y / scene->pratio]);
}

void	rt_render_put_pixel(t_rt_renderer *renderer, t_vec2i coords)
{
	const t_rt_mlx_data	*mlx = renderer->mlx;
	const size_t		index = coords.x * renderer->scene->height + coords.y;
	const t_color_norm	color_norm = (renderer->image)[index];
	const uint32_t		color_int = rt_color_argb(rt_color_from_norm(color_norm));
	
	mlx_set_image_pixel(mlx->rt_mlx, mlx->rt_imgs[0], coords.x, coords.y, color_int);
}

static void	rt_accumulate_pixel(t_rt_renderer *renderer, t_vec2i coords)
{
	const int			pos = coords.x * renderer->scene->height + coords.y;
	const t_color_norm	image_color = (renderer->image)[pos];
	const t_color_norm	calc_color = (renderer->calc)[pos];
	const double		weight = 1. / ((renderer->rendered + 1));
		
	(renderer->image)[pos] = (t_color_norm){
		.a = 1.0f,
		.r = image_color.r * (1.0 - weight) + calc_color.r * weight,
		.g = image_color.g * (1.0 - weight) + calc_color.g * weight,
		.b = image_color.b * (1.0 - weight) + calc_color.b * weight};
}

static void	rt_accumulate(t_rt_renderer *renderer)
{
	t_vec2i			incs;

	incs.x = 0;
	while (incs.x < renderer->scene->width)
	{
		incs.y = 0;
		while (incs.y < renderer->scene->height)
		{
			rt_accumulate_pixel(renderer, incs);
			rt_render_put_pixel(renderer, incs);
			incs.y++;
		}
		incs.x++;
	}
	renderer->rendered++;
}

static void	rt_render_shoot_pixel(t_rt_renderer *renderer, t_vec2i coords)
{
	size_t			i;
	t_color_norm	accu;
	t_color_norm	pixel;

	i = 0;
	accu = (t_color_norm){.a = 1.0f, .r = 0.0f, .g = 0.0f, .b = 0.0f};	
	while (i++ < RAY_PER_PIXEL)
	{
		pixel = rt_get_color(renderer, coords);
		accu = (t_color_norm){
			.a = 1.0f, 
			.r = accu.r + pixel.r,
			.g = accu.g + pixel.g,
			.b = accu.b + pixel.b};	
	}
	accu = (t_color_norm){.a = 1.0f, .r = accu.r / (float) RAY_PER_PIXEL,
			.g = accu.g / (float) RAY_PER_PIXEL,
			.b = accu.b / (float) RAY_PER_PIXEL};
	(renderer->calc)[(coords.x * renderer->scene->height) + coords.y] = accu;
}

static void	rt_render_shoot_rays(t_rt_renderer *renderer)
{
	t_vec2i			coords;

	coords.x = -1;
	while (++coords.x < renderer->scene->width)
	{
		coords.y = -1;
		while (++coords.y < renderer->scene->height)
			rt_render_shoot_pixel(renderer, coords);
	}
}

static void	rt_render_put_line(t_rt_renderer *renderer, int line_y)
{
	int	i;
	
	i = -1;
	while (++i < renderer->scene->width)
		rt_render_put_pixel(renderer, (t_vec2i) {.x = i, .y = line_y});
}

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct s_thread_data
{
	size_t 			line_y;
	int				nlines;
	t_rt_renderer	renderer;
}	t_thread_data;

static void *rt_tamere(void *param)
{
	t_thread_data 	*data;
	t_rt_renderer 	*renderer;
	t_vec2i			coords;

	data = (t_thread_data *) param;
	renderer = &(data->renderer);
	coords = (t_vec2i) {.x = 0, .y = data->line_y};
	size_t cache2 = renderer->rendered;
	while (data->nlines > 0 && coords.y < renderer->scene->height) {
		while (coords.x < renderer->scene->width) 
		{
			renderer->rendered = cache2;
			int n_pass = 0;
			while (n_pass++ < MAX_SEQ_PASSES)
			{
				rt_render_shoot_pixel(renderer, coords);
				rt_accumulate_pixel(renderer, coords);
				renderer->rendered++;
			}
			rt_render_put_pixel(renderer, coords);
			coords.x++;
		}
		coords.x = 0;
		coords.y++;
		data->nlines--;
	}
	mlx_put_image_to_window(renderer->mlx->rt_mlx, renderer->mlx->rt_win, renderer->mlx->rt_imgs[0], 0, 0);
	free(param);
	return NULL;
}

void	rt_render_scene(t_rt_renderer *renderer)
{
	static t_vec2i		coords = {.x = 0, .y = 0};
	t_rt_mlx_data		mlx;
	size_t				cache_rendered;

	mlx = *renderer->mlx;
	if (!(renderer->scene->rt_flags & RT_NO_RENDER))
	{
		if (!(renderer->scene->rt_flags & RT_SEQ_RENDER))
		{
			rt_render_shoot_rays(renderer);
			rt_accumulate(renderer);
		}
		else
		{
			(void) rt_render_put_line;
			int nthreads = 24;
			int init_threads = 0;
			int lines_per_thread = 1;
			cache_rendered = renderer->rendered;
			renderer->rendered *= MAX_SEQ_PASSES;
			pthread_t threads[nthreads];
			ft_memset(threads, 0, nthreads * sizeof(pthread_t));
			for (int i = 0; i < nthreads; i++) {
				t_thread_data *datawowow = malloc(sizeof(t_thread_data));
				if (!datawowow) {
					printf("tout a pete\n");
					exit(-1);
				}
				datawowow->line_y = coords.y;
				datawowow->nlines = lines_per_thread;
				datawowow->renderer = *renderer;
				coords.y += lines_per_thread;
				pthread_create(&threads[init_threads], NULL, &rt_tamere, (void *)datawowow);
				init_threads++;
				if (coords.y >= renderer->scene->height)
					break ;
			}
			for (int i = 0; i < init_threads; i++)
				pthread_join(threads[i], NULL);
			renderer->rendered = cache_rendered;
			if (coords.y >= renderer->scene->height)
			{
				coords.y = 0;
				renderer->rendered++;
			}

			// for (int i = 0; i < renderer->scene->width * 50; i++) {
			// 	cache_rendered = renderer->rendered;
			// 	renderer->rendered = cache_rendered * MAX_SEQ_PASSES;
			// 	n_pass = 0;
			// 	while (n_pass++ < MAX_SEQ_PASSES)
			// 	{
			// 		rt_render_shoot_pixel(renderer, coords);
			// 		rt_accumulate_pixel(renderer, coords);
			// 		renderer->rendered++;
			// 	}
			// 	renderer->rendered = cache_rendered;
			// 	coords.x++;
			// 	if (coords.x >= renderer->scene->width)
			// 	{
			// 		rt_render_put_line(renderer, coords.y);
			// 		coords.x = 0;
			// 		coords.y++;
			// 		if (coords.y >= renderer->scene->height)
			// 		{
			// 			renderer->rendered++;
			// 			coords.y = 0;
			// 			break;
			// 		}
			// 	}
			// }
		}
	}
	// ft_printf("rendering ! %d\n", renderer->rendered);

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
