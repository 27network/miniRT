/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   features.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 02:40:06 by kiroussa          #+#    #+#             */
/*   Updated: 2024/02/22 02:49:20 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FEATURES_H
# define FEATURES_H

# define FEAT_MULTIPLE_LIGHTS 0
# define FEAT_LIGHT_COLOR 0
# define FEAT_OBJ_CONE 0
# define FEAT_BUMP_MAP 0

# ifdef RT_MODE
#  define MINIRT_BONUS
#  define FEAT_MULTIPLE_LIGHTS 1
# endif // RT_MODE

# ifdef MINIRT_BONUS
#  define FEAT_LIGHT_COLOR 1
#  define FEAT_OBJ_CONE 1
#  define FEAT_BUMP_MAP 1
# endif // MINIRT_BONUS

#endif // FEATURES_H
