/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   panel.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lberezyn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/20 15:07:11 by lberezyn          #+#    #+#             */
/*   Updated: 2018/05/13 16:52:31 by skamoza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/panels.h"

static inline void	render_single_button(t_rt *s, int i)
{
	SDL_RenderCopy(s->sdl.renderer, s->buttons[i].txt,
			NULL, &s->buttons[i].rect);
}

void				render_buttons(t_rt *s)
{
	int				i;
	SDL_Texture		*rama;
	SDL_Rect		ram;

	i = 0;
	rama = s->textures[2];
	while (i < 4)
	{
		if (!s->buttons[i].pressed)
			render_single_button(s, i);
		else
		{
			ram.x = s->buttons[i].rect.x - 5;
			ram.y = s->buttons[i].rect.y - 5;
			ram.w = s->buttons[i].rect.w + 10;
			ram.h = s->buttons[i].rect.h + 10;
			render_single_button(s, i);
			SDL_RenderCopy(s->sdl.renderer, rama, NULL, &ram);
		}
		if (i == 2 && s->buttons[i].pressed)
			after_effect_panel(s);
		else if (i == 1 && s->buttons[i].pressed)
			create_new_scene(s);
		i++;
	}
}

void				set_panel(t_rt *s)
{
	SDL_Texture	*bg;
	SDL_Rect	background_pos;

	bg = s->textures[1];
	background_pos.x = 0;
	background_pos.y = 0;
	background_pos.w = 190;
	background_pos.h = 70;
	SDL_RenderCopy(s->sdl.renderer, bg, NULL, &background_pos);
	render_buttons(s);
}

inline static void	render(t_rt *s, unsigned timeout)
{
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), timeout))
	{
		clSetKernelArg(s->kernel.kernel, 5, sizeof(cl_uint), &s->samples);
		rt_cl_push_task(&s->kernel, &s->job_size);
		rt_cl_join(&s->info);
		s->samples++;
	}
}

void				set_bg(t_rt *s)
{
	unsigned timeout;

	ft_putstr_fd(" samples per pixel -> ", 2);
	ft_putnbr_fd(s->samples, 2);
	ft_putstr_fd("\r", 2);
	timeout = SDL_GetTicks() + 17;
	if (s->samples < 30)
		render(s, timeout);
	else
		render(s, timeout + 150);
	clSetKernelArg(s->effect_kernel.kernel, 2, sizeof(cl_int), &s->effect_type);
	rt_cl_push_task(&s->effect_kernel, &s->job_size);
	rt_cl_device_to_host(&s->info, s->pixels_mem,
			s->sdl.pixels, s->job_size * sizeof(int));
	SDL_UpdateTexture(s->sdl.canvas, NULL, s->sdl.pixels, s->sdl.win_w << 2);
	SDL_RenderClear(s->sdl.renderer);
	SDL_RenderCopy(s->sdl.renderer, s->sdl.canvas, NULL, NULL);
	set_panel(s);
	SDL_RenderPresent(s->sdl.renderer);
}
