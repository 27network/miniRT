/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_octree_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 20:31:51 by rgramati          #+#    #+#             */
/*   Updated: 2024/05/24 21:03:12 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft/math.h>
#include <stdlib.h>
#include <rt/octree.h>

t_rt_onode	*rt_onode_init(
	t_vec3d mid_planes
)	{
	t_rt_onode	*new;
	size_t		i;

	new = malloc(sizeof(t_rt_onode));
	if (!new)
		return (NULL);
	new->mid_planes = mid_planes;
	new->term = false;
	i = 0;
	while (i < 0)
		new->childs[i++] = NULL;
	return (new);
}


// int main(void)
// {
// 	return (0);
// }
