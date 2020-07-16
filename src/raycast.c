/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <willehard@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/16 16:10:39 by wkorande          #+#    #+#             */
/*   Updated: 2020/07/16 10:47:51 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include "shape.h"
#include "vector.h"
#include "libft.h"
#include <math.h>

int	trace(t_ray *ray, t_scene *scene, t_raycast_hit *hit, int stop_at_first)
{
	t_raycast_hit	cur_hit;
	double	min_dist;
	size_t		i;
	int		hit_found;

	min_dist = INFINITY;
	cur_hit.distance = INFINITY;
	hit_found = FALSE;
	i = 0;
	while (i < scene->num_shapes)
	{
		if (intersects_shape(ray, &scene->shapes[i], &cur_hit, scene->help_ray))
		{
			if (stop_at_first == 1) // tarviiks muuhun vai oliks varjoo varten?
				return (TRUE);
			if (stop_at_first != 2 || cur_hit.distance < hit->light_dist) // to not accept shadow if shadow is behind light
			{
				hit_found = TRUE;
				if (cur_hit.distance < min_dist)
				{
					*hit = cur_hit;
					min_dist = cur_hit.distance;
					if (scene->help_ray == 1)
						ft_printf("closest distance %f\n", min_dist);
				}
			}
		}
		i++;
	}
	return (hit_found);
}

// static void		init_color_data(t_color_data *color, t_scene *scene,
// 									t_hit *hit)
// {
// 	color->ambient = ft_mul_rgba(hit->object->color,
// 		ft_intensity_rgba(scene->ambient_color));
// 	color->diffuse = ft_make_rgba(0, 0, 0, 1);
// 	color->specular = ft_make_rgba(0, 0, 0, 1);
// 	color->attenuation = 0.0;
// }

static t_rgba	calc_reflect(t_scene *scene, t_vec3 point, t_vec3 idir, t_vec3 normal, int depth)
{
	t_ray			reflect_ray;
	t_rgba			color;

	reflect_ray.origin = ft_add_vec3(point, ft_mul_vec3(normal, EPSILON));
	reflect_ray.direction = ft_normalize_vec3(ft_reflect_vec3(idir, normal));
	color = raycast(&reflect_ray, scene, depth + 1);
	if (scene->help_ray == 1)
		print_rgba("reflect color", color);
	return (color);
}

static double		calc_diffuse(t_light light, t_raycast_hit hit, t_scene *scene)
{
	double	d;
	double	distance;
	double	intensity;
	t_vec3	light_dir;

	if (light.type == DIRECTIONAL)
		light_dir = ft_sub_vec3(light.position, hit.point); // light needs a target field or rotation that we can calculate a direction from
	else
		light_dir = ft_sub_vec3(light.position, hit.point);
	distance = ft_len_vec3(light_dir);
	light_dir = ft_normalize_vec3(light_dir);
	d = ft_dot_vec3(light_dir, hit.normal);
	if (light.type == DIRECTIONAL)
		intensity = light.intensity / 1000;
	else
		intensity = light.intensity / 10 * (1 / (1 + distance + ft_pow(distance, 2)));
	if (scene->help_ray == 1)
		ft_printf(" ray surface dot %f\n", d); //
	return (ft_clamp_d(d, 0, 1));

}

double	calc_shadow(t_light light, t_raycast_hit hit, t_scene *scene);

static t_rgba	shade(t_scene *scene, t_raycast_hit *hit)
{
	t_rgba		ambient;
	t_rgba		light;
	t_rgba		object_c;
	t_rgba		rc;
	double		reflect;
	size_t		i;
	double		d;
	double		s;

	s = 0;

	i = 0;
	d = 0;
	ambient = scene->scene_config.ambient;
	object_c = hit->shape->color;
	reflect = scene->scene_config.reflection == 1 ? hit->shape->reflection : 0;
	if (hit->shape)
	{
		while ( i < scene->num_lights)
		{
			light = ft_make_rgba(0, 0, 0, 1);
			light = scene->lights[i].color;
			d += calc_diffuse(scene->lights[i], *hit, scene);
			s += calc_shadow(scene->lights[i], *hit, scene);
			if (scene->help_ray)
				ft_printf("s: %f\n", s);
			light = ft_mul_rgba(light, d);
			ambient = ft_add_rgba(ambient, light);
			i++;
		}
		rc = calc_reflect(scene, hit->point, hit->ray.direction, hit->normal, hit->depth);
		rc = ft_mul_rgba(rc, reflect);
		//c = ft_add_rgba(c, rc);
		object_c = ft_blend_rgba(ft_mul_rgba(ambient, ft_intensity_rgba(ambient)), object_c);
		object_c = ft_blend_rgba(object_c, rc);
		return (ft_mul_rgba(object_c, 1.0 - s));
	}
	return (ambient);
}

static void		init_hit_info(t_raycast_hit *hit)
{
	hit->shape = NULL;
}

t_rgba			raycast(t_ray *ray, t_scene *scene, int depth)
{
	t_rgba color;
	t_raycast_hit hit;

	color = scene->scene_config.ambient;
	if (depth > MAX_BOUNCES)
		return (color); // red for now should be ambient?
	init_hit_info(&hit);
	if (trace(ray, scene, &hit, FALSE) != FALSE)
	{
		hit.depth = depth;
		hit.normal = calc_hit_normal(&hit);
		hit.ray = *ray;
		color = shade(scene, &hit);
	}
	return (ft_clamp_rgba(color));
}
