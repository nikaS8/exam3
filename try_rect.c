#include "stdlib.h"
#include "unistd.h"
#include "stdio.h"

typedef struct rectangle rectangle;
typedef struct drawing drawing;

struct drawing
{
	int width;
	int height;
	char *matrice;
};

struct rectangle
{
	char type;
	float x;
	float y;
	float width;
	float height;
	char color;
};

int is_inside(float x, float y, rectangle *rect, drawing *draw)
{
	if(x < rect->x || rect->x + rect->width < x || y < rect->y || rect->y + rect->height < y)
		return 0;
	if(x - rect->x < 1.00000000 || (rect->x + rect->width) - x < 1.00000000 || y - rect->y < 1.00000000 || (rect->y + rect->height) - y < 1.00000000)
		return (2);
	return (1);
}

void execute_one(int x, int y, rectangle *rect, drawing *draw)
{
	int is_in;

	is_in = is_inside((float)x, (float)y, rect, draw);
	if(is_in == 2 || is_in == 1 && rect->type == 'R')
		draw->matrice[x + y * draw->width] = rect->color;
	return;
}

int apply_op(rectangle *rect, drawing *draw)
{
	int i;
	int j;

	if((rect->width <= 0.00000000 || rect->height <= 0.00000000) ||
			(rect->type != 'R' && rect->type != 'r'))
		return 1;
	i = 0;
	while(i < draw->width)
	{
		j = 0;
		while(j < draw->height)
		{
			execute_one(i, j, rect, draw);
			j++;
		}
		i++;
	}
	return 0;
}

void print_info(drawing *draw)
{
	int i;

	i = 0;
	while(i < draw->height)
	{
		printf("%.*s\n", draw->width, draw->matrice + i++ * draw->width);
	}
}

int get_info(FILE *file, rectangle *rect, drawing *draw)
{
	char background;
	char *tmp;
	int i = 0;

	if(fscanf(file, "%d %d %c\n", &draw->width, &draw->height, &background) == 3)
	{
		if(draw->width < 0 || draw->width > 300 || draw->height < 0 || draw->height > 300)
			return 1;
		tmp = (char*)malloc(draw->width * draw->height);
		draw->matrice = tmp;
		if(!draw->matrice)
			return 1;
		while(i < draw->width * draw->height)
		{
			draw->matrice[i] = background;
			i++;
		}
		return (0);
	}
	return (1);
}

int execute(FILE *file, rectangle *rect, drawing *draw)
{
	int scan_ret;


	if(!get_info(file, rect, draw))
	{
		scan_ret = fscanf(file, "%c %f %f %f %f %c\n", &rect->type, &rect->x, &rect->y, &rect->width, &rect->height, &rect->color);
		while(scan_ret == 6)
		{
			if(apply_op(rect, draw))
				return 1;
			scan_ret = fscanf(file, "%c %f %f %f %f %c\n", &rect->type, &rect->x, &rect->y, &rect->width, &rect->height, &rect->color);
		}
		if(scan_ret == -1)
		{
			print_info(draw);
			return (0);
		}
	}
	return (1);
}

int main(int argc, char **argv)
{
	FILE *file;
	rectangle rectangle;
	drawing drawing;

	if(argc == 2)
	{
		file = fopen(argv[1], "r");
		if(file && !execute(file, &rectangle, &drawing))
			return 0;
		write(2, "Error: Operation file corrupted\n", 32);
	}
	else
	{
		write(2, "Error: Arguments\n", 17);
	}
	return(1);
}
