/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_rendering.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 14:43:52 by rgramati          #+#    #+#             */
/*   Updated: 2024/05/05 21:10:12 by rgramati         ###   ########.fr       */
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

float	rt_random_value(long long *state)
{
	uint32_t	result;

	*state = *state * 747796405 + 2891336453;
	result = (*state >> ((*state >> 28) + 4) ^ *state) * 277803737;
	result = (result >> 22) ^ result;
	return (result / 4294967295.0);
}

float	rt_random_normal_value(long long *state)
{
	float	theta;
	float	rho;

	theta = 2 * RT_PI * rt_random_value(state);
	rho = sqrt(-2.0f * log(rt_random_value(state)));
	return (rho * cos(theta));
}

t_vec3d	rt_random_direction(long long *state)
{
	t_vec3d	dir;

	dir = (t_vec3d){.x = rt_random_normal_value(state),
					.y = rt_random_normal_value(state),
					.z = rt_random_normal_value(state)};
	return (ft_vec3d_norm(dir));
}

static t_vec3d	rt_random_hemisphere_direction(long long *state, t_vec3d norm)
{
	t_vec3d	dir;

	dir = rt_random_direction(state);
	if (ft_vec3d_dot(norm, dir) < 0)
		dir = ft_vec3d_mult(dir, -1.0f);
	return (dir);
}

static double ft_clamp(double d, double min, double max) {
    if (d < min)
        return min;
    if (d > max)
        return max;
    return d;
}

static t_vec3d		ft_vec3d_lerp(t_vec3d a, t_vec3d b, double t)
{
	t_vec3d	result;
	double	t_inv;

	t = ft_clamp(t, 0., 1.);
	t_inv = 1 - t;
	result.x = (a.x * t_inv) + (b.x * t);
	result.y = (a.y * t_inv) + (b.y * t);
	result.z = (a.z * t_inv) + (b.z * t);
	return (result);
}

static double	ft_smoothstep(double edge0, double edge1, double x)
{
    x = ft_clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
    return (x * x * (3 - 2 * x));
}

static t_color_norm	rt_get_environment(__attribute__((unused)) t_rt_renderer *renderer, t_rt_ray *ray)
{
	/*
	float skyGradientT = pow(smoothstep(0, 0.4, ray.dir.y), 0.35);
	float groundToSkyT = smoothstep(-0.01, 0, ray.dir.y);
	float3 skyGradient = lerp(SkyColourHorizon, SkyColourZenith, skyGradientT);
	float sun = pow(max(0, dot(ray.dir, _WorldSpaceLightPos0.xyz)), SunFocus) * SunIntensity;
	// Combine ground, sky, and sun
	float3 composite = lerp(GroundColour, skyGradient, groundToSkyT) + sun * (groundToSkyT>=1);
	return composite;
	*/

	// const int SUN_FOCUS = 1;
	// const int SUN_INTENSITY = 1;

	const t_vec3d	COLOR1 = ft_vec3d(1.0f, 1.0f, 1.0f);
	const t_vec3d	COLOR2 = ft_vec3d(0x87 / 255., 0xCE / 255., 0xEB / 255.);
	const t_vec3d	GROUND_COLOR = ft_vec3d(0x8C / 255., 0x83 / 255., 0x91 / 255.);

	double 	ground_to_sky;
	t_vec3d	sky;
	// double	sun;
	t_vec3d	composite;

	ground_to_sky = ft_smoothstep(-.01, 0, ray->direction.y);
	sky = ft_vec3d_lerp(COLOR1, COLOR2, pow(ft_smoothstep(0, 0.4, ray->direction.y), 0.35));
	// sun = ft_fpow(ft_fmax(0, ft_vec3d_dot(ray->direction, )), SUN_FOCUS) * SUN_INTENSITY;
	composite = ft_vec3d_lerp(GROUND_COLOR, sky, ground_to_sky);// + sun * (ground_to_sky >= 1);
	return ((t_color_norm){.a = 1., .r = composite.x, .g = composite.y, .b = composite.z});
}

