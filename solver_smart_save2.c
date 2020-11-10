

#include <stdint.h>
#include <stdio.h>
#include <strings.h>
#include <stdlib.h>

uint8_t	board[9 * 9];
uint8_t	ori_board[9 * 9];
uint8_t	uses_horizontal[9 * 10];
uint8_t	uses_vertical[9 * 10];
uint8_t	uses_blocks[9 * 10];
uint8_t	cell_ind_to_block_ind[9 * 9];
uint8_t	cell_ind_to_within_block_x[9 * 9];
uint8_t	cell_ind_to_within_block_y[9 * 9];
uint8_t	cell_ind_to_within_block_ind[9 * 9];
uint8_t	block_ind_to_cell_x[9 * 9];
uint8_t	block_ind_to_cell_y[9 * 9];
uint8_t	next_xs[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 0 };
uint8_t	incr_y[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 1 };

uint8_t	nb_values_impossible[9 * 9];
uint8_t	values_impossible[9 * 9 * 10];

uint8_t	cells_possible_blocks[9 * 10 * 9];
uint8_t	cells_possible_columns[9 * 10 * 9];
uint8_t	cells_possible_rows[9 * 10 * 9];
uint8_t	nb_cells_possible_rows[9 * 10];
uint8_t	nb_cells_possible_columns[9 * 10];
uint8_t	nb_cells_possible_blocks[9 * 10];

void	print_board()
{
	uint32_t	y;
	uint32_t	x;
	char		to_print[9 * 9 + 1];

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
	to_print[9 * 9] = '\0';
	printf("%s\n", to_print);
}

void	print_board_readable()
{
	uint32_t	y;
	uint32_t	x;
	char		to_print[9 * 20 + 1];

	y = 0;
	while (y < 9)
	{
		x = 0;
		while (x < 9)
		{
			to_print[y * 20 + x * 2] = board[y * 9 + x] + '0';
			to_print[y * 20 + x * 2 + 1] = ' ';
			x++;
		}
		to_print[y * 20 + x * 2] = ' ';
		to_print[y * 20 + x * 2 + 1] = '\n';
		y++;
	}
	to_print[9 * 20] = '\0';
	printf("%s", to_print);
}

void	print_cells_possible_state(void)
{
	uint32_t	ind1;
	uint32_t	cell;
	uint32_t	ind2;

	printf("Rows:\n");
	ind1 = 0;
	while (ind1 < 9)
	{
		printf("  Row %d:\n", ind1);
		cell = 1;
		while (cell <= 9)
		{
			printf("    Cell %d, %d candidates: ", cell, nb_cells_possible_rows[ind1 * 10 + cell]);
			ind2 = 0;
			while (ind2 < 9)
			{
				printf("%d: %d | ", ind2, cells_possible_rows[(ind1 * 10 + cell) * 9 + ind2]);
				ind2++;
			}
			printf("\n");
			cell++;
		}
		ind1++;
	}

	printf("Columns:\n");
	ind1 = 0;
	while (ind1 < 9)
	{
		printf("  Column %d:\n", ind1);
		cell = 1;
		while (cell <= 9)
		{
			printf("    Cell %d, %d candidates: ", cell, nb_cells_possible_columns[ind1 * 10 + cell]);
			ind2 = 0;
			while (ind2 < 9)
			{
				printf("%d: %d | ", ind2, cells_possible_columns[(ind1 * 10 + cell) * 9 + ind2]);
				ind2++;
			}
			printf("\n");
			cell++;
		}
		ind1++;
	}

	printf("Blocks:\n");
	ind1 = 0;
	while (ind1 < 9)
	{
		printf("  Block %d:\n", ind1);
		cell = 1;
		while (cell <= 9)
		{
			printf("    Cell %d, %d candidates: ", cell, nb_cells_possible_blocks[ind1 * 10 + cell]);
			ind2 = 0;
			while (ind2 < 9)
			{
				printf("%d: %d | ", ind2, cells_possible_blocks[(ind1 * 10 + cell) * 9 + ind2]);
				ind2++;
			}
			printf("\n");
			cell++;
		}
		ind1++;
	}
}

void	init_ori_board(char *board_descr)
{
	uint32_t	i;

	i = 0;
	while (i < 9 * 9)
	{
		if (board_descr[i] == '.')
			ori_board[i] = 0;
		else
			ori_board[i] = board_descr[i] - '0';
		i++;
	}
}

