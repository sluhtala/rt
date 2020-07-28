/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <willehard@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/23 23:03:26 by rjaakonm          #+#    #+#             */
/*   Updated: 2020/07/29 00:23:44 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include "debug.h"

void	init_camera(t_vec3 origin, t_vec3 target, t_camera *cam, t_scene *scene)
{
	cam->forward = ft_normalize_vec3(ft_rotate_vec3(
		ft_sub_vec3(target, origin), cam->rotation));
	cam->right = ft_normalize_vec3(ft_cross_vec3(cam->forward,
		ft_make_vec3(0, 1, -EPSILON)));
	cam->up = ft_normalize_vec3(ft_cross_vec3(cam->right, cam->forward));
	cam->horizontal = tan((cam->fov * M_PI / 180.0) / 2);
	cam->aspect = cam->aspect * (double)scene->scene_config.width /
		(double)scene->scene_config.height;
	cam->vertical = cam->horizontal / cam->aspect;
}

t_vec2   random_inside_unit_circle(void)
{
	t_vec2 p;
	double r;
	double theta;
	
	r = sqrt((double)rand() / RAND_MAX * 2.0);
	theta = ((double)rand() / RAND_MAX * 2.0) * 2 * M_PI;
	p.x = r * cos(theta);
	p.y = r * sin(theta);
	return (p);
}

double   random_d(void)
{
	return ((double)rand() / RAND_MAX * 2.0 - 1.0);
}

t_ray get_camera_ray(t_scene *scene, t_camera *cam, double screen_x,
	double screen_y)
{
	t_ray ray;
	t_vec3 r;
	t_vec3 u;
	t_vec2 target;

	t_vec3 focal_point;
	double focal_length;
	double aperture;

	aperture = 0.2;
	focal_length = 4;

	target.x = 2.0 * screen_x / (scene->scene_config.width - 1) - 1.0;
	target.y = -2.0 * screen_y / (scene->scene_config.height - 1) + 1.0;
	if (cam->type == PERSPECTIVE)
	{
		ray.origin = cam->position;
		
		ray.direction = cam->forward;
		r = ft_mul_vec3(cam->right, target.x * cam->horizontal);
		u = ft_mul_vec3(cam->up, target.y * cam->vertical);
		ray.direction = ft_add_vec3(ray.direction, r);
		ray.direction = ft_add_vec3(ray.direction, u);
		ray.direction = ft_normalize_vec3(ray.direction);

		focal_point = ft_add_vec3(cam->position, ft_mul_vec3(ray.direction, focal_length));
		ray.origin.x += random_d() * aperture;
		ray.origin.y += random_d() * aperture;
		ray.origin.z += random_d() * aperture;
		ray.direction = ft_normalize_vec3(ft_sub_vec3(focal_point, ray.origin));
	}
	else if (cam->type == ORTHOGRAPHIC)
	{
		ray.direction = cam->forward;
		r = ft_mul_vec3(cam->right, target.x * cam->horizontal * (cam->fov / 5.0));
		u = ft_mul_vec3(cam->up, target.y * cam->vertical * (cam->fov / 5.0));
		ray.origin = cam->position;
		ray.origin = ft_add_vec3(ray.origin, r);
		ray.origin = ft_add_vec3(ray.origin, u);
	}
	else if (cam->type == FISHEYE)
	{
		double x = 180.0 - ((screen_x / scene->scene_config.width) * 360.0);
		double y = 180.0 - ((screen_y / scene->scene_config.height) * 360.0);
		t_vec3 v = ft_rotate_vec3(cam->forward, ft_make_vec3(y, x, 0.0));
		ray.origin = cam->position;
		ray.direction = ft_normalize_vec3(v);
	}


	ray.source_shape = NULL;
	ray.is_shadow = FALSE;
	return (ray);
}
