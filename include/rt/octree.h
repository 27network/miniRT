/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   octree.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 19:40:32 by rgramati          #+#    #+#             */
/*   Updated: 2024/06/01 19:10:14 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OCTREE_H
# define OCTREE_H
# undef OCTREE_H
# ifndef __OCTREE_H__
#  define __OCTREE_H__

#  include <ft/mem.h>
#  include <ft/math.h>
#  include <rt/scene.h>

#  define RT_OCTREE_MIN_OBJS 16
#  define RT_OCTREE_MAX_DEPTH 10

typedef struct	s_rt_octree_node
{
	struct s_rt_octree_node	*childs[8];
	t_aabb					aabb;
	t_rt_object				**objects;
	short					capacity;
	short					n_obj;
	short					depth;
	bool					term;
}	t_rt_onode;

# endif // __OCTREE_H__
#endif // OCTREE_H