// void	init_uses()
// {
// 	uint32_t	y;
// 	uint32_t	x;
// 	uint8_t		cell;

// 	y = 0;
// 	while (y < 9)
// 	{
// 		x = 0;
// 		while (x < 9)
// 		{
// 			cell = board[y * 9 + x];
// 			if (cell)
// 			{
// 				uses_horizontal[y * 10 + cell] = 1;
// 				uses_vertical[x * 10 + cell] = 1;
// 				uses_blocks[cell_ind_to_block_ind[y * 9 + x] + cell] = 1;
// 			}
// 			x++;
// 		}
// 		y++;
// 	}
// }

void	init_block_inds()
{
	uint32_t	y;
	uint32_t	x;

	y = 0;
	while (y < 9)
	{
		x = 0;
		while (x < 9)
		{
			cell_ind_to_block_ind[y * 9 + x] = y / 3 * 3 + x / 3;
			cell_ind_to_within_block_x[y * 9 + x] = x % 3;
			cell_ind_to_within_block_y[y * 9 + x] = y % 3;
			cell_ind_to_within_block_ind[y * 9 + x] = y % 3 * 3 + x % 3;
			block_ind_to_cell_x[cell_ind_to_block_ind[y * 9 + x] * 9 + cell_ind_to_within_block_ind[y * 9 + x]] = x;
			block_ind_to_cell_y[cell_ind_to_block_ind[y * 9 + x] * 9 + cell_ind_to_within_block_ind[y * 9 + x]] = y;
			x++;
		}
		y++;
	}
}

void	init_cells_possible()
{
	uint32_t	i1;
	uint32_t	i2;

	i1 = 0;
	while (i1 < 9 * 10)
	{
		i2 = 0;
		nb_cells_possible_blocks[i1] = 9;
		nb_cells_possible_rows[i1] = 9;
		nb_cells_possible_columns[i1] = 9;
		while (i2 < 9)
		{
			cells_possible_blocks[i1 * 9 + i2] = 1;
			cells_possible_rows[i1 * 9 + i2] = 1;
			cells_possible_columns[i1 * 9 + i2] = 1;
			i2++;
		}
		i1++;
	}
}

uint8_t		find_single_available_slot(uint8_t *elts)
{
	uint8_t		i;

	i = 0;
	while (i < 9)
	{
		if (elts[i])
			return (i);
		i++;
	}
	printf("ERROR, assumed there was an available spot, but there isn't!\n");
	exit(1);
}

