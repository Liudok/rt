/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_quit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ftymchyn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/19 15:37:10 by ftymchyn          #+#    #+#             */
/*   Updated: 2018/04/19 15:37:11 by ftymchyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include "../../frameworks/SDL2.framework/Versions/A/Headers/SDL_events.h"

int	is_quit(SDL_Event e)
{
	return (e.type == SDL_QUIT || (e.type == SDL_WINDOWEVENT &&
			e.window.event == SDL_WINDOWEVENT_CLOSE) ||
			(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE));
}
