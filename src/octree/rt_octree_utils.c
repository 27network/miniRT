/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_octree_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 20:31:51 by rgramati          #+#    #+#             */
/*   Updated: 2024/06/01 19:27:56 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft/math.h>
#include <rt/renderer.h>
#include <rt/octree.h>

bool	rt_aabb_contains(t_aabb *aabb, t_vec3d point)
{
	if (point.x < aabb->min.x || point.x > aabb->max.x)
		return (false);
	if (point.y < aabb->min.y || point.y > aabb->max.y)
		return (false);
	if (point.z < aabb->min.z || point.z > aabb->max.z)
		return (false);
    return (true);
}

bool	rt_aabb_intersect(t_aabb *aabb1, t_aabb *aabb2)
{
	if (aabb1->max.x < aabb2->min.x || aabb1->min.x > aabb2->max.x)
		return (false);
	if (aabb1->max.y < aabb2->min.y || aabb1->min.y > aabb2->max.y)
		return (false);
	if (aabb1->max.z < aabb2->min.z || aabb1->min.z > aabb2->max.z)
		return (false);
    return (true);
}

bool	rt_aabb_ray_intersect(t_rt_ray *ray, t_aabb *aabb)
{
	t_vec3d	bounds;
	t_vec3d temps;
	int		i;

	bounds = ft_vec3d((aabb->min.x - ray->origin.x) / ray->dir.x, \
		(aabb->max.x - ray->origin.x) / ray->dir.x, 0);
    if (bounds.x > bounds.y)
		rt_swap(&bounds.x, &bounds.y);
    i = 1;
	while (i < 3)
	{
        temps.x = *(((double *)&aabb->min) + i) - *(((double *)&ray->origin) + i) / *(((double *)&ray->dir) + i);
        temps.y = *(((double *)&aabb->min) + i) - *(((double *)&ray->origin) + i) / *(((double *)&ray->dir) + i);
        if (temps.x > temps.y)
			rt_swap(&bounds.x, &bounds.y);
        if (temps.x > bounds.x)
            bounds.x = temps.x;
        if (temps.y < bounds.y)
            bounds.y = temps.y;
        if (bounds.x > bounds.y)
            return (false);
    }
    return (true);
}

t_rt_onode	*rt_octree_node_init(t_aabb aabb, int depth)
{
	t_rt_onode	*new;
	size_t		i;

	new = ft_calloc(1, sizeof(t_rt_onode));
	if (!new)
		return (NULL);
	i = 0;
	while (i < 8)
		new->childs[i++] = NULL;
	new->aabb = aabb;
	new->objects = ft_calloc(RT_OCTREE_MIN_OBJS, sizeof(t_rt_object *));
	if (!new->objects)
	{
		free(new);
		return (NULL);
	}
	new->capacity = RT_OCTREE_MIN_OBJS;
	new->n_obj = 0;
	new->depth = depth;
	new->term = (depth == RT_OCTREE_MAX_DEPTH);
	return (new);
}

t_rt_error	rt_octree_node_expand(t_rt_onode *tree)
{
	t_rt_object	**new;
	int			i;

	tree->capacity *= 2;
	new = ft_calloc(tree->capacity, sizeof(t_rt_object *));
	if (!new)
		return (rt_errd(RT_ERROR_ALLOC, "rt_octree_node_expand malloc"));
	i = 0;
	while (i < tree->capacity / 2)
	{
		new[i] = tree->objects[i];
		i++;
	}
	free(tree->objects);
	tree->objects = new;
	return (rt_ok());
}

void	rt_octree_node_subdivide(t_rt_onode *tree)
{
	t_vec3d			mid_planes;
	t_aabb			aabb;
	int				i;
	int				j;

	mid_planes = ft_vec3d_div(ft_vec3d_add(tree->aabb.max, tree->aabb.min), 2);
	i = -1;
	while (++i < 8)
	{
		j = -1;
		while (++j < 3)
		{
			if (i & (1 << j))
			{
				*(((double *)&aabb.min) + j) = *(((double *)&mid_planes) + j);
				*(((double *)&aabb.max) + j) = *(((double *)&tree->aabb.max) + j);
			}
			else
			{
				*((double *)&(aabb.min) + j) = *(((double *)&tree->aabb.min) + j);
				*((double *)&(aabb.max) + j) = *(((double *)&mid_planes) + j);
			}
		}
		tree->childs[i] = rt_octree_node_init(aabb, tree->depth + 1);
	}
	
}

void	rt_octree_node_insert(t_rt_onode *tree, t_rt_object *obj)
{
	int	i;

	if (tree->depth == RT_OCTREE_MAX_DEPTH)
	{
		if (tree->n_obj == tree->capacity)
			rt_octree_node_expand(tree);
		tree->objects[tree->n_obj++] = obj;
		return ;
	}
	if (tree->n_obj < RT_OCTREE_MIN_OBJS)
	{
		tree->objects[tree->n_obj++] = obj;
		return ;
	}
	if (!tree->childs[0])
		rt_octree_node_subdivide(tree);
	i = 0;
	while (i < 8)
	{
		if (rt_aabb_intersect(&tree->childs[i]->aabb, &obj->aabb))
			rt_octree_node_insert(tree->childs[i], obj);
		i++;
	}
}