void	set_cell(uint8_t y, uint8_t x, uint8_t cell)
{
	uint8_t		ind;
	uint8_t		block_ind;
	uint8_t		tmp_block_ind;
	uint8_t		cell_ind_within_block;
	uint8_t		tmp_cell_ind_within_block;
	uint8_t		tmp_y;
	uint8_t		tmp_x;

	//printf("set_cell for %d %d %d\n", y, x, cell);
	board[y * 9 + x] = cell;
	block_ind = cell_ind_to_block_ind[y * 9 + x];
	// Will be set to 0 by following loop
	nb_cells_possible_blocks[block_ind * 10 + cell] = 1;
	nb_cells_possible_rows[y * 10 + cell] = 1;
	nb_cells_possible_columns[x * 10 + cell] = 1;
	// Remove possibility for own rows, columns and blocks:
	ind = 1;
	cell_ind_within_block = cell_ind_to_within_block_ind[y * 9 + x];
	while (ind <= 9)
	{
		nb_cells_possible_blocks[block_ind * 10 + ind] -= cells_possible_blocks[(block_ind * 10 + ind) * 9 + cell_ind_within_block];
		cells_possible_blocks[(block_ind * 10 + ind) * 9 + cell_ind_within_block] = 0;

		nb_cells_possible_rows[y * 10 + ind] -= cells_possible_rows[(y * 10 + ind) * 9 + x];
		cells_possible_rows[(y * 10 + ind) * 9 + x] = 0;
		nb_cells_possible_columns[x * 10 + ind] -= cells_possible_columns[(x * 10 + ind) * 9 + y];
		cells_possible_columns[(x * 10 + ind) * 9 + y] = 0;

		if (nb_cells_possible_blocks[block_ind * 10 + ind] == 1)
		{
			tmp_cell_ind_within_block = find_single_available_slot(&(cells_possible_blocks[(block_ind * 10 + ind) * 9]));
			set_cell(block_ind_to_cell_y[block_ind * 9 + tmp_cell_ind_within_block], block_ind_to_cell_x[block_ind * 9 + tmp_cell_ind_within_block], ind);
		}
		if (nb_cells_possible_rows[y * 10 + ind] == 1)
			set_cell(y, find_single_available_slot(&(cells_possible_rows[(y * 10 + ind) * 9])), ind);
		if (nb_cells_possible_columns[x * 10 + ind] == 1)
			set_cell(find_single_available_slot(&(cells_possible_columns[(x * 10 + ind) * 9])), x, ind);
		ind++;
	}
	// Remove possibility for all other rows, columns and blocks:
	ind = 0;
	while (ind < 9)
	{
		// 2 times for blocks: 1 time horizontal, 1 time vertical
		tmp_block_ind = cell_ind_to_block_ind[ind * 9 + x];
		tmp_cell_ind_within_block = cell_ind_to_within_block_ind[ind * 9 + x];
		nb_cells_possible_blocks[tmp_block_ind * 10 + cell] -= cells_possible_blocks[(tmp_block_ind * 10 + cell) * 9 + tmp_cell_ind_within_block];
		cells_possible_blocks[(tmp_block_ind * 10 + cell) * 9 + tmp_cell_ind_within_block] = 0;
		
		tmp_block_ind = cell_ind_to_block_ind[y * 9 + ind];
		tmp_cell_ind_within_block = cell_ind_to_within_block_ind[y * 9 + ind];
		nb_cells_possible_blocks[tmp_block_ind * 10 + cell] -= cells_possible_blocks[(tmp_block_ind * 10 + cell) * 9 + tmp_cell_ind_within_block];
		cells_possible_blocks[(tmp_block_ind * 10 + cell) * 9 + tmp_cell_ind_within_block] = 0;

		nb_cells_possible_rows[ind * 10 + cell] -= cells_possible_rows[(ind * 10 + cell) * 9 + x];
		cells_possible_rows[(ind * 10 + cell) * 9 + x] = 0;
		nb_cells_possible_columns[ind * 10 + cell] -= cells_possible_columns[(ind * 10 + cell) * 9 + y];
		cells_possible_columns[(ind * 10 + cell) * 9 + y] = 0;

		tmp_block_ind = cell_ind_to_block_ind[ind * 9 + x];
		if (nb_cells_possible_blocks[tmp_block_ind * 10 + cell] == 1)
		{
			tmp_cell_ind_within_block = find_single_available_slot(&(cells_possible_blocks[(tmp_block_ind * 10 + cell) * 9]));
			set_cell(block_ind_to_cell_y[tmp_block_ind * 9 + tmp_cell_ind_within_block], block_ind_to_cell_x[tmp_block_ind * 9 + tmp_cell_ind_within_block], cell);
		}
		tmp_block_ind = cell_ind_to_block_ind[y * 9 + ind];
		if (nb_cells_possible_blocks[tmp_block_ind * 10 + cell] == 1)
		{
			tmp_cell_ind_within_block = find_single_available_slot(&(cells_possible_blocks[(tmp_block_ind * 10 + cell) * 9]));
			set_cell(block_ind_to_cell_y[tmp_block_ind * 9 + tmp_cell_ind_within_block], block_ind_to_cell_x[tmp_block_ind * 9 + tmp_cell_ind_within_block], cell);
		}
		if (nb_cells_possible_rows[ind * 10 + cell] == 1)
			set_cell(ind, find_single_available_slot(&(cells_possible_rows[(ind * 10 + cell) * 9])), cell);
		if (nb_cells_possible_columns[ind * 10 + cell] == 1)
			set_cell(find_single_available_slot(&(cells_possible_columns[(ind * 10 + cell) * 9])), ind, cell);
		ind++;
	}
	// Remove possibility for all cells within same block for rows and columns
	ind = 0;
	while (ind < 9)
	{
		tmp_y = block_ind_to_cell_y[block_ind * 9 + ind];
		tmp_x = block_ind_to_cell_x[block_ind * 9 + ind];

		nb_cells_possible_rows[tmp_y * 10 + cell] -= cells_possible_rows[(tmp_y * 10 + cell) * 9 + tmp_x];
		cells_possible_rows[(tmp_y * 10 + cell) * 9 + tmp_x] = 0;
		nb_cells_possible_columns[tmp_x * 10 + cell] -= cells_possible_columns[(tmp_x * 10 + cell) * 9 + tmp_y];
		cells_possible_columns[(tmp_x * 10 + cell) * 9 + tmp_y] = 0;

		if (nb_cells_possible_rows[tmp_y * 10 + cell] == 1)
			set_cell(tmp_y, find_single_available_slot(&(cells_possible_rows[(tmp_y * 10 + cell) * 9])), cell);
		if (nb_cells_possible_columns[tmp_x * 10 + cell] == 1)
			set_cell(find_single_available_slot(&(cells_possible_columns[(tmp_x * 10 + cell) * 9])), tmp_x, cell);

		ind++;
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
			cell = ori_board[y * 9 + x];
			// Should not be already set in board
			if (cell && !board[y * 9 + x])
			{
				//printf("Setting %d %d\n", y, x);
				// if (y == 8 && x == 4)
				// {
				// 	printf("LAA\n");
				// 	print_cells_possible_state();
				// 	print_board_readable();
				// }
				//print_cells_possible_state();
				set_cell(y, x, cell);
				// if (y == 8 && x == 4)
				// {
				// 	printf("LAA\n");
				// 	print_cells_possible_state();
				// 	print_board_readable();
				// 	exit(1);
				// }
				//print_cells_possible_state();
			}
			x++;
		}
		y++;
	}
	print_cells_possible_state();
	return (1);
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

