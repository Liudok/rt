/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_opencl.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ftymchyn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/16 15:26:45 by ftymchyn          #+#    #+#             */
/*   Updated: 2018/05/10 19:42:20 by skamoza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include "../../include/rt.h"

static cl_uint	*make_seeds(t_rt *rt)
{
	cl_uint	*seeds;
	uint	local_seeds[2];
	uint	i;

	i = (uint)-1;
	seeds = (cl_uint *)malloc(MAX_WIDTH * MAX_HEIGHT * 2 * sizeof(cl_uint));
	rt_check_error(!seeds, MALLOC_ERR, NULL);
	rt->job_size = rt->sdl.win_w * rt->sdl.win_h;
	if (rt->painting_flag == 0)
		while (++i < (unsigned int)(MAX_WIDTH * MAX_HEIGHT * 2))
		{
			seeds[i] = rand();
			if (seeds[i] < 2)
				seeds[i] = 2;
		}
	else
		while (++i < (unsigned int)(MAX_WIDTH * MAX_HEIGHT * 2))
		{
			seeds[i] = local_seeds[i % 2];
			if (seeds[i] < 2)
				seeds[i] = 2;
		}
	return (seeds);
}

void			set_kernel_args(t_rt *rt)
{
	uint				i;
	static const size_t	sizes[] = {
		sizeof(cl_mem), sizeof(cl_uint), sizeof(t_camera),
		sizeof(cl_mem), sizeof(cl_mem), sizeof(cl_uint),
		sizeof(cl_mem), sizeof(cl_mem)};
	const void			*ptrs[] = {&rt->scene.objs_mem, &rt->scene.objnum,
		&rt->scene.camera, &rt->seeds, &rt->colors,
		&rt->samples, &rt->textures_mem, &rt->tex_size_mem};

	i = (uint)-1;
	while (++i < sizeof(ptrs) / sizeof(void *))
		clSetKernelArg(rt->kernel.kernel, i, sizes[i], ptrs[i]);
	clSetKernelArg(rt->effect_kernel.kernel, 0, sizes[0], &rt->colors);
	clSetKernelArg(rt->effect_kernel.kernel, 1, sizes[0], &rt->pixels_mem);
	clSetKernelArg(rt->effect_kernel.kernel, 2, 1, &rt->effect_type);
	clSetKernelArg(rt->effect_kernel.kernel, 3, sizes[2], &rt->scene.camera);
}

void			init_opencl(t_rt *rt)
{
	cl_uint *seeds;

	rt->effect_type = 0;
	seeds = make_seeds(rt);
	rt_cl_init(&rt->info);
	rt_cl_compile(&rt->info, "src/opencl/kernel.cl");
	rt->kernel = rt_cl_create_kernel(&rt->info, "path_tracing");
	rt->mouse_kernel = rt_cl_create_kernel(&rt->info, "mouse_hook");
	rt->effect_kernel = rt_cl_create_kernel(&rt->info, "after_effects");
	rt->mouse_intersect = rt_cl_malloc_read(&rt->info, sizeof(cl_int));
	rt->scene.objs_mem = rt_cl_malloc_write(
			&rt->info, sizeof(t_object) * rt->scene.objnum, rt->scene.objs);
	rt->seeds = rt_cl_malloc_write(
			&rt->info, sizeof(cl_uint) * MAX_WIDTH * MAX_HEIGHT * 2, seeds);
	rt->colors = rt_cl_malloc_read(
			&rt->info, sizeof(cl_float3) * MAX_WIDTH * MAX_HEIGHT * 2);
	rt->textures_mem = rt_cl_create_image_tex(&rt->info, &rt->textures_img[0],
			rt->texture_sizes);
	rt->pixels_mem = rt_cl_malloc_read(&rt->info, sizeof(cl_int) *
			rt->job_size);
	rt->tex_size_mem = rt_cl_malloc_write(&rt->info, sizeof(cl_uint2)
			* (2 + NUM_TEX), &rt->texture_sizes);
	set_kernel_args(rt);
	free(seeds);
}

void			reinit_opencl(t_rt *rt)
{
	cl_uint *seeds;

	seeds = make_seeds(rt);
	rt->samples = 0;
	clReleaseMemObject(rt->scene.objs_mem);
	rt->scene.objs_mem = rt_cl_malloc_write(&rt->info,
			sizeof(t_object) * rt->scene.objnum, rt->scene.objs);
	rt->seeds = rt_cl_malloc_write(
			&rt->info, sizeof(cl_uint) * MAX_WIDTH * MAX_HEIGHT * 2, seeds);
	clSetKernelArg(rt->kernel.kernel, 0, sizeof(cl_mem), &rt->scene.objs_mem);
	clSetKernelArg(rt->kernel.kernel, 1, sizeof(cl_uint), &rt->scene.objnum);
	clSetKernelArg(rt->kernel.kernel, 2, sizeof(t_camera), &rt->scene.camera);
	clSetKernelArg(rt->kernel.kernel, 3, sizeof(cl_mem), &rt->seeds);
	free(seeds);
}

int				mouse_ray(t_rt *rt, int x, int y)
{
	cl_uint2	coords;
	cl_int		i;

	coords.x = x;
	coords.y = y;
	clSetKernelArg(rt->mouse_kernel.kernel, 0, sizeof(cl_mem),
			&rt->scene.objs_mem);
	clSetKernelArg(rt->mouse_kernel.kernel, 1, sizeof(cl_uint),
			&rt->scene.objnum);
	clSetKernelArg(rt->mouse_kernel.kernel, 2, sizeof(t_camera),
			&rt->scene.camera);
	clSetKernelArg(rt->mouse_kernel.kernel, 3, sizeof(cl_uint2),
			&coords);
	clSetKernelArg(rt->mouse_kernel.kernel, 4, sizeof(cl_mem),
			&rt->mouse_intersect);
	clEnqueueTask(rt->info.command_queue, rt->mouse_kernel.kernel,
			0, NULL, NULL);
	rt_cl_device_to_host(&rt->info, rt->mouse_intersect, &i, sizeof(cl_int));
	return (i);
}
