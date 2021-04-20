#include "ulibc.h"

typedef enum { 
    CASE_EMPTY = 0,
    CASE_P1 = 1,
    CASE_P2 = 2,
    __CASE_END__
} case_state_t;

typedef struct res {
	bool valid;
	uint8_t row;
} res_t;

static const uint8_t ROW = 6;
static const uint8_t COL = 7;

static bool verify_entries(uint8_t min, uint8_t max, uint8_t entry) {
	return (entry >= min && entry <= max);
}

static res_t affect_token(uint8_t col, bool current_player, case_state_t grid[ROW][COL]) {
	res_t res;
	res.valid = false;
	res.row = 0;
	for (int current_row = ROW-1; current_row >= 0; current_row--) {
		if (grid[current_row][col] == CASE_EMPTY) {
			grid[current_row][col] = (current_player)? CASE_P1 : CASE_P2;
			res.valid = true;
			res.row = (uint8_t)current_row;
			break;
		}
	}
	return res;
}

static uint8_t play_random(case_state_t grid[ROW][COL]) {
	uint8_t col = (uint8_t)random(COL);
	while(grid[0][col] != CASE_EMPTY) {
		col = (col + 1) % COL;
	}
	return col;
}

static bool verify_vertical(uint8_t col, uint8_t row, case_state_t player_value, case_state_t grid[ROW][COL]) {
	uint8_t counter = 0;
	uint8_t row_max = row + 3;
	if (row_max < ROW) {
		for (int i = row; i <= row_max; i++)
		{
			if (grid[i][col] != player_value) {
				break;
			}
			counter++;
		}
		
	}
	return (counter == 4);
}

static bool verify_horizontal(uint8_t col, uint8_t row, case_state_t player_value, case_state_t grid[ROW][COL]) {
	uint8_t counter = 0;
	int col_min = (col >= 3)? col - 3 : 0;
	int col_max = (col > 3)? COL : col + 4; 
	for (int i = col_min; i < col_max; i++)
	{
		counter = (grid[row][i] == player_value)? counter + 1 : 0;
		if (counter == 4) {
			break;
		}
	}
	
	return (counter == 4);
}

static bool verify_up_left_to_down_right(uint8_t col, uint8_t row, case_state_t player_value, case_state_t grid[ROW][COL]) {
    int counter = 0;

    int diagonale_col = col - row; // pour trouver la position de début en colonne -> colonne actuelle - ligne actuelle
    int diagonale_row = row - col; // pour trouver la position de début en ligne -> ligne actuelle - colonne actuelle

    if (diagonale_col < 0) {
        diagonale_col = 0;
    }

    if (diagonale_row < 0) {
        diagonale_row = 0;
    }

    while (diagonale_col < COL && diagonale_row < ROW && counter < 4) {
        counter = (grid[diagonale_row][diagonale_col] == player_value)? counter + 1 : 0;
        diagonale_col++;
        diagonale_row++;
    }

    return (counter == 4);
}

static bool verify_down_left_to_up_right(uint8_t col, uint8_t row, case_state_t player_value, case_state_t grid[ROW][COL]) {
    int counter = 0;

    int diagonale_row = row + col;
    int diagonale_col = diagonale_row - (ROW - 1); // pour trouver la position de début en colonne -> colonne actuelle - ligne actuelle

    if (diagonale_col < 0) {
        diagonale_col = 0;
    }

    if (diagonale_row > ROW - 1) {
        diagonale_row = ROW - 1;
    }

    while (diagonale_col < COL && diagonale_row >= 0 && counter < 4) {
        counter = (grid[diagonale_row][diagonale_col] == player_value)? counter + 1 : 0;
        diagonale_col++;
        diagonale_row--;
    }

    return (counter == 4);
}

static bool player_as_win(uint8_t col, uint8_t row, bool current_player, case_state_t grid[ROW][COL]) {
	bool win = false;
	case_state_t player_value = (current_player)? CASE_P1 : CASE_P2;
	if (
		verify_vertical(col, row, player_value, grid) ||
		verify_horizontal(col, row, player_value, grid) ||
		verify_up_left_to_down_right(col, row, player_value, grid) ||
		verify_down_left_to_up_right(col, row, player_value, grid) 
	) {
		win = true;
	}

	return win;
}

static int player_can_win(bool current_player, case_state_t grid[ROW][COL]) {
	int can_win = -1;
	for (int i = 0; i < COL; i++)
	{
		case_state_t grid_copy[ROW][COL];
		memcpy(grid_copy, grid, ROW * COL * sizeof(case_state_t));
		res_t res = affect_token(i, current_player, grid_copy);
		if (res.valid && player_as_win(i, res.row, current_player, grid_copy)) {
			can_win = i;
			break;
		}
	}
	
	return can_win;
}

static uint8_t IA_play_intelligent(bool current_player, case_state_t grid[ROW][COL]) {
	int col = 0;
	col = player_can_win(current_player, grid);
	if (col == -1) {
		col = player_can_win(!current_player, grid); // try to block
		if (col == -1) {
			col = play_random(grid);
		}
	}
	return (uint8_t)col;
}

static bool equality(case_state_t grid[ROW][COL]) {
	bool res = true;
	for (int i = 0; i < COL; i++)
	{
		if (grid[0][i] == CASE_EMPTY) {
			res = false;
			break;
		}
	}
	return res;
}