// void	set_cell(uint8_t cell, uint8_t y, uint8_t x);

// void	set_cells_possible_blocks(uint8_t set_block_ind, uint8_t cell, uint8_t y, uint8_t x)
// {
// 	uint8_t		block_row;
// 	uint8_t		i_candidate;

// 	(void)x;
// 	block_row = (set_block_ind * 10 + cell) * 9 + y / 3 * 3;
// 	nb_cells_impossible_blocks[set_block_ind * 10 + cell] +=
// 		cells_possible_blocks[block_row + 0] +
// 		cells_possible_blocks[block_row + 1] +
// 		cells_possible_blocks[block_row + 2];
// 	cells_possible_blocks[block_row + 0] = 0;
// 	cells_possible_blocks[block_row + 1] = 0;
// 	cells_possible_blocks[block_row + 2] = 0;
// 	if (nb_cells_impossible_blocks[set_block_ind * 10 + cell] == 8)
// 	{
// 		nb_cells_impossible_blocks[set_block_ind * 10 + cell]++;
// 		i_candidate = 0;
// 		while (i_candidate < 9)
// 		{
// 			if (cells_possible_blocks[(set_block_ind * 10 + cell) * 9 + i_candidate])
// 			{
// 				set_cell(cell, (set_block_ind / 3 * 3) + i_candidate / 3, (set_block_ind % 3 * 3) + i_candidate % 3);
// 				break;
// 			}
// 			i_candidate++;
// 		}
// 	}
// }

// void	set_cell(uint8_t cell, uint8_t y, uint8_t x)
// {
// 	uint8_t		set_y;
// 	uint8_t		set_x;
// 	uint8_t		set_y2;
// 	uint8_t		set_x2;
// 	uint8_t		tmp_cell;
// 	uint8_t		block_ind;
// 	uint8_t		set_block_ind;





// 	nb_cells_impossible_rows[y * 10 + cell] = 9;
// 	nb_cells_impossible_columns[x * 10 + cell] = 9;
// 	block_ind = cell_ind_to_block_ind[y * 9 + x];
// 	nb_cells_impossible_blocks[block_ind * 10 + cell] = 9;
// 	switch (block_ind)
// 	{
// 		case 0:
// 			set_block_ind = 1;
// 			set_cells_possible_blocks(set_block_ind, cell, y, x);
// 			set_block_ind = 2;
// 			set_cells_possible_blocks(set_block_ind, cell, y, x);
// 			set_block_ind = 3;
// 			set_cells_possible_blocks(set_block_ind, cell, y, x);
// 			set_block_ind = 6;
// 			set_cells_possible_blocks(set_block_ind, cell, y, x);
// 			break;

