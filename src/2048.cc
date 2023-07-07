#include <iostream>
#include <iomanip>
#include <random>
#include <vector>

int board[16];
int coord[] = {0, 1, 2, 3};
std::random_device random_device;
std::default_random_engine random_engine(random_device());
std::bernoulli_distribution get_four(0.1);

int idx(int row, int col)
{
	return row*4+col;
}

void show_board()
{
    for (auto col:coord) std::cout << "+-----";
    std::cout << "+\n";
    for (auto row:coord) {
        std::cout << '|';
        for (auto col:coord) {
            if (board[idx(row,col)]) std::cout << std::setw(5) << board[idx(row,col)] << '|';
            else std::cout << "     |";
        }
        std::cout << '\n';
        for (auto col:coord) std::cout << "+-----";
        std::cout << "+\n";
    }
}

void initial_board()
{
    for (auto & i:board) i = 0;
}

bool has_room()
{
    for (auto i:board) if (i==0) return true;
    return false;
}

void random_drop()
{
    std::vector<int> empty_sites;
    for (int i = 0; i<16; i++) {
        if (board[i]==0) empty_sites.push_back(i);
    }
    std::uniform_int_distribution<int> random_pick(0,empty_sites.size()-1);
    int site = random_pick(random_engine);
    board[empty_sites[site]] = get_four(random_engine)?4:2;
}

enum class Move {
	Up,
	Down,
	Left,
	Right
};

void merge_line(int start, int increment)
{
	auto last_site = 0;
	for (int site = 1; site<4; site++) if (board[start+increment*site]) {
		if (board[start+increment*last_site]==board[start+increment*site]) { // Merge
			board[start+increment*last_site] *= 2;
			board[start+increment*site] = 0;
			last_site += 1;
		}
		else if (board[start+increment*last_site]==0) { // Move
			board[start+increment*last_site] = board[start+increment*site];
			board[start+increment*site] = 0;
		}
		else { // Not the same number
			last_site ++;
			if (last_site<site) { // Move
				board[start+increment*last_site] = board[start+increment*site];
				board[start+increment*site] = 0;
			}
		}
	}
}

void play(Move move)
{
	switch (move) {
	case Move::Up:
		for (auto col:coord) merge_line(idx(0,col),4);
		break;
	case Move::Down:
		for (auto col:coord) merge_line(idx(3,col),-4);
		break;
	case Move::Right:
		for (auto row:coord) merge_line(idx(row,3),-1);
		break;
	case Move::Left:
		for (auto row:coord) merge_line(idx(row,0),1);
		break;
	}
    random_drop();
    show_board();
}

int main(int argc, char ** argv)
{
    initial_board();
	random_drop();
	random_drop();
	show_board();
    while (has_room()) {
        bool want_quit = false;
        auto i = getchar();
        switch (i) {
        case 'i':
        case 'w':
            // Up
            std::cout << "Up\n";
			play(Move::Up);
            break;
        case 'j':
        case 'a':
            // Left
            std::cout << "Left\n";
			play(Move::Left);
            break;
        case 'k':
        case 's':
            // Down
            std::cout << "Down\n";
			play(Move::Down);
            break;
        case 'l':
        case 'd':
            // Right
            std::cout << "Right\n";
			play(Move::Right);
            break;
        case 'q':
            want_quit = true;
            break;
        case 10:
            break;
        default:
            std::cout << "Unknown key: " << i << '\n';
        }
        if (want_quit) break;
    }
}
