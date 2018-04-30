/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling_window_resizable.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ftymchyn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/19 16:13:53 by ftymchyn          #+#    #+#             */
/*   Updated: 2018/04/19 16:13:55 by ftymchyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	handling_window_resizable(t_rt *rt, SDL_Event e)
{
	rt->sdl.win_w = (e.window.data1 > MAX_WIDTH) ? MAX_WIDTH : e.window.data1;
	rt->sdl.win_h = (e.window.data2 > MAX_HEIGHT) ? MAX_HEIGHT : e.window.data2;
	SDL_DestroyTexture(rt->sdl.canvas);
	clReleaseMemObject(rt->pixels_mem);
	free(rt->sdl.pixels);
	SDL_DestroyRenderer(rt->sdl.renderer);
	rt->sdl.renderer = SDL_CreateRenderer(rt->sdl.win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (rt->sdl.renderer == NULL) {
		ft_putendl_fd("failed to initiate renderer in SDL", 2);
	}
    SDL_RenderSetLogicalSize(rt->sdl.renderer, rt->sdl.win_w, rt->sdl.win_h);
	rt->samples = 0;
	rt->job_size = rt->sdl.win_w * rt->sdl.win_h;
	create_canvas(rt, rt->sdl.win_w, rt->sdl.win_h);
	rt->sdl.pixels =
		(int*)malloc(sizeof(int) * rt->job_size);
	rt_check_error(!rt->sdl.pixels, MALLOC_ERR, NULL);
	rt->pixels_mem =
		rt_cl_malloc_read(&rt->info, sizeof(cl_int) * rt->job_size);
	init_camera(rt, rt->scene.camera.origin);
	rotate_camera(rt);
	clSetKernelArg(rt->kernel.kernel, 2, sizeof(t_camera), &rt->scene.camera);
	clSetKernelArg(rt->kernel.kernel, 5, sizeof(cl_mem), &rt->pixels_mem);
}