// 		case 1:
// 			set_block_ind = 0;
// 			set_cells_possible_blocks(set_block_ind, cell, y, x);
// 			set_block_ind = 2;
// 			set_cells_possible_blocks(set_block_ind, cell, y, x);
// 			set_block_ind = 4;
// 			set_cells_possible_blocks(set_block_ind, cell, y, x);
// 			set_block_ind = 7;
// 			set_cells_possible_blocks(set_block_ind, cell, y, x);
// 			break;

// 		case 2:
// 			set_block_ind = 0;
// 			set_cells_possible_blocks(set_block_ind, cell, y, x);
// 			set_block_ind = 1;
// 			set_cells_possible_blocks(set_block_ind, cell, y, x);
// 			set_block_ind = 5;
// 			set_cells_possible_blocks(set_block_ind, cell, y, x);
// 			set_block_ind = 8;
// 			set_cells_possible_blocks(set_block_ind, cell, y, x);
// 			break;

// 		case 3:
// 			set_block_ind = 0;
// 			set_cells_possible_blocks(set_block_ind, cell, y, x);
// 			set_block_ind = 4;
// 			set_cells_possible_blocks(set_block_ind, cell, y, x);
// 			set_block_ind = 5;
// 			set_cells_possible_blocks(set_block_ind, cell, y, x);
// 			set_block_ind = 6;
// 			set_cells_possible_blocks(set_block_ind, cell, y, x);
// 			break;

// 		case 4:
// 			set_block_ind = 1;
// 			set_cells_possible_blocks(set_block_ind, cell, y, x);
// 			set_block_ind = 3;
// 			set_cells_possible_blocks(set_block_ind, cell, y, x);
// 			set_block_ind = 5;
// 			set_cells_possible_blocks(set_block_ind, cell, y, x);
// 			set_block_ind = 7;
// 			set_cells_possible_blocks(set_block_ind, cell, y, x);
// 			break;

// 		case 5:
// 			set_block_ind = 2;
// 			set_cells_possible_blocks(set_block_ind, cell, y, x);
// 			set_block_ind = 3;
// 			set_cells_possible_blocks(set_block_ind, cell, y, x);
// 			set_block_ind = 4;
// 			set_cells_possible_blocks(set_block_ind, cell, y, x);
// 			set_block_ind = 8;
// 			set_cells_possible_blocks(set_block_ind, cell, y, x);
// 			break;

// 		case 6:
// 			set_block_ind = 0;
// 			set_cells_possible_blocks(set_block_ind, cell, y, x);
// 			set_block_ind = 3;
// 			set_cells_possible_blocks(set_block_ind, cell, y, x);
// 			set_block_ind = 7;
// 			set_cells_possible_blocks(set_block_ind, cell, y, x);
// 			set_block_ind = 8;
// 			set_cells_possible_blocks(set_block_ind, cell, y, x);
// 			break;

// 		case 7:
// 			set_block_ind = 1;
// 			set_cells_possible_blocks(set_block_ind, cell, y, x);
// 			set_block_ind = 4;
// 			set_cells_possible_blocks(set_block_ind, cell, y, x);
// 			set_block_ind = 6;
// 			set_cells_possible_blocks(set_block_ind, cell, y, x);
// 			set_block_ind = 8;
// 			set_cells_possible_blocks(set_block_ind, cell, y, x);
// 			break;

// 		case 8:
// 			set_block_ind = 2;
// 			set_cells_possible_blocks(set_block_ind, cell, y, x);
// 			set_block_ind = 5;
// 			set_cells_possible_blocks(set_block_ind, cell, y, x);
// 			set_block_ind = 6;
// 			set_cells_possible_blocks(set_block_ind, cell, y, x);
// 			set_block_ind = 7;
// 			set_cells_possible_blocks(set_block_ind, cell, y, x);
// 			break;
// 	}







