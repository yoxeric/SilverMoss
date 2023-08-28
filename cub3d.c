/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azaghlou <azaghlou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 20:05:54 by yhachami          #+#    #+#             */
/*   Updated: 2023/08/28 16:51:17 by azaghlou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"cub3d.h"

float	dst(t_vector2f a, t_vector2f b)
{
	// ok
	return (sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y)));
}

float	circle(float fish)
{
	if (fish >= 360)
		fish -= 360;
	if (fish < 0)
		fish += 360;
	return (fish);
}

// void	read_map(t_game *game, char **av)
// {	
// 	int		file;
// 	char	*line;
// 	int		x;
// 	int		y;
// 	int		z;

// 	game->fov = 60;
// 	game->column_size = 1;
// 	//game->column_size = WIDTH / game->fov;
// 	game->tile_size = 50;
// 	// tex
// 	game->map.tex[0] = mlx_load_png("./vega.png");
// 	game->map.tex[1] = mlx_load_png("./tex2.png");
// 	game->map.tex[2] = mlx_load_png("./tex3.png");
// 	game->map.tex[3] = mlx_load_png("./tex4.png");
// 	// sky and floor color
// 	game->map.col[0] = 0x2E1A47FF;
// 	game->map.col[1] = 0x1E3226FF;
// 	// read map.cub file
// 	game->map.size.x = 50;
// 	game->map.size.y = 50;
// 	x = 50;
// 	y = 50;
// 	game->map.map = (int **) malloc(y * sizeof(int *));
// 	z = -1;
// 	while (++z < y)
// 		game->map.map[z] = (int *) malloc(x * sizeof(int));
// 	y = -1;
// 	while (++y < 50)
// 	{
// 		x = -1;
// 		while (++x < 50)
// 			game->map.map[x][y] = -1;
// 	}
// 	file = open(av[1], O_RDONLY);
// 	line = get_next_line(file);
// 	y = 0;
// 	while (line)
// 	{
// 		printf("%s", line);
// 		x = -1;
// 		while (line[++x] && line[x] != '\n')
// 		{
// 			game->map.map[x][y] = line[x] - '0';
// 			if (line[x] == 'N')
// 			{
// 				game->map.map[x][y] = 0;
// 				game->player.pos.x = (float) x * game->tile_size;
// 				game->player.pos.y = (float) y * game->tile_size;
// 				game->player.rot = 270;
// 			}
// 		}
// 		y++;
// 		line = get_next_line(file);
// 	}
// 	printf("player (%f, %f)\n", game->player.pos.x, game->player.pos.y);
// }

void    draw_line1(mlx_image_t *img, t_vector2f p0, t_vector2f p1)
{
        t_vector3f      v;
        t_vector3f      d;
        int             i;

        v.x = p0.x;
        v.y = p0.y;
        d.x = p1.x - p0.x;
        d.y = p1.y - p0.y;
        d.z = d.y / d.x;
        if (fabs(d.x) >= fabs(d.y))
			d.z = fabs(d.x);
        else
			d.z = fabs(d.y);
        d.x = d.x / d.z;
        d.y = d.y / d.z;
        i = 1;
        while (i <= d.z)
        {
			if (v.x >= 0 && v.y >= 0 && v.x < img->width && v.y < img->height) 
				mlx_put_pixel(img, v.x, v.y, 0xff0000ff);
			v.x = v.x + d.x;
			v.y = v.y + d.y;
			i++;
        }
}

void	draw_cube(mlx_image_t *img, t_vector2i start, t_vector2i end, int color)
{
	t_vector2i	v;

	v.x = start.x;
	v.y = start.y;
	while (v.x < end.x)
	{
		if (v.x >= 0 && v.y >= 0 && v.x < img->width && v.y < img->height) 
			mlx_put_pixel(img, v.x, v.y, color);
		v.y++;
		if (v.y == end.y)
		{
			v.x++;
			v.y = start.y;
		}
	}
}

void	draw_rays(t_game *game, t_vector2f ray)
{
	int			s;
	t_vector2f	pp;
	t_vector2f	r;

	s = 5;
	r.x = ray.x / s;
	r.y = ray.y / s;
	pp.x = game->player.pos.x / s;
	pp.y = game->player.pos.y / s;
	if (r.x > 0 && r.y > 0 && r.x < WIDTH && r.y < HEIGHT)
		draw_line1(game->map.img, pp, r);
}

bool draw_texture(t_game *game, uint32_t h2, t_vector2i a, t_vector2i b, t_ray ray)
{
	mlx_image_t* image;
	mlx_texture_t* texture;
	uint8_t* pixelx;
	uint8_t* pixeli;
	uint32_t i;
	uint32_t j;
	uint32_t z;
	uint32_t z2;
	float	r;
	float	r2;
	float	rx;
	float	ry;
	int		h;

	image = game->img;
	texture = game->map.tex[0];
	//printf("xy = %d, %d\n", xy[0], xy[1]);

	//h = ray.dst;	
	h = (game->tile_size * HEIGHT) / ray.dst;
	r = (float) texture->height / h;
	r2 = (float) texture->width / h;
	//printf("r = %f, r2 = %f, h = %d\n", r, r2, h);
	
	j = 0;
	z2 = 0;
	while (j < game->column_size)
	{
		//rx = (float) z2 * r2;
		rx = 0;
		i = 0;
		z = 0;
		while (i < h2)
		{
			//ry = (float) z / r;
			ry = (float) z * r;
			pixelx = &texture->pixels[(((((int) ry) * texture->width) + (a.x + (int) rx))) * texture->bytes_per_pixel];
			pixeli = &image->pixels[((a.y + i) * image->width + (a.x + j)) * texture->bytes_per_pixel];
			ft_memmove(pixeli, pixelx, texture->bytes_per_pixel);
			i++;
			z++;
			if (ry > texture->height)
				z = 0;
		}
		j++;
		//z2++;	
		//if (rx > texture->width)
		//	z2 = 0;
	}
	return (true);
}

bool draw_texture2(t_game *game, uint32_t h2, t_vector2i a, t_ray ray)
{
	mlx_texture_t*	tex;
	uint8_t*		pixelx;
	uint8_t*		pixeli;
	uint32_t		i;
	uint32_t		j;
	uint32_t		z;
	float			r;
	float			r2;
	float			rx;
	float			ry;
	int				h;
	int				px;
	int				py;
	int				x;
	int				y;

	//h = (ray.dst / 2) + 200;
	//h = ray.dst;
	h = (game->tile_size * HEIGHT) / ray.dst;
	px = ray.ray.x - (ray.tile.x * game->tile_size);
	py = ray.ray.y - (ray.tile.y * game->tile_size);
	if (py == 0)
	{
		tex = game->map.tex[1];
		x = px;
	}
	else if (py == 49)
	{
		tex = game->map.tex[0];
		x = px;
	}
	else if (px == 0)
	{
		tex = game->map.tex[3];
		x = py;
	}
	else if (px == 49)
	{
		tex = game->map.tex[2];
		x = py;
	}
	y = 0;
	if (h > HEIGHT)
		y = (HEIGHT - h) / 2;

	r = (float) tex->height / h;
	r2 = (float) tex->width / game->tile_size;
	//printf("r = %f, r2 = %f, h = %d\n", r, r2, h);

	j = 0;
	while (j < game->column_size)
	{
		rx = (float) x * r2;
		i = 0;
		z = 0;
		while (i < h2)
		{
			ry = (float) (z - y) * r;
			if (ry > tex->height)
				ry = (float) z * r;
			pixelx = &tex->pixels[((((int) ry) * tex->width) + ((int) rx)) * tex->bytes_per_pixel];
			pixeli = &game->img->pixels[((a.y + i) * game->img->width + (a.x + j)) * tex->bytes_per_pixel];
			ft_memmove(pixeli, pixelx, tex->bytes_per_pixel);
			i++;
			z++;
			if (ry > tex->height)
				z = 0;
		}
		j++;
	}
	return (true);
}

int	draw_colum(t_game *game, int x, t_ray ray)
{
	t_vector2i	a;
	t_vector2i	b;
	int		c;
	int		s;
	int		h;
	int		o;
	int		h2;

	// column
	s = game->tile_size;
	c = game->column_size;
	h = (ray.dst / 2) + (5000 / ray.dst);
	if (h > HEIGHT)
		h = HEIGHT;
	a.x = x;
	a.y = h ;
	b.x = x + c;
	b.y = HEIGHT - h;
	h2 = b.y - h;
	if (b.y <= a.y)
		b.y = a.y + 1;
	if (h2 <= 1)
		h2 = 1;	
	//printf("h = %f, h2 = %d\n", h, h2);
	//printf("a = %d, %d\n", a.x, a.y);
	//printf("b = %d, %d\n", b.x, b.y);

	if (a.x > 1 && a.x < WIDTH - 1 && a.y > 1 && a.y < HEIGHT - 1
			&& b.x > 1 && b.x < WIDTH - 1 && b.y > 1 && b.y < HEIGHT - 1)
	{	
		// draw_cube(game->img, a, b, 0xCCBBAA);
		//draw_texture(game, h2, a, b, ray);
		draw_texture2(game, h2, a, ray);
	}
	return (0);
}

int	draw_colum2(t_game *game, int x, t_ray ray)
{
	t_vector2i	a;
	t_vector2i	b;
	int		c;
	int		s;
	int		h;
	int		o;
	int		h2;

	// column
	s = game->tile_size;
	c = game->column_size;
	h = (game->tile_size * HEIGHT) /ray.dst;
	if (h > HEIGHT)
		h = HEIGHT;
	a.x = x;
	a.y = (HEIGHT / 2) - (h / 2);
	b.x = x + c;
	b.y = (HEIGHT / 2) + (h / 2);
	h2 = h;

	if (a.y < 5)
		a.y = 5;
	if (b.y > HEIGHT - 5)
		b.y = HEIGHT - 5;
	if (a.x > 0 && a.x < WIDTH && a.y > 0 && a.y < HEIGHT
			&& b.x > 0 && b.x < WIDTH && b.y > 0 && b.y < HEIGHT)
	{	
		draw_cube(game->img, a, b, 0xFFFFFFFF);
		//draw_texture(game, wh, a, b, ray);
		// draw_texture2(game, h2, a, ray);
	}
	return (0);
}

void	horizon_ray(t_game *game, t_ray *ray)
{
	t_vector2f	p;
	int			ts;
	float		arctan;

	p = game->player.pos;
	ts = game->tile_size;
	arctan = -1 / tan(ray->angel * DR);
	if (ray->angel > 180)
	{
		ray->ray.y = ((int) p.y / ts) * ts - 0.001;
		ray->ray.x = (p.y - ray->ray.y) * arctan + p.x;
		ray->step.y = -ts;
		ray->step.x = -ray->step.y * arctan;
	}
	else if (ray->angel < 180)
	{
		ray->ray.y = ((int) p.y / ts) * ts + ts;
		ray->ray.x = (p.y - ray->ray.y) * arctan + p.x;
		ray->step.y = ts;
		ray->step.x = -ray->step.y * arctan;
	}
}

void	vertical_ray(t_game *game, t_ray *ray)
{
	t_vector2f	p;
	int			ts;
	float		arctan;

	p = game->player.pos;
	ts = game->tile_size;
	arctan = -tan(ray->angel * DR);
	if (ray->angel > 90 && ray->angel < 270)
	{
		ray->ray.x = ((int) p.x / ts) * ts - 0.001;
		ray->ray.y = (p.x - ray->ray.x) * arctan + p.y;
		ray->step.x = -ts;
		ray->step.y = -ray->step.x * arctan;
	}
	else if (ray->angel < 90 || ray->angel > 270)
	{
		ray->ray.x = ((int) p.x / ts) * ts + ts;
		ray->ray.y = (p.x - ray->ray.x) * arctan + p.y;
		ray->step.x = ts;
		ray->step.y = -ray->step.x * arctan;
	}
}

t_vector2f	cast_rays(t_game *game, int dof, t_ray *ray)
{
	t_vector2f	outray;
	t_vector2i	tile;
	int			ts;

	ts = game->tile_size;
	//outray = ray;
	while (dof > 0)
	{
		ray->tile.x = ray->ray.x / ts;
		ray->tile.y = ray->ray.y / ts;
		if (ray->tile.x >= 0 && ray->tile.y >= 0 && ray->tile.x < game->map.size.x && ray->tile.y < game->map.size.y
				&& game->map.map[ray->tile.y][ray->tile.x] == '1')
		{
			dof = 0;
			outray = ray->ray;
		}
		else
		{
			dof--;
			ray->ray.x += ray->step.x;
			ray->ray.y += ray->step.y;
		}
	}
	return (outray);
}

int		distance(t_game *game, t_vector2f hray, t_vector2f vray, t_ray *ray)
{
	t_vector2f	d;
	float		fish;

	fish = game->player.rot - ray->angel;
	d.x = dst(game->player.pos, hray);
	d.y = dst(game->player.pos, vray);
	if (d.x > d.y)
	{
		ray->dst = d.y;
		ray->ray = vray;
	}
	else
	{
		ray->dst = d.x;
		ray->ray = hray;
	}
	ray->tile.x = ray->ray.x / game->tile_size;
	ray->tile.y = ray->ray.y / game->tile_size;
	ray->dst = ray->dst * cos(fish * DR);
	draw_rays(game, ray->ray);
	return (ray->dst);
}

void	draw_walls(t_game *game)
{
	t_ray		ray;
	t_vector2f	vray;
	t_vector2f	hray;
	int			dof;
	int			c;

	c = 0;
	//c = WIDTH / 2;
	ray.angel_step = (float) game->fov / (WIDTH / game->column_size);
	ray.angel = circle(game->player.rot - (float) game->fov / 2);
	while (c < WIDTH)
	//while (c <= WIDTH / 2)
	{
		// horizon
		dof = 50;
		if (ray.angel != 180 && ray.angel != 0)
			horizon_ray(game, &ray);
		else
		{
			ray.ray.x = game->player.pos.x;
			ray.ray.y = game->player.pos.y;
			dof = 0;
		}
		hray = cast_rays(game, dof, &ray);	
		// vertical
		dof = 50;
		if (ray.angel != 90 && ray.angel != 270)
			vertical_ray(game, &ray);
		else
		{
			ray.ray.x = game->player.pos.x;
			ray.ray.y = game->player.pos.y;
			dof = 0;
		}
		vray = cast_rays(game, dof, &ray);
		// distance
		distance(game, hray, vray, &ray);
		draw_colum2(game, c, ray);
		ray.angel = circle(ray.angel + ray.angel_step);
		c += game->column_size;
	}
	//printf("\n");
}

void	draw_map2(t_game *game)
{
	t_vector2i	a;
	t_vector2i	b;
	t_vector2i	i;
	t_vector2f	p1;
	t_vector2f	p2;
	int		t;
	int		scale;

	// map
	scale = 5;
	t = game->tile_size / scale;
	i.y = 0;
	// printf("x = %d, y = %d\n", game->map.size.x,  game->map.size.y);
	while (i.y < game->map.size.y)
	{
		i.x = 0;
		while (i.x < game->map.size.x - 1)
		{
			a.x = i.x * t;
			a.y = i.y * t;
			b.x = (i.x + 1) * t;
			b.y = (i.y + 1) * t;
			//draw_cube(game, a, b, 0xedf2f4ff);
			// printf("%d", game->map.map[i.x][i.y]);
			if (game->map.map[i.y][i.x] == '0')
				draw_cube(game->map.img, a, b, 0x8d99aeff);
			if (game->map.map[i.y][i.x] == '1')
				draw_cube(game->map.img, a, b, 0x2b2d42ff);
			i.x++;
		}
		// printf("\n");
		i.y++;
	}
	// player
	p1.x = game->player.pos.x / scale;
	p1.y = game->player.pos.y / scale;
	//p1.c = int2rgb(0xffffffff);
	p2.x = game->player.pos.x / scale + cos(game->player.rot * DR) * 2;
	p2.y = game->player.pos.y / scale + sin(game->player.rot * DR) * 2;
	//p2.c = int2rgb(0xffffffff);
	//draw_lineDDA(game, p1, p2);
	draw_line1(game->map.img, p1, p2);
}

void	draw_game(t_game *game)
{
	t_vector2i	a;
	t_vector2i	b;

	// sky
	a.x = 0;
	a.y = 0;
	b.x = WIDTH;
	b.y = HEIGHT / 2;
	draw_cube(game->img, a, b, game->map.col[0]);
	// floor
	a.x = 0;
	a.y = HEIGHT / 2;
	b.x = WIDTH;
	b.y = HEIGHT;
	draw_cube(game->img, a, b, game->map.col[1]);
	// map
	draw_map2(game);
	// walls
	draw_walls(game);
}

void	render(t_game *game)
{
	// mlx_delete_image(game->mlx, game->img);
	// mlx_delete_image(game->mlx, game->map.img);
	// game->img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
	// game->map.img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
	draw_game(game);
	// if (!game->img || (mlx_image_to_window(game->mlx, game->img, 0, 0) < 0))
	// 	return ;
	// if (!game->map.img || (mlx_image_to_window(game->mlx, game->map.img, 0, 0) < 0))
	// 	return ;
}

void	hook(void *param)
{
	t_game  	*game;
	float		pdx;
	float		pdy;
	t_vector2i	ptile;
	int			speed;
	int			margin;

	game = (t_game *)param;
	render(game);
	game->player.rot = circle(game->player.rot);
	if (mlx_is_key_down(game->mlx, MLX_KEY_D))
		game->player.rot += 3;
	if (mlx_is_key_down(game->mlx, MLX_KEY_A))
		game->player.rot -= 3;
	speed = 3;
	margin = 5;
	pdx = cos(game->player.rot * DR) * speed;
	pdy = sin(game->player.rot * DR) * speed;
	if (mlx_is_key_down(game->mlx, MLX_KEY_W))
	{
		ptile.x = (int) (game->player.pos.x + pdx * margin) / game->tile_size;
		ptile.y = (int) (game->player.pos.y + pdy * margin) / game->tile_size;
		if (ptile.x >= 0 && ptile.y >= 0 && ptile.x < game->map.size.x && ptile.y < game->map.size.y
				&& game->map.map[ptile.y][ptile.x] != '1')
		{
			game->player.pos.x += pdx;
			game->player.pos.y += pdy;
		}
	}
	if (mlx_is_key_down(game->mlx, MLX_KEY_S))
	{
		ptile.x = (int) (game->player.pos.x - pdx * margin) / game->tile_size;
		ptile.y = (int) (game->player.pos.y - pdy * margin) / game->tile_size;
		if (ptile.x >= 0 && ptile.y >= 0 && ptile.x < game->map.size.x && ptile.y < game->map.size.y
				&& game->map.map[ptile.y][ptile.x] != '1')
		{
			game->player.pos.x -= pdx;
			game->player.pos.y -= pdy;
		}
	}
	if (mlx_is_key_down(game->mlx, MLX_KEY_ESCAPE))
	{
		mlx_close_window(game->mlx);
		free_arr(game->map.map);
		//free(game->p);
	}
}
/*
void movehook(mlx_key_data_t keydata, void* param)
{
	t_game	*game;
	float	pdx;
	float	pdy;

	game = (t_game *) param;
	//if (game->player.rot > 2 * PI)
	//	game->player.rot -= 2 * PI;
	//if (game->player.rot < 0)
	//	game->player.rot += 2 * PI;
	pdx = cos(game->player.rot) * 10;
	pdy = sin(game->player.rot) * 10;
	if (keydata.key == MLX_KEY_D && keydata.action == MLX_PRESS)
		game->player.rot += DR;
	if (keydata.key == MLX_KEY_A && keydata.action == MLX_PRESS)
		game->player.rot += DR;
	if (keydata.key == MLX_KEY_W && keydata.action == MLX_PRESS)
		game->player.pos.x += 10;
	if (keydata.key == MLX_KEY_S && keydata.action == MLX_PRESS)
		game->player.pos.y += 10;
}
*/
int	main(int ac, char **av)
{
	t_game	game;

	if (ac == 2)
	{
		game.tile_size = 32;
		if (parsing_main(&game, av[1]))
		{
			write(2, "Error\n", 6);
			exit(1);
		}
		game.fov = 60;
		game.column_size = 1;
		// game->column_size = WIDTH / game->fov;
		// read_map(&game, av);
		// game.map.size.x = 50;
		// game.map.size.y = 50;
		// printf("player (%f, %f)\n", game.player.pos.x, game.player.pos.y);
		// printf("size (%d, %d)\n", game.map.size.x, game.map.size.y);
		game.mlx = mlx_init(WIDTH, HEIGHT, "mossy rocks", true);
		game.img = mlx_new_image(game.mlx, WIDTH, HEIGHT);
		game.map.img = mlx_new_image(game.mlx, WIDTH, HEIGHT);
		// game.map.img = mlx_new_image(game.mlx, (game.map.size.x + 1) * game.tile_size, (game.map.size.y + 1) * game.tile_size);
		if (!game.img || mlx_image_to_window(game.mlx, game.img, 0, 0) < 0)
			return (0);
		if (!game.img || mlx_image_to_window(game.mlx, game.map.img, 0, 0) < 0)
			return (0);
		//draw_game(&game);
		mlx_loop_hook(game.mlx, hooploop, &game);
		// mlx_loop_hook(game.mlx, &hook, &game);
		// mlx_key_hook(game.mlx, &movehook, &game);
		mlx_loop(game.mlx);
		mlx_terminate(game.mlx);
	}
}
