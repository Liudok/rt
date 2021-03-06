static float3 julia_normal(float4 qP)
{
	return (fast_normalize(qP.xyz));
}

static float3	sphere_normal(global t_sphere *obj, float3 pos)
{
	return (normalize(pos - obj->origin));
}

static float3	plane_normal(global t_plane *obj)
{
	return (fast_normalize(obj->normal));
}

static float3	cylinder_normal(global t_cylinder *obj, float3 pos, float m)
{
	return (normalize(pos - obj->origin - obj->normal * m));
}

static float3	cone_normal(global t_cone *obj, float3 pos, float m)
{
	return (normalize(pos - obj->origin -
			(1 + obj->half_tangent * obj->half_tangent) * obj->normal * m));
}

static float3	disk_normal(global t_disk *obj)
{
	return (fast_normalize(obj->normal));
}

static float3	torus_normal(global t_torus *obj, float3 pos)
{
	float3	cent;
	float	k;
	float3	a;

	cent = pos - obj->origin;
	k = dot(normalize(obj->normal), cent);
	a = normalize(obj->normal) * k;
	a = cent - a;
	k = dot(a, a);
	k = 1 / sqrt(k) * sqrt(obj->big_radius2);
	cent = pos - a - (obj->origin - a) * k / (sqrt(obj->big_radius2) + k);
	return (normalize(cent));
}

static float3	triangle_normal(global t_triangle *obj)
{
	return (normalize(
		cross(obj->vertex1 - obj->vertex0, obj->vertex2 - obj->vertex1)));
}

static float3	paraboloid_normal(global t_parabaloid *obj, float3 pos, float m)
{
    float x = pos.x;
    float y = pos.y;
    float z = pos.z;
    float R = obj->radius;
    float3 ret =  {2 * x * y - 2 * R * z - 4 * x * z,
		-R * R + x * x + 3 * y * y - 4 * y * z + z * z,
		-2 * R * x - 2 * x * x - 2 * y * y + 2 * y * z};
		return (ret);
}

static float3	mobius_normal(float3 p)
{
   float3 ret;

   ret.x = 2.0 * p.x * p.y - 2.0 * p.z - 4.0 * p.x * p.z;
   ret.y = -1 + p.x * p.x + 3.0 * p.y * p.y - 4.0 * p.y * p.z + p.z * p.z;
   ret.z = -2.0 * p.x - 2.0 * p.x * p.x - 2.0 * p.y * p.y + 2.0 * p.y * p.z;
   return (normalize(ret));
}

static float3	cube_normal(global t_cube *obj, float3 pos, float m)
{
    float3 	normal = {0, 0, 0};
    normal[(int)m] = 1;
    return (normal);
}

float3	find_normal(global t_object *obj, float3 hit_pos, float m)
{
	switch (obj->type) {
		case sphere:
		case bool_substraction:
		case bool_intersection:
		case second:
			return (sphere_normal(&obj->prim.sphere, hit_pos));
		case plane:
			return (plane_normal(&obj->prim.plane));
		case cylinder:
			return (cylinder_normal(&obj->prim.cylinder, hit_pos, m));
		case cone:
			return (cone_normal(&obj->prim.cone, hit_pos, m));
		case disk:
			return (disk_normal(&obj->prim.disk));
        case torus:
            return (torus_normal(&obj->prim.torus, hit_pos));
		case triangle:
			return (triangle_normal(&obj->prim.triangle));
        case mobius:
            return (mobius_normal(hit_pos));
        case cube:
            return (cube_normal(&obj->prim.cube, hit_pos, m));
        case parabaloid:
            return (sphere_normal(&obj->prim.sphere, hit_pos));
        case julia:
            return (julia_normal((float4)(hit_pos, 1.f)).xyz);
		default:
			break;
	}
	return ((float3)(0,0,0));
}

static t_material  get_material(global t_object *obj, uint *seeds)
{
	if (obj->material == emission || obj->material == diffuse)
		return (obj->material);
	return (get_random(&seeds[0], &seeds[1]) < obj->roughness ?
			diffuse :
			obj->material);
}

static void map_normal(read_only image2d_array_t textures,
						 t_surface *surf, uint2 size)
{
	float3 map_n = get_texel(textures, surf, surf->obj->texture.y, size);
	float3 t = cross(surf->nl, (float3)(0.f, 1.f, 0.f));
	if (fast_length(t) == 0.f)
		t = cross(surf->nl, (float3)(0.f, 0.f, 1.f));
	t = normalize(t);
	float3 b = cross(surf->nl, t);
	surf->nl = normalize(t * map_n.x
						+ b * map_n.y
						+ surf->nl * map_n.z);
}

static float3 map_light(read_only image2d_array_t textures,
						 t_surface *surf, uint2 size)
{
	return get_texel(textures, surf, surf->obj->texture.z, size);
}

