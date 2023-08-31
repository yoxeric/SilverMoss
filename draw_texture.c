/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_texture.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhachami <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 23:28:17 by yhachami          #+#    #+#             */
/*   Updated: 2023/08/31 05:14:48 by yhachami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	player_to_image(t_game *game, t_vector2i img_pos, float sx, float sy)
{
	mlx_texture_t	*tex;
	uint8_t			*pixelx;
	uint8_t			*pixeli;
	int				i;
	int				j;

	tex = game->player.tex;
	j = -1;
	while (++j * sx < tex->width)
	{
		i = -1;
		while (++i * sy < tex->height)
		{
			if ((i + img_pos.y) < HEIGHT && (j + img_pos.x) < WIDTH)
			{
				pixelx = &tex->pixels[(((int)(i * sy) *tex->width) 
						+ (int)(j * sx)) * tex->bytes_per_pixel];
				pixeli = &game->player.img->pixels[((i + img_pos.y) 
						* game->player.img->width + (j + img_pos.x))
					* tex->bytes_per_pixel];
				ft_memmove(pixeli, pixelx, tex->bytes_per_pixel);
			}
		}
	}
}

void	texture_to_image(t_game *game, t_ray ray, t_vector2i a, t_vector2f wall)
{
	mlx_texture_t	*tex;
	uint8_t			*pixelx;
	uint8_t			*pixeli;
	t_vector2f		r;
	int				i;

	tex = game->tex;
	wall.y = 1;
	if (ray.wall_height > HEIGHT)
		wall.y = ((HEIGHT - ray.wall_height) / 2);
	r.x = (float) wall.x * (tex->width / game->tile_size);
	i = 0;
	while (i < ray.wall_height && i < HEIGHT)
	{
		r.y = (float)(i - wall.y) *(tex->height / ray.wall_height);
		if ((((int) r.y * tex->width) + (int) r.x) < (tex->width * tex->height))
		{
			pixelx = &tex->pixels[((((int) r.y) * tex->width)
					+ ((int) r.x)) * tex->bytes_per_pixel];
			pixeli = &game->img->pixels[((a.y + i)
					* game->img->width + (a.x)) * tex->bytes_per_pixel];
			ft_memmove(pixeli, pixelx, tex->bytes_per_pixel);
		}
		i++;
	}
}

void	draw_texture(t_game *game, t_ray ray, t_vector2i a)
{
	t_vector2f	wall;

	if (ray.pos_in_tile.y == 0)
	{
		game->tex = game->map.tex[0];
		wall.x = ray.pos_in_tile.x;
	}
	if (ray.pos_in_tile.y >= game->tile_size - 0.01)
	{
		game->tex = game->map.tex[1];
		wall.x = ray.pos_in_tile.x;
	}
	if (ray.pos_in_tile.x == 0)
	{
		game->tex = game->map.tex[2];
		wall.x = ray.pos_in_tile.y;
	}
	if (ray.pos_in_tile.x >= game->tile_size - 0.01)
	{
		game->tex = game->map.tex[3];
		wall.x = ray.pos_in_tile.y;
	}
	texture_to_image(game, ray, a, wall);
}

//draw_cube(game->img, a, b, 0xAAAAAAFF);
void	draw_colum(t_game *game, int x, t_ray ray)
{
	t_vector2i	a;
	t_vector2i	b;
	int			c;
	int			h;

	c = game->column_size;
	ray.wall_height = (game->tile_size * HEIGHT) / ray.dst;
	h = ray.wall_height;
	if (h > HEIGHT)
		h = HEIGHT;
	a.x = x;
	a.y = (HEIGHT / 2) - (h / 2);
	b.x = x + c;
	b.y = (HEIGHT / 2) + (h / 2);
	if (a.y < 5)
		a.y = 5;
	if (b.y > HEIGHT - 5)
		b.y = HEIGHT - 5;
	if (a.x > 0 && a.x < WIDTH && a.y > 0 && a.y < HEIGHT
		&& b.x > 0 && b.x < WIDTH && b.y > 0 && b.y < HEIGHT)
	{
		draw_texture(game, ray, a);
	}
}