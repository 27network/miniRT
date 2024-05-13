/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_render_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 14:50:03 by rgramati          #+#    #+#             */
/*   Updated: 2024/05/11 19:47:30 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <ft/math.h>
#include <rt/renderer.h>

static double ft_clamp(double d, double min, double max)
{
    if (d < min)
        return min;
    if (d > max)
        return max;
    return d;
}

t_vec3d		ft_vec3d_lerp(t_vec3d a, t_vec3d b, double t)
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

double	ft_smoothstep(double start, double end, double x)
{
    x = ft_clamp((x - start) / (end - start), 0.0, 1.0);
    return (x * x * (3 - 2 * x));
}

float	ft_random_value(long long *state)
{
	uint32_t	result;

	*state = *state * 747796405 + 2891336453;
	result = (*state >> ((*state >> 28) + 4) ^ *state) * 277803737;
	result = (result >> 22) ^ result;
	return (result / 4294967295.0);
}

static float	ft_random_normal_value(long long *state)
{
	float	theta;
	float	rho;

	theta = 2 * RT_PI * ft_random_value(state);
	rho = sqrt(-2.0f * log(ft_random_value(state)));
	return (rho * cos(theta));
}

t_vec3d	ft_vec3d_random(long long *state)
{
	t_vec3d	dir;

	dir = (t_vec3d){.x = ft_random_normal_value(state),
					.y = ft_random_normal_value(state),
					.z = ft_random_normal_value(state)};
	return (ft_vec3d_norm(dir));
}