static void map_transparent(read_only image2d_array_t textures,
						 t_surface *surf, uint2 size, uint *seeds)
{
	float3 texel = get_texel(textures, surf, surf->obj->texture.w, size);
	if (get_random(seeds, seeds + 1) > texel.x * texel.y * texel.z)
		surf->material = transparent;
}

static float3 apply_textures(t_surface *surf, read_only image2d_array_t textures,
		global uint2 *sizes, uint *seeds)
{
	uchar4 tex = surf->obj->texture;
	float3 color = surf->obj->color;
	surf->uv = get_tex_coords(surf);

	if (tex.x == 253)
		color *= sin(surf->nl * .48f + .5f);
	else if (tex.x == 254)
		color *= perlin_noise(surf->uv * (float2)(4048.f, 4048.f));
	else if (tex.x == 255)
		color = surf->nl * .48f + .5f;
	else if (tex.x && tex.x <= NUM_TEX)
		color = get_texel(textures, surf, tex.x, sizes[tex.x]);
	if (tex.y == 253)
		surf->nl = normalize((float3)(sin(surf->uv * 512.f), 1));
	else if (tex.y && tex.y <= NUM_TEX) //normal_map
		map_normal(textures, surf, sizes[tex.y]);
	if (tex.z && tex.z <= NUM_TEX) //light map
		surf->emission = map_light(textures, surf, sizes[tex.z]) * color;
	if (tex.w && tex.w <= NUM_TEX) //transparency map
		map_transparent(textures, surf, sizes[tex.w], seeds);
	if (tex.z == 254)
		surf->emission = color * perlin_noise(surf->uv * (float2)(4048.f, 4048.f));
	else if (tex.z == 255)
		surf->emission = color * surf->nl * .49f + .5f;
	return (color);
}

static t_surface   get_surface_properties(global t_object *obj, t_ray r, float t,
		float m, read_only image2d_array_t textures, global uint2 *sizes, uint* seeds)
{
    t_surface   s;

	s.emission = 0;
    s.obj = obj;
    s.pos = r.o + r.d * t;
    s.m = m;
    s.n = find_normal(obj, s.pos, s.m);
    s.nl = (dot(s.n, r.d) < 0) ? s.n : -s.n;
    s.material = get_material(obj, seeds);
	s.ref = apply_textures(&s, textures, sizes, seeds);
    s.maxref = fmax(fmax(s.ref.x, s.ref.y), s.ref.z);
    s.maxref -= (s.maxref > 0.75f) ? (s.maxref - 0.75f) * 0.1f : 0;
    s.ior = obj->ior;
    return (s);
}

static t_ray   diffuse_reflection(t_surface surf, uint *seeds)
{
    float 	r1, r2, r2s;
    float3	w, u, v, d;

    r1 = 2 * M_PI * get_random(&seeds[0], &seeds[1]);
    r2 = get_random(&seeds[0], &seeds[1]);
    r2s = native_sqrt(r2);

	w = surf.nl;
	u = normalize(cross((fabs(w[0]) > .1f ?
					(float3)(0, 1, 0) : (float3)(1, 0, 0)), w));
	v = cross(w, u);

	u = u * native_cos(r1) * r2s;
	v = v * native_sin(r1) * r2s;
	w = w * native_sqrt(1 - r2);

	d = normalize(u + v + w);
    t_ray	rand_ray = (t_ray){ surf.pos, d };
	return (rand_ray);
}

static t_ray   specular_reflection(t_surface surf, t_ray r)
{
    t_ray spec;

    spec.o = surf.pos;
    spec.d = normalize(r.d - surf.nl * 2 * dot(r.d, surf.nl));
    return (spec);
}

static t_ray   glass_refraction(t_surface surf, t_ray r, uint *seeds,  float3 *accum_ref)
{
	t_ray   refracted_ray;
    t_ray	reflected_ray = specular_reflection(surf, r);
    uchar	into = dot(surf.n, surf.nl) > 0;
    float	nc = 1;
    float	nnt = into ? nc / surf.ior : surf.ior / nc;
    float	ddn = dot(r.d, surf.nl);
    float 	cos2t = 1 - nnt * nnt * (1 - ddn * ddn);

    if (cos2t < 0)
    	return (reflected_ray);

    refracted_ray.o = surf.pos;
    refracted_ray.d = 
    	normalize((r.d * nnt - surf.n * ((into ? 1 : -1) * (ddn * nnt + sqrt(cos2t)))));

    float 	a = surf.ior - nc;
    float 	b = surf.ior + nc;
    float 	c = 1 - (into ? -ddn : dot(refracted_ray.d, surf.n));
    float 	R0 = a * a / (b * b);
    float 	Rfres = R0 + (1 - R0) * c * c * c * c * c;
    float 	P = 0.25 + 0.5 * Rfres;
    float 	RP = Rfres / P;
    float 	TP = (1 - Rfres) / (1 - P);

    if (get_random(&seeds[0], &seeds[1]) < P)
    {
    	*accum_ref = *accum_ref * RP; 
    	return (reflected_ray);
    }
    *accum_ref = *accum_ref * TP;
    return (refracted_ray);
}