t_color_norm	rt_get_color(t_rt_renderer *renderer, t_vec2i coords)
{
	static long long	rngState = 0;
	static t_color_norm	pixels[1920];
	t_color_norm		light;
	t_color_norm		color;
	t_rt_ray			ray;
	t_rt_hit			hit;
	t_rt_scene			*scene;
	const size_t		frame = renderer->rendered;

	scene = renderer->scene;
	if (rngState == 0)
		rngState = ft_atoll(__TIME__);
	color = (t_color_norm){1., 1., 1., 1.};
	light = (t_color_norm){1., .0, .0, .0};
	if (!(coords.x % scene->pratio) && !(coords.y % scene->pratio))
	{
		ray.bounces = 0;
		rt_ray_init(scene, &ray, coords);
		while (ray.bounces < MAX_BOUNCES)
		{
			hit = (t_rt_hit) {(t_vec3d) {0.0f, 0.0f, 0.0f}, NULL, false, INFINITY};
			rt_ray_cast(scene, &ray, &hit);
			if (hit.hit && hit.hit_object)
			{
				if (ray.bounces == 0 && scene->rt_flags & RT_RAY_DEBUG && hit.hit_object && hit.hit_object->type != RT_OBJ_PLANE)
					rt_render_ray(scene, ray, hit, rt_color_from_norm(hit.hit_object->material.obj_color));
				
				rngState = rngState + frame * 1471343;
				ray.direction = rt_random_hemisphere_direction(&rngState, hit.hit_object->norm(ray, hit));
				// ray.direction = hit.hit_object->norm(ray, hit);
				ray.origin = ft_vec3d_add(hit.position, ft_vec3d_mult(ray.direction, 0.1f));
				
				t_color_norm emittedlight = rt_color_fact(hit.hit_object->material.emi_color, hit.hit_object->material.emi_strength);
				double lightStrength = ft_vec3d_dot(hit.hit_object->norm(ray, hit), ray.direction);
				

				light = rt_color_add(light, rt_color_mult(color, emittedlight, false));
				color = rt_color_mult(color, rt_color_fact(hit.hit_object->material.obj_color, 2 * lightStrength), false);
	
				double p = fmax(fmax(color.r, color.g), color.b);
				if (rt_random_value(&rngState) >= p)
					break ;
				
				color = rt_color_mult(color, (t_color_norm){.a = 1., .r = 1./p, .g = 1./p, .b = 1./p}, false);
			}
			else
			{
				light = rt_color_add(light, rt_color_mult(color, rt_get_environment(renderer, &ray), false));
				break ;
			}
			ray.bounces++;
		}
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

static void	rt_accumulate(t_rt_renderer *renderer)
{
	const double	weight = 1.0 / (renderer->rendered + 1.);
	t_vec2i			incs;
	t_color_norm	calc_color;
	t_color_norm	image_color;

	incs.x = 0;
	while (incs.x < renderer->scene->width)
	{
		incs.y = 0;
		while (incs.y < renderer->scene->height)
		{
			int	pos = incs.x * renderer->scene->height + incs.y;
			calc_color = (renderer->calc)[pos];
			image_color = (renderer->image)[pos];
			(renderer->image)[pos] = (t_color_norm){
				.a = 1.0f,
				.r = image_color.r * (1.0 - weight) + calc_color.r * weight,
				.g = image_color.g * (1.0 - weight) + calc_color.g * weight,
				.b = image_color.b * (1.0 - weight) + calc_color.b * weight};
			rt_render_put_pixel(renderer, incs);
			incs.y++;
		}
		incs.x++;
	}
	renderer->rendered++;
}


static void	rt_render_shoot_rays(t_rt_renderer *renderer)
{
	const int		rayperpixel = 1;
	t_vec2i			coords;
	size_t			i;
	t_color_norm	accu;
	t_color_norm	pixel;

	coords.x = -1;
	while (!(renderer->scene->rt_flags & RT_NO_RENDER) && ++coords.x < renderer->scene->width)
	{
		coords.y = -1;
		while (++coords.y < renderer->scene->height)
		{
			i = 0;
			accu = (t_color_norm){.a = 1.0f, .r = 0.0f, .g = 0.0f, .b = 0.0f};	
			while (i++ < rayperpixel)
			{
				pixel = rt_get_color(renderer, coords);
				accu = (t_color_norm){
					.a = 1.0f, 
					.r = accu.r + pixel.r,
					.g = accu.g + pixel.g,
					.b = accu.b + pixel.b};	
			}
			accu = (t_color_norm){.a = 1.0f, .r = accu.r / (float) rayperpixel,
					.g = accu.g / (float) rayperpixel,
					.b = accu.b / (float) rayperpixel};
			(renderer->calc)[(coords.x * renderer->scene->height) + coords.y] = accu;
		}
	}
}

void	rt_render_scene(t_rt_renderer *renderer)
{
	t_rt_mlx_data		mlx;

	mlx = *renderer->mlx;
	
	if (!(renderer->scene->rt_flags & RT_NO_RENDER))
	{
		rt_render_shoot_rays(renderer);
		rt_accumulate(renderer);
	}
	ft_printf("rendering !\n");

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
