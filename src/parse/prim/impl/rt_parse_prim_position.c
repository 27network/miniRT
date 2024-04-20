/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_parse_prim_position.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 19:44:06 by kiroussa          #+#    #+#             */
/*   Updated: 2024/04/18 22:07:06 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <rt/parse/prims.h>

static void	rt_free_array(char **array)
{
	size_t	i;

	i = 0;
	while (array[i])
		free(array[i++]);
	free(array);
}

t_rt_error	rt_parse_prim_position(
	t_rt_parse_ctx *ctx,
	const char *slice,
	void *target
) {
	t_rt_error	err;
	char		**parts;
	size_t		length;

	parts = ft_split(slice, ',');
	if (!parts)
		return (rt_err(RT_ERROR_ALLOC));
	length = 0;
	while (parts[length])
		length++;
	if (length != 3)
	{
		if (length > 3)
			err = rt_parse_error(slice, "Too many values, expected 3");
		else
			err = rt_parse_error(slice, "Not enough values, expected 3");
	}
	if (err.type == RT_OK)
	{
	}
	rt_free_array(parts);
	return (err);
}

__attribute__((constructor))
void	rt_prim_position_init(void)
{
	rt_parse_register_prim((t_rt_parser){
		.name = "position",
		.fn = rt_parse_prim_position,
		.size = sizeof(t_vec3d)
	});
}
