

#include <stdint.h>
#include <stdio.h>
#include <strings.h>

uint8_t	board[9 * 9];
uint8_t	uses_horizontal[9 * 10];
uint8_t	uses_vertical[9 * 10];
uint8_t	uses_blocks[9 * 10];
uint8_t	cell_ind_to_block_ind[9 * 9];
uint8_t	next_xs[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 0 };
uint8_t	incr_y[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 1 };
uint8_t	nb_values_impossible[9 * 9];
uint8_t	values_impossible[9 * 9 * 10];

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

// uint8_t		solve_board()
// {
// 	uint8_t		prev_possible_cells[9 * 9];
// 	uint8_t		possible_cell;
// 	uint8_t		prev_xs[9 * 9];
// 	uint8_t		prev_ys[9 * 9];
// 	uint8_t		y;
// 	uint8_t		x;
// 	uint8_t		backtrack_n;
// 	uint8_t		ind_horizontal;
// 	uint8_t		ind_vertical;
// 	uint8_t		ind_block;

// 	y = 0;
// 	x = 0;
// 	possible_cell = 1;
// 	backtrack_n = 0;
// 	while (1)
// 	{
// 		while (board[y * 9 + x])
// 		{
// 			// All ok
// 			if (y == 8 && x == 8)
// 				return (1);
// 			y += incr_y[x];
// 			x = next_xs[x];
// 			possible_cell = 1;
// 		}
// 		while (possible_cell <= 9)
// 		{
// 			ind_horizontal = y * 10 + possible_cell;
// 			ind_vertical = x * 10 + possible_cell;
// 			ind_block = cell_ind_to_block_ind[y * 9 + x] + possible_cell;
// 			if (!uses_horizontal[ind_horizontal] && !uses_vertical[ind_vertical] && !uses_blocks[ind_block])
// 			{
// 				board[y * 9 + x] = possible_cell;
// 				uses_horizontal[ind_horizontal] = 1;
// 				uses_vertical[ind_vertical] = 1;
// 				uses_blocks[ind_block] = 1;
// 				// all ok
// 				if (y == 8 && x == 8)
// 					return (1);
// 				prev_xs[backtrack_n] = x;
// 				prev_ys[backtrack_n] = y;
// 				prev_possible_cells[backtrack_n] = possible_cell;
// 				backtrack_n++;
// 				y += incr_y[x];
// 				x = next_xs[x];
// 				possible_cell = 1;
// 				break;
// 			}
// 			possible_cell++;
// 		}
// 		// No cell worked, backtrack
// 		if (possible_cell == 10)
// 		{
// 			backtrack_n--;
// 			x = prev_xs[backtrack_n];
// 			y = prev_ys[backtrack_n];
// 			possible_cell = prev_possible_cells[backtrack_n];
// 			board[y * 9 + x] = 0;
// 			uses_horizontal[y * 10 + possible_cell] = 0;
// 			uses_vertical[x * 10 + possible_cell] = 0;
// 			uses_blocks[cell_ind_to_block_ind[y * 9 + x] + possible_cell] = 0;
// 			possible_cell++;
// 		}
// 	}
// 	return (0);
// }

void	set_cell(uint8_t cell, uint8_t y, uint8_t x)
{
	uint8_t		set_y;
	uint8_t		set_x;
	uint8_t		set_y2;
	uint8_t		set_x2;
	uint8_t		tmp_cell;

	set_y = 0;
	while (set_y < 8)
	{
		if (!values_impossible[(set_y * 9 + x) * 10 + cell])
		{
			values_impossible[(set_y * 9 + x) * 10 + cell] = 1;
			nb_values_impossible[set_y * 9 + x]++;
			if (nb_values_impossible[set_y * 9 + x] == 8)
			{
				nb_values_impossible[set_y * 9 + x]++;
				tmp_cell = 1;
				while (tmp_cell <= 9)
				{
					if (!values_impossible[(set_y * 9 + x) * 10 + tmp_cell])
					{
						board[set_y * 9 + x] = tmp_cell;
						set_cell(tmp_cell, set_y, x);
					}
					tmp_cell++;
				}
			}
		}
		set_y++;
	}
	set_x = 0;
	while (set_x < 8)
	{
		if (!values_impossible[(y * 9 + set_x) * 10 + cell])
		{
			values_impossible[(y * 9 + set_x) * 10 + cell] = 1;
			nb_values_impossible[y * 9 + set_x]++;
			if (nb_values_impossible[y * 9 + set_x] == 8)
			{
				nb_values_impossible[y * 9 + set_x]++;
				tmp_cell = 1;
				while (tmp_cell <= 9)
				{
					if (!values_impossible[(y * 9 + set_x) * 10 + tmp_cell])
					{
						board[y * 9 + set_x] = tmp_cell;
						set_cell(tmp_cell, y, set_x);
					}
					tmp_cell++;
				}
			}
		}
		set_x++;
	}
	set_x = cell_ind_to_block_ind[y * 9 + x] % 3 * 3;
	set_y = cell_ind_to_block_ind[y * 9 + x] / 3 * 3;
	set_y2 = set_y;
	while (set_y2 < set_y + 3)
	{
		set_x2 = set_x;
		while (set_x2 < set_x + 3)
		{
			if (!values_impossible[(set_y2 * 9 + set_x2) * 10 + cell])
			{
				values_impossible[(set_y2 * 9 + set_x2) * 10 + cell] = 1;
				nb_values_impossible[set_y2 * 9 + set_x2]++;
				if (nb_values_impossible[set_y2 * 9 + set_x2] == 8)
				{
					nb_values_impossible[set_y2 * 9 + set_x2]++;
					tmp_cell = 1;
					while (tmp_cell <= 9)
					{
						if (!values_impossible[(set_y2 * 9 + set_x2) * 10 + tmp_cell])
						{
							board[set_y2 * 9 + set_x2] = tmp_cell;
							set_cell(tmp_cell, set_y2, set_x2);
						}
						tmp_cell++;
					}
				}
			}
			set_x2++;
		}
		set_y2++;
	}
}

uint8_t		solve_board()
{
	uint8_t		y;
	uint8_t		x;
	uint8_t		cell;

	y = 0;
	while (y < 9)
	{
		x = 0;
		while (x < 9)
		{
			cell = board[y * 9 + x];
			if (cell)
				nb_values_impossible[y * 9 + x] = 9;
			x++;
		}
		y++;
	}
	y = 0;
	while (y < 9)
	{
		x = 0;
		while (x < 9)
		{
			cell = board[y * 9 + x];
			if (cell)
				set_cell(cell, y, x);
			x++;
		}
		y++;
	}
	y = 0;
	while (y < 9)
	{
		x = 0;
		while (x < 9)
		{
			if (!board[y * 9 + x])
				return (0);
			x++;
		}
		y++;
	}
	return (1);
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
	bzero(nb_values_impossible, sizeof(uint8_t) * 9 * 9);
	bzero(values_impossible, sizeof(uint8_t) * 9 * 9 * 10);
	init_board(argv[1]);
	init_cell_ind_to_block_ind();
	//init_uses();
	if (!solve_board())
		printf("Could not solve board!\n");
	else
		print_board();
}