

#include <stdint.h>
#include <stdio.h>
#include <strings.h>

uint8_t	board[9 * 9] = {
	5, 3, 0,   0, 7, 0,   0, 0, 0,
	6, 0, 0,   1, 9, 5,   0, 0, 0,
	0, 9, 8,   0, 0, 0,   0, 6, 0,

	8, 0, 0,   0, 6, 0,   0, 0, 3,
	4, 0, 0,   8, 0, 3,   0, 0, 1,
	7, 0, 0,   0, 2, 0,   0, 0, 6,

	0, 6, 0,   0, 0, 0,   2, 8, 0,
	0, 0, 0,   4, 1, 9,   0, 0, 5,
	0, 0, 0,   0, 8, 0,   0, 7, 9,
};
uint8_t	uses_horizontal[9 * 10];
uint8_t	uses_vertical[9 * 10];
uint8_t	uses_blocks[9 * 10];
uint8_t	cell_ind_to_block_ind[9 * 9];

// void	test_uses(uint8_t *uses_horizontal, uint8_t *uses_vertical, uint8_t *uses_blocks, uint8_t *cell_ind_to_block_ind)
// {
// 	uint32_t	n;
// 	uint32_t	n2;
// 	uint32_t	i_cell;

// 	printf("Horizontal:\n");
// 	n = 0;
// 	while (n < 9)
// 	{
// 		i_cell = 0;
// 		while (i_cell < 9)
// 		{
// 			printf("%d: %d | ", i_cell + 1, uses_horizontal[n * 10 + (i_cell + 1)]);
// 			i_cell++;
// 		}
// 		printf("\n");
// 		n++;
// 	}
// 	printf("Vertical:\n");
// 	n = 0;
// 	while (n < 9)
// 	{
// 		i_cell = 0;
// 		while (i_cell < 9)
// 		{
// 			printf("%d: %d | ", i_cell + 1, uses_vertical[n * 10 + (i_cell + 1)]);
// 			i_cell++;
// 		}
// 		printf("\n");
// 		n++;
// 	}
// 	printf("Blocks:\n");
// 	n = 0;
// 	while (n < 3)
// 	{
// 		n2 = 0;
// 		while (n2 < 3)
// 		{
// 			i_cell = 0;
// 			while (i_cell < 9)
// 			{
// 				printf("%d: %d | ", i_cell + 1, uses_blocks[cell_ind_to_block_ind[n * 3 * 9 + n2 * 3] + (i_cell + 1)]);
// 				i_cell++;
// 			}
// 			printf("\n");
// 			n2++;
// 		}
// 		n++;
// 	}
// }

void	init_board(char *board_descr)
{
	uint32_t	i;

	i = 0;
	while (i < 9 * 9)
	{
		if (board_descr[i] == '.')
			board[i] = 0;
		else
			board[i] = board_descr[i] - '0';
		i++;
	}
}

void	init_cell_ind_to_block_ind()
{
	uint32_t	y;
	uint32_t	x;

	y = 0;
	while (y < 9)
	{
		x = 0;
		while (x < 9)
		{
			cell_ind_to_block_ind[y * 9 + x] = (y / 3 * 3 + x / 3) * 10;
			x++;
		}
		y++;
	}
}

void	init_uses()
{
	uint32_t	y;
	uint32_t	x;
	uint8_t		cell;

	y = 0;
	while (y < 9)
	{
		x = 0;
		while (x < 9)
		{
			cell = board[y * 9 + x];
			if (cell)
			{
				uses_horizontal[y * 10 + cell] = 1;
				uses_vertical[x * 10 + cell] = 1;
				uses_blocks[cell_ind_to_block_ind[y * 9 + x] + cell] = 1;
			}
			x++;
		}
		y++;
	}
}

uint8_t		solve_board(uint32_t y, uint32_t x)
{
	uint8_t		possible_cell;
	uint32_t	next_y;
	uint32_t	next_x;

	next_x = x + 1;
	next_y = y + (next_x / 9);
	next_x = next_x % 9;
	if (board[y * 9 + x])
	{
		// all ok
		if (next_y > 8)
			return (1);
		return (solve_board(next_y, next_x));
	}
	possible_cell = 1;
	while (possible_cell <= 9)
	{
		if (!uses_horizontal[y * 10 + possible_cell] && !uses_vertical[x * 10 + possible_cell] && !uses_blocks[cell_ind_to_block_ind[y * 9 + x] + possible_cell])
		{
			board[y * 9 + x] = possible_cell;
			uses_horizontal[y * 10 + possible_cell] = 1;
			uses_vertical[x * 10 + possible_cell] = 1;
			uses_blocks[cell_ind_to_block_ind[y * 9 + x] + possible_cell] = 1;
			// all ok
			if (next_y > 8)
				return (1);
			if (solve_board(next_y, next_x))
				return (1);
			board[y * 9 + x] = 0;
			uses_horizontal[y * 10 + possible_cell] = 0;
			uses_vertical[x * 10 + possible_cell] = 0;
			uses_blocks[cell_ind_to_block_ind[y * 9 + x] + possible_cell] = 0;
		}
		possible_cell++;
	}
	return (0);
}

void	print_board()
{
	uint32_t	y;
	uint32_t	x;
	char		to_print[9 * 9];

	y = 0;
	while (y < 9)
	{
		x = 0;
		while (x < 9)
		{
			to_print[y * 9 + x] = board[y * 9 + x] + '0';
			x++;
		}
		y++;
	}
	printf("%s\n", to_print);
}

int		main(int argc, char **argv)
{
	(void)argc;
	bzero(uses_horizontal, sizeof(uint8_t) * 9 * 10);
	bzero(uses_vertical, sizeof(uint8_t) * 9 * 10);
	bzero(uses_blocks, sizeof(uint8_t) * 9 * 10);
	init_board(argv[1]);
	init_cell_ind_to_block_ind();
	init_uses();
	//test_uses(uses_horizontal, uses_vertical, uses_blocks, cell_ind_to_block_ind);
	if (!solve_board(0, 0))
		printf("Could not solve board!\n");
	else
	{
		//printf("Solved board!\n");
		print_board();
	}
}