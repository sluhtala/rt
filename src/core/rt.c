/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <willehard@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/09 13:50:27 by wkorande          #+#    #+#             */
/*   Updated: 2020/08/03 15:37:30 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include "thread_pool.h"
#include "shape.h"
#include "mlx_image.h"
#include "texture.h"
#include "ft_printf.h"
#include <stdarg.h>
#include "scene.h"
#include "material.h"

void	free_null(size_t count, ...)
{
	va_list pl;
	size_t	i;
	void	*param_type;

	i = 0;
	va_start(pl, count);
	while (i < count)
	{
		param_type = va_arg(pl, void *);
		free(param_type);
		param_type = NULL;
		i++;
	}
	va_end(pl);
}

void	exit_message(char *error)
{
	ft_putendl_fd(error, 2);
	exit(1);
}

t_rt	*rt_init(size_t num_scenes)
{
	t_rt	*rt;

	if (!(rt = (t_rt*)malloc(sizeof(t_rt))))
		exit_message("Failed to malloc rt!");
	if (!(rt->scenes = (t_scene**)malloc(sizeof(t_scene*) * num_scenes)))
		exit_message("Failed to malloc rt->scenes!");
	rt->num_scenes = num_scenes;
	rt->cur_scene = 0;
	rt->render_task.render_started = FALSE;
	rt->render_task.render_finished = FALSE;
	rt->render_task.done_tiles = NULL;
	rt->perlin_data = NULL;
	rt->render_task.thread_pool = NULL;
	rt->render_task.job_data_block = NULL;
	rt->render_task.jobs = 0;
	rt->render_task.num_jobs = 0;
	rt->render_requested = FALSE;
	srand(4);
	return (rt);
}

void	destroy_scene(t_rt *rt, t_scene *scene)
{
	size_t i;

	i = 0;
	while (i < scene->num_shapes)
	{
		if (scene->shapes[i].type == MODEL)
		{
			mesh_destroy(scene->shapes[i].mesh);
			octree_destroy(scene->shapes[i].octree);
		}
		if (scene->shapes[i].material->id == DEFAULT_MATERIAL_ID)
			free_null(1, scene->shapes[i].material);
		i++;
	}
	i = 0;
	while (i < scene->num_textures)
	{
		destroy_mlx_img(rt->mlx, scene->textures[i].img_data);
		i++;
	}
	free_null(6, scene->cameras, scene->lights, scene->shapes,
		scene->textures, scene->materials, scene);
}

void	rt_destroy_exit(t_rt *rt, int status)
{
	size_t i;

	i = 0;
	while (i < rt->num_scenes)
	{
		destroy_scene(rt, rt->scenes[i]);
		i++;
	}
	free(rt->scenes);
	free(rt->mlx);
	free(rt->perlin_data);
	free(rt);
	exit(status);
}
