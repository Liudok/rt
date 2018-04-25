/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_camera.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ftymchyn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/19 18:25:48 by ftymchyn          #+#    #+#             */
/*   Updated: 2018/04/19 18:25:52 by ftymchyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	update_camera(t_rt *rt)
{
	t_camera	*c;

	rt->samples = 0; 
	c = &rt->scene.camera;
	if (rt->event.w)
		c->origin = vadd(c->origin, vmul(normalize(c->dir), 0.2));
	if (rt->event.s)
		c->origin = vsub(c->origin, vmul(normalize(c->dir), 0.2));
	if (rt->event.d)
		c->origin = vadd(c->origin, vmul(normalize(c->cx), 0.2));
	if (rt->event.a)
		c->origin = vsub(c->origin, vmul(normalize(c->cx), 0.2));
	if (rt->event.q)
		c->origin = vadd(c->origin, vmul(normalize(c->cy), 0.2));
	if (rt->event.e)
		c->origin = vsub(c->origin, vmul(normalize(c->cy), 0.2));
	if (rt->event.up)
		c->rotate.x += 1;
	if (rt->event.down)
		c->rotate.x -= 1;
	if (rt->event.left)
		c->rotate.y -= 1;
	if (rt->event.right)
		c->rotate.y += 1;
	if (rt->event.up || rt->event.down || rt->event.left || rt->event.right)
		rotate_camera(rt);
	clSetKernelArg(rt->kernel.kernel, 2, sizeof(t_camera), &rt->scene.camera);
}