

// for fixed size types
#include <stdint.h>
// for printf()
#include <stdio.h>
// for exit()
#include <stdlib.h>

uint8_t	nb_cells_set;
uint8_t	board[9 * 9];
uint8_t	ori_board[9 * 9];

uint8_t	cell_ind_to_block_ind[9 * 9];
uint8_t	cell_ind_to_within_block_x[9 * 9];
uint8_t	cell_ind_to_within_block_y[9 * 9];
uint8_t	cell_ind_to_within_block_ind[9 * 9];
uint8_t	block_ind_to_cell_x[9 * 9];
uint8_t	block_ind_to_cell_y[9 * 9];
uint8_t	next_xs[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 0 };
uint8_t	incr_y[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 1 };

uint8_t	cells_possible_blocks[9 * 10 * 9];
uint8_t	cells_possible_columns[9 * 10 * 9];
uint8_t	cells_possible_rows[9 * 10 * 9];
uint8_t	nb_cells_possible_rows[9 * 10];
uint8_t	nb_cells_possible_columns[9 * 10];
uint8_t	nb_cells_possible_blocks[9 * 10];

uint8_t	values_possible_for_cell[9 * 9 * 10];
uint8_t	nb_values_possible_for_cell[9 * 9];

void	my_memcpy(void *dest, void *src, size_t size)
{
	size_t		i;
	size_t		big_end;
	uint64_t	*dest64;
	uint64_t	*src64;
	uint8_t		*dest8;
	uint8_t		*src8;

	dest64 = (uint64_t*)dest;
	src64 = (uint64_t*)src;
	dest8 = (uint8_t*)dest;
	src8 = (uint8_t*)src;
	big_end = size / sizeof(uint64_t);
	i = 0;
	while (i < big_end)
	{
		dest64[i] = src64[i];
		i++;
	}
	i = big_end * sizeof(uint64_t);
	while (i < size)
	{
		dest8[i] = src8[i];
		i++;
	}
}

