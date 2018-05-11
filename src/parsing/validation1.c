/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lberezyn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/11 20:55:08 by lberezyn          #+#    #+#             */
/*   Updated: 2018/05/11 20:55:11 by lberezyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parser.h"

void 	validation_sphere(t_sphere *o)
{
	if (o->origin.s0 > MAXF || o->origin.s0 < MINF ||
		o->origin.s1 > MAXF || o->origin.s1 < MINF ||
		o->origin.s2 > MAXF || o->origin.s2 < MINF)
	{
		ft_putendl_fd("ORIGIN of sphere is invalid", 2);
	}
	o->origin.s0 = (o->origin.s0 > MAXF) ? MAXF : o->origin.s0;
	o->origin.s1 = (o->origin.s1 > MAXF) ? MAXF : o->origin.s1;
	o->origin.s2 = (o->origin.s2 > MAXF) ? MAXF : o->origin.s2;
	o->origin.s0 = (o->origin.s0 < MINF) ? MINF : o->origin.s0;
	o->origin.s1 = (o->origin.s1 < MINF) ? MINF : o->origin.s1;
	o->origin.s2 = (o->origin.s2 < MINF) ? MINF : o->origin.s2;
	if (o->radius > MAXF || o->radius < MINF)
	{
		ft_putendl_fd("Invalid radius of sphere", 2);
		o->radius = 5;
		o->r2 = o->radius * o->radius;
	}
}

void 	validation_plane(t_plane *o)
{
	if (o->origin.s0 > MAXF || o->origin.s0 < MINF ||
		o->origin.s1 > MAXF || o->origin.s1 < MINF ||
		o->origin.s2 > MAXF || o->origin.s2 < MINF)
	{
		ft_putendl_fd("ORIGIN of plane is invalid", 2);
	}
	o->origin.s0 = (o->origin.s0 > MAXF) ? MAXF : o->origin.s0;
	o->origin.s1 = (o->origin.s1 > MAXF) ? MAXF : o->origin.s1;
	o->origin.s2 = (o->origin.s2 > MAXF) ? MAXF : o->origin.s2;
	o->origin.s0 = (o->origin.s0 < MINF) ? MINF : o->origin.s0;
	o->origin.s1 = (o->origin.s1 < MINF) ? MINF : o->origin.s1;
	o->origin.s2 = (o->origin.s2 < MINF) ? MINF : o->origin.s2;
	if (o->normal.s0 > MAXF || o->normal.s0 < MINF ||
		o->normal.s1 > MAXF || o->normal.s1 < MINF ||
		o->normal.s2 > MAXF || o->normal.s2 < MINF)
	{
		ft_putendl_fd("NORMAL of plane is invalid", 2);
	}
	o->normal.s0 = (o->normal.s0 > 1) ? 1 : o->normal.s0;
	o->normal.s1 = (o->normal.s1 > 1) ? 1 : o->normal.s1;
	o->normal.s2 = (o->normal.s2 > 1) ? 1 : o->normal.s2;
	o->normal.s0 = (o->normal.s0 < 0) ? 0 : o->normal.s0;
	o->normal.s1 = (o->normal.s1 < 0) ? 1 : o->normal.s1;
	o->normal.s2 = (o->normal.s2 < 0) ? 0 : o->normal.s2;
}

void 	validation_cylinder(t_cylinder *o)
{
	if (o->origin.s0 > MAXF || o->origin.s0 < MINF ||
		o->origin.s1 > MAXF || o->origin.s1 < MINF ||
		o->origin.s2 > MAXF || o->origin.s2 < MINF)
	{
		ft_putendl_fd("ORIGIN of cylinder is invalid", 2);
	}
	o->origin.s0 = (o->origin.s0 > MAXF) ? MAXF : o->origin.s0;
	o->origin.s1 = (o->origin.s1 > MAXF) ? MAXF : o->origin.s1;
	o->origin.s2 = (o->origin.s2 > MAXF) ? MAXF : o->origin.s2;
	o->origin.s0 = (o->origin.s0 < MINF) ? MINF : o->origin.s0;
	o->origin.s1 = (o->origin.s1 < MINF) ? MINF : o->origin.s1;
	o->origin.s2 = (o->origin.s2 < MINF) ? MINF : o->origin.s2;
	if (o->normal.s0 > 1 || o->normal.s0 < 0 ||
		o->normal.s1 > 1 || o->normal.s1 < 0 ||
		o->normal.s2 > 1 || o->normal.s2 < 0)
	{
		ft_putendl_fd("NORMAL of cylinder is invalid", 2);
	}
	o->normal.s0 = (o->normal.s0 > 1) ? 1 : o->normal.s0;
	o->normal.s1 = (o->normal.s1 > 1) ? 1 : o->normal.s1;
	o->normal.s2 = (o->normal.s2 > 1) ? 1 : o->normal.s2;
	o->normal.s0 = (o->normal.s0 < 0) ? 0 : o->normal.s0;
	o->normal.s1 = (o->normal.s1 < 0) ? 1 : o->normal.s1;
	o->normal.s2 = (o->normal.s2 < 0) ? 0 : o->normal.s2;
	if (o->radius > MAXF || o->radius < MINF)
	{
		ft_putendl_fd("Invalid radius of cylinder", 2);
		o->radius = 2;
		o->r2 = o->radius * o->radius;
	}
	if (o->height > MAXF || o->height < MINF)
	{
		ft_putendl_fd("Invalid height of cylinder", 2);
		o->height = 2;
	}
}

void 	validation_cone(t_cone *o)
{
	if (o->origin.s0 > MAXF || o->origin.s0 < MINF ||
		o->origin.s1 > MAXF || o->origin.s1 < MINF ||
		o->origin.s2 > MAXF || o->origin.s2 < MINF)
	{
		ft_putendl_fd("ORIGIN of cone is invalid", 2);
	}
	o->origin.s0 = (o->origin.s0 > MAXF) ? MAXF : o->origin.s0;
	o->origin.s1 = (o->origin.s1 > MAXF) ? MAXF : o->origin.s1;
	o->origin.s2 = (o->origin.s2 > MAXF) ? MAXF : o->origin.s2;
	o->origin.s0 = (o->origin.s0 < MINF) ? MINF : o->origin.s0;
	o->origin.s1 = (o->origin.s1 < MINF) ? MINF : o->origin.s1;
	o->origin.s2 = (o->origin.s2 < MINF) ? MINF : o->origin.s2;
	if (o->normal.s0 > 1 || o->normal.s0 < 0 ||
		o->normal.s1 > 1 || o->normal.s1 < 0 ||
		o->normal.s2 > 1 || o->normal.s2 < 0)
	{
		ft_putendl_fd("NORMAL of cone is invalid", 2);
	}
	o->normal.s0 = (o->normal.s0 > 1) ? 1 : o->normal.s0;
	o->normal.s1 = (o->normal.s1 > 1) ? 1 : o->normal.s1;
	o->normal.s2 = (o->normal.s2 > 1) ? 1 : o->normal.s2;
	o->normal.s0 = (o->normal.s0 < 0) ? 0 : o->normal.s0;
	o->normal.s1 = (o->normal.s1 < 0) ? -1 : o->normal.s1;
	o->normal.s2 = (o->normal.s2 < 0) ? 0 : o->normal.s2;
	if (o->half_tangent > MAXF || o->half_tangent < 0)
	{
		ft_putendl_fd("Invalid half_tangent of cone", 2);
		o->half_tangent = 0.5;
	}
	if (o->m1 > MAXF || o->m1 < MINF)
	{
		ft_putendl_fd("Invalid m1 of cone", 2);
		o->m1 = 2;
	}
	if (o->m2 > MAXF || o->m2 < MINF)
	{
		ft_putendl_fd("Invalid m2 of cone", 2);
		o->m2 = 4;
	}
}

void 	validation_disk(t_disk *o)
{
	if (o->origin.s0 > MAXF || o->origin.s0 < MINF ||
		o->origin.s1 > MAXF || o->origin.s1 < MINF ||
		o->origin.s2 > MAXF || o->origin.s2 < MINF)
	{
		ft_putendl_fd("ORIGIN of disk is invalid", 2);
	}
	o->origin.s0 = (o->origin.s0 > MAXF) ? MAXF : o->origin.s0;
	o->origin.s1 = (o->origin.s1 > MAXF) ? MAXF : o->origin.s1;
	o->origin.s2 = (o->origin.s2 > MAXF) ? MAXF : o->origin.s2;
	o->origin.s0 = (o->origin.s0 < MINF) ? MINF : o->origin.s0;
	o->origin.s1 = (o->origin.s1 < MINF) ? MINF : o->origin.s1;
	o->origin.s2 = (o->origin.s2 < MINF) ? MINF : o->origin.s2;
	if (o->normal.s0 > 1 || o->normal.s0 < 0 ||
		o->normal.s1 > 1 || o->normal.s1 < 0 ||
		o->normal.s2 > 1 || o->normal.s2 < 0)
	{
		ft_putendl_fd("NORMAL of disk is invalid", 2);
	}
	o->normal.s0 = (o->normal.s0 > 1) ? 1 : o->normal.s0;
	o->normal.s1 = (o->normal.s1 > 1) ? 1 : o->normal.s1;
	o->normal.s2 = (o->normal.s2 > 1) ? 1 : o->normal.s2;
	o->normal.s0 = (o->normal.s0 < 0) ? 0 : o->normal.s0;
	o->normal.s1 = (o->normal.s1 < 0) ? 1 : o->normal.s1;
	o->normal.s2 = (o->normal.s2 < 0) ? 0 : o->normal.s2;
	if (o->radius2 > MAXF || o->radius2 < MINF)
	{
		ft_putendl_fd("Invalid radius of disk", 2);
		o->radius2 = 5;
	}
}

void 	validation_torus(t_torus *o)
{
	if (o->origin.s0 > MAXF || o->origin.s0 < MINF ||
		o->origin.s1 > MAXF || o->origin.s1 < MINF ||
		o->origin.s2 > MAXF || o->origin.s2 < MINF)
	{
		ft_putendl_fd("ORIGIN of torus is invalid", 2);
	}
	o->origin.s0 = (o->origin.s0 > MAXF) ? MAXF : o->origin.s0;
	o->origin.s1 = (o->origin.s1 > MAXF) ? MAXF : o->origin.s1;
	o->origin.s2 = (o->origin.s2 > MAXF) ? MAXF : o->origin.s2;
	o->origin.s0 = (o->origin.s0 < MINF) ? MINF : o->origin.s0;
	o->origin.s1 = (o->origin.s1 < MINF) ? MINF : o->origin.s1;
	o->origin.s2 = (o->origin.s2 < MINF) ? MINF : o->origin.s2;
	if (o->normal.s0 > 1 || o->normal.s0 < 0 ||
		o->normal.s1 > 1 || o->normal.s1 < 0 ||
		o->normal.s2 > 1 || o->normal.s2 < 0)
	{
		ft_putendl_fd("NORMAL of torus is invalid", 2);
	}
	o->normal.s0 = (o->normal.s0 > 1) ? 1 : o->normal.s0;
	o->normal.s1 = (o->normal.s1 > 1) ? 1 : o->normal.s1;
	o->normal.s2 = (o->normal.s2 > 1) ? 1 : o->normal.s2;
	o->normal.s0 = (o->normal.s0 < 0) ? 0 : o->normal.s0;
	o->normal.s1 = (o->normal.s1 < 0) ? 1 : o->normal.s1;
	o->normal.s2 = (o->normal.s2 < 0) ? 0 : o->normal.s2;
	if (o->big_radius2 > MAXF || o->big_radius2 < MINF)
	{
		ft_putendl_fd("Invalid radius of torus", 2);
		o->big_radius2 = 5;
	}
	if (o->small_radius2 > MAXF || o->small_radius2 < MINF)
	{
		ft_putendl_fd("Invalid radius of torus", 2);
		o->small_radius2 = 5;
	}
}

void 	validation_triangle(t_triangle *o)
{
	if (o->vertex0.s0 > MAXF || o->vertex0.s0 < MINF ||
		o->vertex0.s1 > MAXF || o->vertex0.s1 < MINF ||
		o->vertex0.s2 > MAXF || o->vertex0.s2 < MINF ||
		o->vertex1.s0 > MAXF || o->vertex1.s0 < MINF ||
		o->vertex1.s1 > MAXF || o->vertex1.s1 < MINF ||
		o->vertex1.s2 > MAXF || o->vertex1.s2 < MINF ||
		o->vertex2.s0 > MAXF || o->vertex2.s0 < MINF ||
		o->vertex2.s1 > MAXF || o->vertex2.s1 < MINF ||
		o->vertex2.s2 > MAXF || o->vertex2.s2 < MINF)
	{
		ft_putendl_fd("triangle is invalid", 2);
	}
	o->vertex0.s0 = (o->vertex0.s0 > MAXF) ? MAXF : o->vertex0.s0;
	o->vertex0.s1 = (o->vertex0.s1 > MAXF) ? MAXF : o->vertex0.s1;
	o->vertex0.s2 = (o->vertex0.s2 > MAXF) ? MAXF : o->vertex0.s2;
	o->vertex0.s0 = (o->vertex0.s0 < MINF) ? MINF : o->vertex0.s0;
	o->vertex0.s1 = (o->vertex0.s1 < MINF) ? MINF : o->vertex0.s1;
	o->vertex0.s2 = (o->vertex0.s2 < MINF) ? MINF : o->vertex0.s2;
	o->vertex1.s0 = (o->vertex1.s0 > MAXF) ? MAXF : o->vertex1.s0;
	o->vertex1.s1 = (o->vertex1.s1 > MAXF) ? MAXF : o->vertex1.s1;
	o->vertex1.s2 = (o->vertex1.s2 > MAXF) ? MAXF : o->vertex1.s2;
	o->vertex1.s0 = (o->vertex1.s0 < MINF) ? MINF : o->vertex1.s0;
	o->vertex1.s1 = (o->vertex1.s1 < MINF) ? MINF : o->vertex1.s1;
	o->vertex1.s2 = (o->vertex1.s2 < MINF) ? MINF : o->vertex1.s2;
	o->vertex2.s0 = (o->vertex2.s0 > MAXF) ? MAXF : o->vertex2.s0;
	o->vertex2.s1 = (o->vertex2.s1 > MAXF) ? MAXF : o->vertex2.s1;
	o->vertex2.s2 = (o->vertex2.s2 > MAXF) ? MAXF : o->vertex2.s2;
	o->vertex2.s0 = (o->vertex2.s0 < MINF) ? MINF : o->vertex2.s0;
	o->vertex2.s1 = (o->vertex2.s1 < MINF) ? MINF : o->vertex2.s1;
	o->vertex2.s2 = (o->vertex2.s2 < MINF) ? MINF : o->vertex2.s2;

}