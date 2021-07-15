#include "stdlib.h"
#include "unistd.h"
#include "stdio.h"
#include "math.h"

typedef struct circle circle;

struct circle
{
	char type;
	float x;
	float y;
	float radius;
	char color;
};

typedef struct drawing drawing;

struct drawing
{
	int width;
	int height;
	char *metrice;
};

float square(float a)
{
	return (a * a);
}

float sq_dist(float x1, float y1, float x2, float y2)
{
	float dist_x;
	float dist_y;

	dist_x = square(x1 - x2);
	dist_y = square(y1 - y2);
	return (dist_x + dist_y);
}

int is_in_circle(float x, float y, circle *circle)
{
	float distance;
	float distance_sqrt;

	distance_sqrt = sqrtf(sq_dist(x, y, circle->x, circle->y));
	distance = distance_sqrt - (float)circle->radius;
	if(distance <= 0.00000000)
	{
		if(distance <= -1.00000000)
			return 1;
		return 2;
	}
	return 0;
}

void execute_one(circle *circle, drawing *drawing, int x, int y)
{
	int is_in;

	is_in = is_in_circle((float) x, (float)y, circle);
	if(is_in == 2 || (is_in == 1 && circle->color == 'C'))
		drawing->metrice[x + y * drawing->width] = circle->color;
}

int apply_op(circle *circle, drawing *drawing)
{
	int i;
	int j;

	if((circle->radius <= 0.00000000) || (circle->type != 'C' && circle->type != 'c'))
		return 1;
	i = 0;
	while(i < drawing->width)
	{
		j = 0;
		while(j < drawing->height)
		{
			execute_one(circle, drawing, i, j);
			j++;
		}
		i++;
	}
	return 0;
}

int get_info(FILE *file, circle *circle, drawing *drawing)
{
	int scan_ret;
	char backgroud;
	char *tmp;
	int i;

	scan_ret = fscanf(file, "%d %d %c\n", &drawing->width, &drawing->height, &backgroud);
	if(scan_ret == 3)
	{
		if(drawing->width < 0 || drawing->width > 300 || drawing->height < 0 || drawing->height > 300)
			return 1;
		tmp = (char *)malloc(drawing->width * drawing->height);
		drawing->metrice = tmp;
		if(!drawing->metrice)
			return 1;
		i = 0;
		while(i < drawing->width * drawing->height)
		{
			drawing->metrice[i] = backgroud;
			i++;
		}
		return 0;
	}
	return 1;
}

void print_info(drawing *drawing)
{
	int i;

	i = 0;
	while(i < drawing->height)
	{
		printf("%.*s\n", drawing->width, drawing->metrice + i * drawing->width);
		i = i + 1;
	}
}

int execute(FILE *file)
{
	int scan_ret;
	drawing drawing;
	circle circle;

	if(!(get_info(file, &circle, &drawing)))
	{
		scan_ret = fscanf(file, "%c %f %f %f %c\n", &circle.type, &circle.x, &circle.y, &circle.radius, &circle.color);
		while(scan_ret == 5)
		{
			if(apply_op(&circle, &drawing))
				return 1;
			scan_ret = fscanf(file, "%c %f %f %f %c\n", &circle.type, &circle.x, &circle.y, &circle.radius, &circle.color);
		}
		if(scan_ret == -1)
		{
			print_info(&drawing);
			return 0;
		}
		return 1;
	}
	return 1;
}

int main(int argc, char **argv)
{
	FILE *file;

	if(argc == 2)
	{
		file = fopen(argv[1], "r");
		if(file && !execute(file))
			return (0);
		write(2, "Error 1\n", 7);
	}
	else
	{
		write(2, "Error\n", 6);
	}
	return 1;
}