void	my_bzero(void *arr, size_t size)
{
	size_t		i;
	size_t		big_end;
	uint64_t	*arr64;
	uint8_t		*arr8;

	arr64 = (uint64_t*)arr;
	arr8 = (uint8_t*)arr;
	big_end = size / sizeof(uint64_t);
	i = 0;
	while (i < big_end)
	{
		arr64[i] = 0;
		i++;
	}
	i = big_end * sizeof(uint64_t);
	while (i < size)
	{
		arr8[i] = 0;
		i++;
	}
}

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

	printf("Cells possible for values:\n");
	printf("  Rows:\n");
	ind1 = 0;
	while (ind1 < 9)
	{
		printf("    Row %d:\n", ind1);
		cell = 1;
		while (cell <= 9)
		{
			printf("      Cell %d, %d candidates: ", cell, nb_cells_possible_rows[ind1 * 10 + cell]);
			ind2 = 0;
			while (ind2 < 9)
			{
				if (cells_possible_rows[(ind1 * 10 + cell) * 9 + ind2])
					printf("%d | ", ind2);
				ind2++;
			}
			printf("\n");
			cell++;
		}
		ind1++;
	}

	printf("  Columns:\n");
	ind1 = 0;
	while (ind1 < 9)
	{
		printf("    Column %d:\n", ind1);
		cell = 1;
		while (cell <= 9)
		{
			printf("      Cell %d, %d candidates: ", cell, nb_cells_possible_columns[ind1 * 10 + cell]);
			ind2 = 0;
			while (ind2 < 9)
			{
				if (cells_possible_columns[(ind1 * 10 + cell) * 9 + ind2])
					printf("%d | ", ind2);
				ind2++;
			}
			printf("\n");
			cell++;
		}
		ind1++;
	}

	printf("  Blocks:\n");
	ind1 = 0;
	while (ind1 < 9)
	{
		printf("    Block %d:\n", ind1);
		cell = 1;
		while (cell <= 9)
		{
			printf("      Cell %d, %d candidates: ", cell, nb_cells_possible_blocks[ind1 * 10 + cell]);
			ind2 = 0;
			while (ind2 < 9)
			{
				if (cells_possible_blocks[(ind1 * 10 + cell) * 9 + ind2])
					printf("%d | ", ind2);
				ind2++;
			}
			printf("\n");
			cell++;
		}
		ind1++;
	}

	printf("Values possible for cells:\n");
	ind1 = 0;
	while (ind1 < 9)
	{
		printf("  Row %d\n", ind1);
		ind2 = 0;
		while (ind2 < 9)
		{
			printf("    Cell %d, %d candidates: ", ind2, nb_values_possible_for_cell[ind1 * 9 + ind2]);
			cell = 1;
			while (cell <= 9)
			{
				if (values_possible_for_cell[(ind1 * 9 + ind2) * 10 + cell])
					printf("%d | ", cell);
				cell++;
			}
			printf("\n");
			ind2++;
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
	i1 = 0;
	while (i1 < 9 * 9)
	{
		nb_values_possible_for_cell[i1] = 9;
		i2 = 0;
		while (i2 < 10)
		{
			values_possible_for_cell[i1 * 10 + i2] = 1;
			i2++;
		}
		i1++;
	}
}

uint8_t		find_possible_value_for_cell(uint8_t *elts)
{
	uint32_t	i;

	i = 1;
	while (i <= 9)
	{
		if (elts[i])
			return (i);
		i++;
	}
	printf("ERROR, assumed there was an available value, but there isn't!\n");
	exit(1);
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

uint8_t	set_cell(uint8_t y, uint8_t x, uint8_t cell)
{
	uint8_t		ind;
	uint8_t		ind_cell;
	uint8_t		block_ind;
	uint8_t		block_ind_vertical;
	uint8_t		block_ind_horizontal;
	uint8_t		tmp_block_ind;
	uint8_t		cell_ind_within_block;
	uint8_t		tmp_cell_ind_within_block;
	uint8_t		tmp_y;
	uint8_t		tmp_x;

	nb_cells_set++;
	board[y * 9 + x] = cell;
	block_ind = cell_ind_to_block_ind[y * 9 + x];
	nb_values_possible_for_cell[y * 9 + x] = 255;
	nb_cells_possible_blocks[block_ind * 10 + cell] = 255;
	nb_cells_possible_rows[y * 10 + cell] = 255;
	nb_cells_possible_columns[x * 10 + cell] = 255;
	// Remove possibility for own rows, columns and blocks:
	ind_cell = 1;
	ind = 0;
	cell_ind_within_block = cell_ind_to_within_block_ind[y * 9 + x];
	while (ind_cell <= 9)
	{
		// Remove possible values for own row
		nb_values_possible_for_cell[y * 9 + ind] -= values_possible_for_cell[(y * 9 + ind) * 10 + cell];
		if (nb_values_possible_for_cell[y * 9 + ind] == 0)
			return (0);
		values_possible_for_cell[(y * 9 + ind) * 10 + cell] = 0;
		// Remove possible values for own column
		nb_values_possible_for_cell[ind * 9 + x] -= values_possible_for_cell[(ind * 9 + x) * 10 + cell];
		if (nb_values_possible_for_cell[ind * 9 + x] == 0)
			return (0);
		values_possible_for_cell[(ind * 9 + x) * 10 + cell] = 0;
		// Remove possible values for own block
		tmp_block_ind = block_ind_to_cell_y[block_ind * 9 + ind] * 9 + block_ind_to_cell_x[block_ind * 9 + ind];
		nb_values_possible_for_cell[tmp_block_ind] -= values_possible_for_cell[tmp_block_ind * 10 + cell];
		if (nb_values_possible_for_cell[tmp_block_ind] == 0)
			return (0);
		values_possible_for_cell[tmp_block_ind * 10 + cell] = 0;

		// Remove possible cells for own block
		nb_cells_possible_blocks[block_ind * 10 + ind_cell] -= cells_possible_blocks[(block_ind * 10 + ind_cell) * 9 + cell_ind_within_block];
		if (nb_cells_possible_blocks[block_ind * 10 + ind_cell] == 0)
			return (0);
		cells_possible_blocks[(block_ind * 10 + ind_cell) * 9 + cell_ind_within_block] = 0;
		// Remove possible cells for own row
		nb_cells_possible_rows[y * 10 + ind_cell] -= cells_possible_rows[(y * 10 + ind_cell) * 9 + x];
		if (nb_cells_possible_rows[y * 10 + ind_cell] == 0)
			return (0);
		cells_possible_rows[(y * 10 + ind_cell) * 9 + x] = 0;
		// Remove possible cells for own column
		nb_cells_possible_columns[x * 10 + ind_cell] -= cells_possible_columns[(x * 10 + ind_cell) * 9 + y];
		if (nb_cells_possible_columns[x * 10 + ind_cell] == 0)
			return (0);
		cells_possible_columns[(x * 10 + ind_cell) * 9 + y] = 0;

		// Remove possible cells for all other blocks vertically
		block_ind_vertical = cell_ind_to_block_ind[ind * 9 + x];
		tmp_cell_ind_within_block = cell_ind_to_within_block_ind[ind * 9 + x];
		nb_cells_possible_blocks[block_ind_vertical * 10 + cell] -= cells_possible_blocks[(block_ind_vertical * 10 + cell) * 9 + tmp_cell_ind_within_block];
		if (nb_cells_possible_blocks[block_ind_vertical * 10 + cell] == 0)
			return (0);
		cells_possible_blocks[(block_ind_vertical * 10 + cell) * 9 + tmp_cell_ind_within_block] = 0;

		// Remove possible cells for all other blocks horizontally
		block_ind_horizontal = cell_ind_to_block_ind[y * 9 + ind];
		tmp_cell_ind_within_block = cell_ind_to_within_block_ind[y * 9 + ind];
		nb_cells_possible_blocks[block_ind_horizontal * 10 + cell] -= cells_possible_blocks[(block_ind_horizontal * 10 + cell) * 9 + tmp_cell_ind_within_block];
		if (nb_cells_possible_blocks[block_ind_horizontal * 10 + cell] == 0)
			return (0);
		cells_possible_blocks[(block_ind_horizontal * 10 + cell) * 9 + tmp_cell_ind_within_block] = 0;

		// Remove possible cells for all other blocks rows
		nb_cells_possible_rows[ind * 10 + cell] -= cells_possible_rows[(ind * 10 + cell) * 9 + x];
		if (nb_cells_possible_rows[ind * 10 + cell] == 0)
			return (0);
		cells_possible_rows[(ind * 10 + cell) * 9 + x] = 0;
		// Remove possible cells for all other blocks columns
		nb_cells_possible_columns[ind * 10 + cell] -= cells_possible_columns[(ind * 10 + cell) * 9 + y];
		if (nb_cells_possible_columns[ind * 10 + cell] == 0)
			return (0);
		cells_possible_columns[(ind * 10 + cell) * 9 + y] = 0;

		tmp_y = block_ind_to_cell_y[block_ind * 9 + ind];
		tmp_x = block_ind_to_cell_x[block_ind * 9 + ind];
		// Remove possible cells for all cells within same block for rows
		nb_cells_possible_rows[tmp_y * 10 + cell] -= cells_possible_rows[(tmp_y * 10 + cell) * 9 + tmp_x];
		if (nb_cells_possible_rows[tmp_y * 10 + cell] == 0)
			return (0);
		cells_possible_rows[(tmp_y * 10 + cell) * 9 + tmp_x] = 0;
		// Remove possible cells for all cells within same block for columns
		nb_cells_possible_columns[tmp_x * 10 + cell] -= cells_possible_columns[(tmp_x * 10 + cell) * 9 + tmp_y];
		if (nb_cells_possible_columns[tmp_x * 10 + cell] == 0)
			return (0);
		cells_possible_columns[(tmp_x * 10 + cell) * 9 + tmp_y] = 0;

		ind_cell++;
		ind++;
	}

	ind_cell = 1;
	ind = 0;
	while (ind_cell <= 9)
	{
		if (nb_values_possible_for_cell[y * 9 + ind] == 1)
		{
			if (!set_cell(y, ind, find_possible_value_for_cell(&(values_possible_for_cell[(y * 9 + ind) * 10]))))
				return (0);
		}
		if (nb_values_possible_for_cell[ind * 9 + x] == 1)
		{
			if (!set_cell(ind, x, find_possible_value_for_cell(&(values_possible_for_cell[(ind * 9 + x) * 10]))))
				return (0);
		}
		tmp_block_ind = block_ind_to_cell_y[block_ind * 9 + ind] * 9 + block_ind_to_cell_x[block_ind * 9 + ind];
		if (nb_values_possible_for_cell[tmp_block_ind] == 1)
		{
			if (!set_cell(block_ind_to_cell_y[block_ind * 9 + ind], block_ind_to_cell_x[block_ind * 9 + ind], find_possible_value_for_cell(&(values_possible_for_cell[tmp_block_ind * 10]))))
				return (0);
		}

		if (nb_cells_possible_blocks[block_ind * 10 + ind_cell] == 1)
		{
			tmp_cell_ind_within_block = find_single_available_slot(&(cells_possible_blocks[(block_ind * 10 + ind_cell) * 9]));
			if (!set_cell(block_ind_to_cell_y[block_ind * 9 + tmp_cell_ind_within_block], block_ind_to_cell_x[block_ind * 9 + tmp_cell_ind_within_block], ind_cell))
				return (0);
		}
		if (nb_cells_possible_rows[y * 10 + ind_cell] == 1)
		{
			if (!set_cell(y, find_single_available_slot(&(cells_possible_rows[(y * 10 + ind_cell) * 9])), ind_cell))
				return (0);
		}
		if (nb_cells_possible_columns[x * 10 + ind_cell] == 1)
		{
			if (!set_cell(find_single_available_slot(&(cells_possible_columns[(x * 10 + ind_cell) * 9])), x, ind_cell))
				return (0);
		}

		block_ind_vertical = cell_ind_to_block_ind[ind * 9 + x];
		block_ind_horizontal = cell_ind_to_block_ind[y * 9 + ind];
		if (nb_cells_possible_blocks[block_ind_vertical * 10 + cell] == 1)
		{
			tmp_cell_ind_within_block = find_single_available_slot(&(cells_possible_blocks[(block_ind_vertical * 10 + cell) * 9]));
			if (!set_cell(block_ind_to_cell_y[block_ind_vertical * 9 + tmp_cell_ind_within_block], block_ind_to_cell_x[block_ind_vertical * 9 + tmp_cell_ind_within_block], cell))
				return (0);
		}
		if (nb_cells_possible_blocks[block_ind_horizontal * 10 + cell] == 1)
		{
			tmp_cell_ind_within_block = find_single_available_slot(&(cells_possible_blocks[(block_ind_horizontal * 10 + cell) * 9]));
			if (!set_cell(block_ind_to_cell_y[block_ind_horizontal * 9 + tmp_cell_ind_within_block], block_ind_to_cell_x[block_ind_horizontal * 9 + tmp_cell_ind_within_block], cell))
				return (0);
		}
		if (nb_cells_possible_rows[ind * 10 + cell] == 1)
		{
			if (!set_cell(ind, find_single_available_slot(&(cells_possible_rows[(ind * 10 + cell) * 9])), cell))
				return (0);
		}
		if (nb_cells_possible_columns[ind * 10 + cell] == 1)
		{
			if (!set_cell(find_single_available_slot(&(cells_possible_columns[(ind * 10 + cell) * 9])), ind, cell))
				return (0);
		}

		tmp_y = block_ind_to_cell_y[block_ind * 9 + ind];
		tmp_x = block_ind_to_cell_x[block_ind * 9 + ind];
		if (nb_cells_possible_rows[tmp_y * 10 + cell] == 1)
		{
			if (!set_cell(tmp_y, find_single_available_slot(&(cells_possible_rows[(tmp_y * 10 + cell) * 9])), cell))
				return (0);
		}
		if (nb_cells_possible_columns[tmp_x * 10 + cell] == 1)
		{
			if (!set_cell(find_single_available_slot(&(cells_possible_columns[(tmp_x * 10 + cell) * 9])), tmp_x, cell))
				return (0);
		}

		ind_cell++;
		ind++;
	}
	return (1);
}

void	find_next_cell_to_try(uint8_t *next_y, uint8_t *next_x)
{
	uint8_t		y;
	uint8_t		x;
	uint8_t		best_nb_candidates;

	best_nb_candidates = 10;
	y = 0;
	while (y < 9)
	{
		x = 0;
		while (x < 9)
		{
			if (nb_values_possible_for_cell[y * 9 + x] < best_nb_candidates)
			{
				best_nb_candidates = nb_values_possible_for_cell[y * 9 + x];
				*next_y = y;
				*next_x = x;
			}
			x++;
		}
		y++;
	}
}

uint8_t		try_to_fill_board(uint8_t y, uint8_t x)
{
	uint8_t		value;
	uint8_t		set_cell_ok;
	uint8_t		next_y;
	uint8_t		next_x;

	uint8_t		saved_nb_cells_set;
	uint8_t		saved_board[9 * 9];
	uint8_t		saved_cells_possible_blocks[9 * 10 * 9];
	uint8_t		saved_cells_possible_columns[9 * 10 * 9];
	uint8_t		saved_cells_possible_rows[9 * 10 * 9];
	uint8_t		saved_nb_cells_possible_rows[9 * 10];
	uint8_t		saved_nb_cells_possible_columns[9 * 10];
	uint8_t		saved_nb_cells_possible_blocks[9 * 10];
	uint8_t		saved_values_possible_for_cell[9 * 9 * 10];
	uint8_t		saved_nb_values_possible_for_cell[9 * 9];

	while (board[y * 9 + x])
	{
		y += incr_y[x];
		x = next_xs[x];
	}

	saved_nb_cells_set = nb_cells_set;
	my_memcpy(saved_board, board, sizeof(uint8_t) * 9 * 9);
	my_memcpy(saved_cells_possible_blocks, cells_possible_blocks, sizeof(uint8_t) * 9 * 10 * 9);
	my_memcpy(saved_cells_possible_columns, cells_possible_columns, sizeof(uint8_t) * 9 * 10 * 9);
	my_memcpy(saved_cells_possible_rows, cells_possible_rows, sizeof(uint8_t) * 9 * 10 * 9);
	my_memcpy(saved_nb_cells_possible_rows, nb_cells_possible_rows, sizeof(uint8_t) * 9 * 10);
	my_memcpy(saved_nb_cells_possible_columns, nb_cells_possible_columns, sizeof(uint8_t) * 9 * 10);
	my_memcpy(saved_nb_cells_possible_blocks, nb_cells_possible_blocks, sizeof(uint8_t) * 9 * 10);
	my_memcpy(saved_values_possible_for_cell, values_possible_for_cell, sizeof(uint8_t) * 9 * 9 * 10);
	my_memcpy(saved_nb_values_possible_for_cell, nb_values_possible_for_cell, sizeof(uint8_t) * 9 * 9);

	value = 1;
	while (value <= 9)
	{
		if (!values_possible_for_cell[(y * 9 + x) * 10 + value])
		{
			value++;
			continue;
		}
		set_cell_ok = set_cell(y, x, value);
		if (set_cell_ok && nb_cells_set == 9 * 9)
			return (1);
		if (set_cell_ok)
		{
			find_next_cell_to_try(&next_y, &next_x);
			if (try_to_fill_board(next_y, next_x))
				return (1);
		}
		nb_cells_set = saved_nb_cells_set;
		my_memcpy(board, saved_board, sizeof(uint8_t) * 9 * 9);
		my_memcpy(cells_possible_blocks, saved_cells_possible_blocks, sizeof(uint8_t) * 9 * 10 * 9);
		my_memcpy(cells_possible_columns, saved_cells_possible_columns, sizeof(uint8_t) * 9 * 10 * 9);
		my_memcpy(cells_possible_rows, saved_cells_possible_rows, sizeof(uint8_t) * 9 * 10 * 9);
		my_memcpy(nb_cells_possible_rows, saved_nb_cells_possible_rows, sizeof(uint8_t) * 9 * 10);
		my_memcpy(nb_cells_possible_columns, saved_nb_cells_possible_columns, sizeof(uint8_t) * 9 * 10);
		my_memcpy(nb_cells_possible_blocks, saved_nb_cells_possible_blocks, sizeof(uint8_t) * 9 * 10);
		my_memcpy(values_possible_for_cell, saved_values_possible_for_cell, sizeof(uint8_t) * 9 * 9 * 10);
		my_memcpy(nb_values_possible_for_cell, saved_nb_values_possible_for_cell, sizeof(uint8_t) * 9 * 9);
		value++;
	}
	// No value could be set for this cell
	return (0);
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
				set_cell(y, x, cell);
			x++;
		}
		y++;
	}
	if (nb_cells_set == 9 * 9)
		return (1);
	return (try_to_fill_board(0, 0));
}

uint8_t		*solve(char *board_descr)
{
	nb_cells_set = 0;
	my_bzero(board, sizeof(uint8_t) * 9 * 9);
	init_ori_board(board_descr);
	init_cells_possible();
	init_block_inds();
	//init_uses();
	if (!solve_board())
	{
		//print_cells_possible_state();
		//print_board_readable();
		//printf("Could not solve board!\n");
	}
	//else
	//	print_board();
	return (board);
}

// int		main(int argc, char **argv)
// {
// 	uint8_t		*board;

// 	(void)argc;
// 	board = solve(argv[1]);
// 	(void)board;
// 	print_board();
// 	return (0);
// }
