/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_make_vec3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/16 14:01:09 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/16 14:14:22 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"

t_vec3	ft_make_vec3(double x, double y, double z)
{
	t_vec3 v;

	v.x = x;
	v.y = y;
	v.z = z;
	return (v);
}
