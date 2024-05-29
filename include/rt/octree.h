/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   octree.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 19:40:32 by rgramati          #+#    #+#             */
/*   Updated: 2024/05/24 21:02:45 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OCTREE_H
# define OCTREE_H
# undef OCTREE_H
# ifndef __OCTREE_H__
#  define __OCTREE_H__

#  include <rt/scene.h>
#  include <ft/math.h>

typedef struct	s_rt_octree_node
{
	struct s_rt_octree_node	*childs[8];
	t_vec3d					mid_planes;
	bool					term;
}	t_rt_onode;

# endif // __OCTREE_H__
#endif // OCTREE_H

