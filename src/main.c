/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lberezyn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/25 12:14:39 by lberezyn          #+#    #+#             */
/*   Updated: 2018/05/10 15:52:48 by skamoza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/rt.h"

int g_error_flag = 0;

int			main(void)
{
	t_rt	rt;

	srand(time(0));
	rt.sdl.win_w = WINDOW_WIDTH;
	rt.sdl.win_h = WINDOW_HEIGHT;
	rt.scene.file = NULL;
	if (!sdl_init_everything(&rt))
		ft_error("Failed SDL initialization");
	init_default_scene(&rt);
	run_ui(&rt);
	destroy(&rt);
	return (0);
}

void		rt_check_error(int flag, char *err1, char *err2)
{
	if (flag)
	{
		ft_putstr_fd(err1, 2);
		ft_putstr_fd(" ", 2);
		if (err2)
			ft_putstr_fd(err2, 2);
		ft_putendl_fd("", 2);
		exit(0);
	}
}

void		ft_error(char *s)
{
	ft_putendl_fd(s, 2);
	exit(0);
}

void		destroy(t_rt *pt)
{
	free(pt->icon);
	SDL_DestroyTexture(pt->sdl.canvas);
	SDL_DestroyRenderer(pt->sdl.renderer);
	SDL_DestroyWindow(pt->sdl.win);
	rt_cl_free_kernel(&pt->kernel);
	rt_cl_free_kernel(&pt->mouse_kernel);
	rt_cl_free(&pt->info);
	clReleaseMemObject(pt->scene.objs_mem);
	clReleaseMemObject(pt->textures_mem);
	clReleaseMemObject(pt->seeds);
	clReleaseMemObject(pt->colors);
	clReleaseMemObject(pt->pixels_mem);
	clReleaseMemObject(pt->tex_size_mem);
	clReleaseMemObject(pt->mouse_intersect);
	free(pt->sdl.pixels);
	SDL_Quit();
}
