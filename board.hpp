typedef unsigned long long ull;

#include <utility>
#include <vector>

#ifndef REVERSICLASS_MAIN_HPP
#define REVERSICLASS_MAIN_HPP

class Board {

private:
    static const int max_turn = 130;
    ull black, white, flip_mask[max_turn]{}, flip_point[max_turn]{};
    int turn, black_turn;
public:
    Board();

    Board(ull new_black, ull new_white, int new_turn);

    void show();

    void pass();

    ull get_flip_point();

    void flip(int point);

    void undo();

    std::pair<int, int> count();

    std::vector<int> get_flip_point_vector();

    bool is_end();

    void play();

    Board copy();

    std::vector<int> get_board_vector();

    bool is_black();

    bool is_white();
};

#endif //REVERSICLASS_MAIN_HPP