// 	set_y = 0;
// 	while (set_y < 8)
// 	{
// 		if (!values_impossible[(set_y * 9 + x) * 10 + cell])
// 		{
// 			values_impossible[(set_y * 9 + x) * 10 + cell] = 1;
// 			nb_values_impossible[set_y * 9 + x]++;
// 			if (nb_values_impossible[set_y * 9 + x] == 8)
// 			{
// 				nb_values_impossible[set_y * 9 + x]++;
// 				tmp_cell = 1;
// 				while (tmp_cell <= 9)
// 				{
// 					if (!values_impossible[(set_y * 9 + x) * 10 + tmp_cell])
// 					{
// 						board[set_y * 9 + x] = tmp_cell;
// 						set_cell(tmp_cell, set_y, x);
// 					}
// 					tmp_cell++;
// 				}
// 			}
// 		}
// 		set_y++;
// 	}
// 	set_x = 0;
// 	while (set_x < 8)
// 	{
// 		if (!values_impossible[(y * 9 + set_x) * 10 + cell])
// 		{
// 			values_impossible[(y * 9 + set_x) * 10 + cell] = 1;
// 			nb_values_impossible[y * 9 + set_x]++;
// 			if (nb_values_impossible[y * 9 + set_x] == 8)
// 			{
// 				nb_values_impossible[y * 9 + set_x]++;
// 				tmp_cell = 1;
// 				while (tmp_cell <= 9)
// 				{
// 					if (!values_impossible[(y * 9 + set_x) * 10 + tmp_cell])
// 					{
// 						board[y * 9 + set_x] = tmp_cell;
// 						set_cell(tmp_cell, y, set_x);
// 					}
// 					tmp_cell++;
// 				}
// 			}
// 		}
// 		set_x++;
// 	}
// 	set_x = cell_ind_to_block_ind[y * 9 + x] % 3 * 3;
// 	set_y = cell_ind_to_block_ind[y * 9 + x] / 3 * 3;
// 	set_y2 = set_y;
// 	while (set_y2 < set_y + 3)
// 	{
// 		set_x2 = set_x;
// 		while (set_x2 < set_x + 3)
// 		{
// 			if (!values_impossible[(set_y2 * 9 + set_x2) * 10 + cell])
// 			{
// 				values_impossible[(set_y2 * 9 + set_x2) * 10 + cell] = 1;
// 				nb_values_impossible[set_y2 * 9 + set_x2]++;
// 				if (nb_values_impossible[set_y2 * 9 + set_x2] == 8)
// 				{
// 					nb_values_impossible[set_y2 * 9 + set_x2]++;
// 					tmp_cell = 1;
// 					while (tmp_cell <= 9)
// 					{
// 						if (!values_impossible[(set_y2 * 9 + set_x2) * 10 + tmp_cell])
// 						{
// 							board[set_y2 * 9 + set_x2] = tmp_cell;
// 							set_cell(tmp_cell, set_y2, set_x2);
// 						}
// 						tmp_cell++;
// 					}
// 				}
// 			}
// 			set_x2++;
// 		}
// 		set_y2++;
// 	}
// }

// uint8_t		solve_board()
// {
// 	uint8_t		y;
// 	uint8_t		x;
// 	uint8_t		cell;

// 	y = 0;
// 	while (y < 9)
// 	{
// 		x = 0;
// 		while (x < 9)
// 		{
// 			cell = board[y * 9 + x];
// 			if (cell)
// 				nb_values_impossible[y * 9 + x] = 9;
// 			x++;
// 		}
// 		y++;
// 	}
// 	y = 0;
// 	while (y < 9)
// 	{
// 		x = 0;
// 		while (x < 9)
// 		{
// 			cell = board[y * 9 + x];
// 			if (cell)
// 				set_cell(cell, y, x);
// 			x++;
// 		}
// 		y++;
// 	}
// 	y = 0;
// 	while (y < 9)
// 	{
// 		x = 0;
// 		while (x < 9)
// 		{
// 			if (!board[y * 9 + x])
// 				return (0);
// 			x++;
// 		}
// 		y++;
// 	}
// 	return (1);
// }

int		main(int argc, char **argv)
{
	(void)argc;
	bzero(board, sizeof(uint8_t) * 9 * 9);
	bzero(uses_horizontal, sizeof(uint8_t) * 9 * 10);
	bzero(uses_vertical, sizeof(uint8_t) * 9 * 10);
	bzero(uses_blocks, sizeof(uint8_t) * 9 * 10);
	bzero(nb_values_impossible, sizeof(uint8_t) * 9 * 9);
	bzero(values_impossible, sizeof(uint8_t) * 9 * 9 * 10);
	init_ori_board(argv[1]);
	init_cells_possible();
	init_block_inds();
	//init_uses();
	if (!solve_board())
	{
		//print_cells_possible_state();
		print_board_readable();
		printf("Could not solve board!\n");
	}
	else
		print_board();
}