static void print_grid(case_state_t grid[ROW][COL]) {
	char buf[LG_BUFFER];
	memset(buf, '\0', sizeof(char) * LG_BUFFER);
	char *ptr = buf;
	for (int row = 0; row < ROW; row++)
	{
		strcat(ptr, "\n+---------------------------+\n");
		for (int col = 0; col < COL; col++)
		{
			switch (grid[row][col])
			{
				case CASE_EMPTY:
					strcat(ptr, "|   ");
					break;
				case CASE_P1:
					strcat(ptr, "| x ");
					break;
				case CASE_P2:
					strcat(ptr, "| o ");
					break;
				default:
					strcat(ptr, "|---");
					break;
			}
		}
		strcat(ptr, "|");
	}
	strcat(ptr, "\n+---------------------------+\n");
	strcat(ptr, "  1   2   3   4   5   6   7\n");
	puts_x_y(buf, 40, 9);
}

static void display_error_msg() {
	puts_x_y("\n+--------------------------------+\n| Please enter a correct entry ! |\n+--------------------------------+\n", 2, 20);
	sleep(1000);
}

static void display_win_msg(bool current_player) {
	char buf[SM_BUFFER];
	memset(buf, '\0', sizeof(char) * SM_BUFFER);
	char *ptr = buf;
	strcat(ptr, "\n+--------------------+\n|   ");
	strcat(ptr, (current_player)? "Player 1" : "Player 2");
	strcat(ptr, " win !   |\n+--------------------+\n");
	puts_x_y(buf, 2, 20);
}

static void display_equality_msg() {
	puts_x_y("\n+------------+\n| Equality ! |\n+------------+\n", 2, 20);
}

static void display_replay_msg() {
	puts_x_y("\n+-----------------+\n| Please replay ! |\n+-----------------+\n", 2, 20);
	sleep(1000);
}

static void display_play_msg(bool current_player) {
	char buf[SM_BUFFER];
	memset(buf, '\0', sizeof(char) * SM_BUFFER);
	char *ptr = buf;
	strcat(ptr, (current_player)? "Player 1" : "Player 2");
	strcat(ptr, ", Choose a column between 1-7");
	puts_x_y(buf, 2, 18);
}

static void display_main_menu() {
    puts_x_y("\n+-------------------------+\n|   Choose a game mode    |\n+-------------------------+\n| 1 : Player  VS  Player  |\n| 2 : Player  VS  Dumb IA |\n| 3 : Player  VS  Hard IA |\n| 4 : Hard IA VS  Hard IA |\n| 5 : Exit                |\n+-------------------------+\n",
		2,
		9
	);
}

static void display_info() {
	puts_x_y("+---------------+\n| Player 1 -> x |\n| Player 2 -> o |\n+---------------+", 2, 13);
}

static int get_answer(uint8_t min, uint8_t max) {
	char buf[XS_BUFFER];
	int answer;
	puts_x_y("Your answer :", 2, 19);
	setcursor(15, 19);
	read_string(buf, XS_BUFFER);
	answer = atoi(trim(buf));
	if (!verify_entries(min, max, (uint8_t)answer)) {
		answer = -1;
		display_error_msg();
	}
	return answer;
}

// Entry point of a user application
int main() {
	// setcursor(0, 0);
	clean_vid_x_y(0, 0);
	char buf[LG_BUFFER];
	if (filesread("puissance_4_logo", buf) == 0) {
		puts_x_y(buf, 11, 1);
	}
	case_state_t grid[ROW][COL];
	bool win = false;
	bool current_player = true;
	res_t res;
	int col = -1;
	int game_mode = -1;

	memset(grid, CASE_EMPTY, COL * ROW * sizeof(case_state_t));
	
	while (game_mode == -1)
	{
		clean_vid_x_y(0, 9);
		print_grid(grid);
		display_main_menu();
		game_mode = get_answer(1, 5);
	}
	
	game_mode--;

	if (game_mode == 4) {
		clean_vid_x_y(0, 0);
		setcursor(0,1);
		return 2;
	}

	while (1)
	{
		clean_vid_x_y(0, 9);
		display_info();
		print_grid(grid);
		if (current_player)
		{
			if (game_mode != 3 )
			{
				display_play_msg(current_player);
				col = get_answer(1, 7);
				col--;
			} else {
				col = IA_play_intelligent(current_player, grid);
				sleep(500);
			}
		} else {
			if (game_mode == 0)
			{
				display_play_msg(current_player);
				col = get_answer(1, 7);
				col--;
			} else if (game_mode == 1) {
                col = play_random(grid);
				sleep(500);
            } else {
                col = IA_play_intelligent(current_player, grid);
				sleep(500);
            }
		}

		if (col > -1)
		{
			res = affect_token(col, current_player, grid);
			if (res.valid) {
				if((win = player_as_win(col, res.row, current_player, grid))) {
					break;
				} else if (equality(grid)) {
					break;
				}
				current_player = !current_player;
			} else {
				display_replay_msg();
			}
			
		}
	}


	clean_vid_x_y(0, 9);
	print_grid(grid);
	if (win) {
		display_win_msg(current_player);
	} else {
		display_equality_msg();
	}
	setcursor(2, 19);
	puts("Press enter to exit ");
	read_string(buf, SM_BUFFER);
	
	clean_vid_x_y(0, 0);
	setcursor(0,1);

	return 1;
}