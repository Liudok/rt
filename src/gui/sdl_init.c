/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sdl_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lberezyn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/23 12:20:12 by lberezyn          #+#    #+#             */
/*   Updated: 2018/05/10 16:01:19 by skamoza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/panels.h"
#define TP "src/img/"
#define TE ".jpg"

int		load_all_textures(t_rt *s)
{
	static const char	*tex_names[] = {TP"earth"TE, TP"earth_normal"TE, TP
	"earth_light"TE, TP"earth_clouds"TE, TP"brickwall"TE, TP"brickwall_normal"
	TE, TP"TennisBallColor"TE, TP"TennisBallNormal"TE, TP"BeachBallColor"TE,
	TP"BeachBallTransp"TE, TP"SoftballColor"TE, TP"SoftballNormal"TE,
	TP"processor"TE, TP"BallCue"TE, TP"Ball1"TE, TP"Ball2"TE, TP"Ball3"TE, TP
	"Ball4"TE, TP"Ball5"TE, TP"Ball6"TE, TP"Ball7"TE, TP"Ball8"TE, TP"Ball9"TE,
	TP"Ball10"TE, TP"Ball11"TE, TP"Ball12"TE, TP"Ball13"TE, TP"Ball14"TE,
	TP"BasketballColor"TE};
	int					i;

	i = -1;
	if (!(s->sdl.canvas = SDL_CreateTexture(s->sdl.renderer,
		SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET,
		s->sdl.win_w, s->sdl.win_h)))
	{
		ft_putendl_fd("failed to initiatetexture in SDL", 2);
		return (0);
	}
	s->textures[0] = load_texture("src/img/dark_pixel2.png", s);
	s->textures[1] = load_texture("src/img/dark_pixel.png", s);
	s->textures[2] = load_texture("src/img/rama.png", s);
	while (++i < NUM_TEX)
		if (!(s->textures_img[i] = IMG_Load(tex_names[i])))
			return (0);
	return (1);
}

int		sdl_init_everything(t_rt *rt)
{
	if (SDL_Init(SDL_INIT_VIDEO) == -1 || !(rt->sdl.win = SDL_CreateWindow(
		"RT", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, rt->sdl.win_w,
		rt->sdl.win_h, SDL_WINDOW_RESIZABLE)) || !(rt->sdl.renderer =
		SDL_CreateRenderer(rt->sdl.win, -1, SDL_RENDERER_ACCELERATED)))
	{
		ft_putendl_fd("failed to initiate SDL", 2);
		return (0);
	}
	if (!load_all_textures(rt))
		return (0);
	SDL_RenderSetLogicalSize(rt->sdl.renderer, rt->sdl.win_w, rt->sdl.win_h);
	rt->sdl.pixels = malloc(sizeof(int) * (rt->sdl.win_w * rt->sdl.win_h));
	create_buttons(rt);
	return (1);
}

void	run_ui(t_rt *s)
{
	int			running;
	SDL_Event	evt;

	running = 1;
	init_opencl(s);
	while (running)
	{
		while (SDL_PollEvent(&evt))
			if (is_quit(evt))
				running = 0;
			else if (is_window_resizable(evt))
				handling_window_resizable(s, evt);
			else if (evt.type == SDL_KEYDOWN)
				on_event(s, &evt);
			else if (evt.type == SDL_KEYUP)
				off_event(s, &evt);
			else if (evt.type == SDL_MOUSEBUTTONDOWN)
				check_pressing(s, evt.button.x, evt.button.y);
		if (is_camera_event(s))
			update_camera(s);
		set_bg(s);
		render_buttons(s);
	}
}
