#include "board.hpp"
#include <iostream>
#include <utility>

using namespace std;

typedef unsigned long long ull;

Board::Board() {
    black = 0x0000000810000000ULL;
    white = 0x0000001008000000ULL;
    turn = 0;
    black_turn = 0;
}

Board::Board(ull new_black, ull new_white, int new_turn) {
    black = new_black;
    white = new_white;
    turn = 0;
    black_turn = new_turn;
}

void Board::show() {
    ull mask = 1;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if ((black & mask) != 0) {
                cout << " X ";
            } else if ((white & mask) != 0) {
                cout << " O ";
            } else {
                cout << " - ";
            }
            mask <<= 1ULL;
        }
        cout << endl;
    }
}

void Board::pass() {
    flip_mask[turn] = 0;
    flip_point[turn] = 0;
    if (turn + 1 < max_turn) {
        turn++;
    }
}

ull Board::get_flip_point() {
    ull bl = black, wh = white;
    if (is_white()) {
        swap(bl, wh);
    }
    ull blank = ~(bl | wh), w, t, mob = 0;

    w = wh & 0x7e7e7e7e7e7e7e7eULL;
    t = w & (bl << 1ULL);
    t |= w & (t << 1ULL);
    t |= w & (t << 1ULL);
    t |= w & (t << 1ULL);
    t |= w & (t << 1ULL);
    t |= w & (t << 1ULL);

    mob |= blank & (t << 1ULL);

    w = wh & 0x7e7e7e7e7e7e7e7eULL;
    t = w & (bl >> 1ULL);
    t |= w & (t >> 1ULL);
    t |= w & (t >> 1ULL);
    t |= w & (t >> 1ULL);
    t |= w & (t >> 1ULL);
    t |= w & (t >> 1ULL);

    mob |= blank & (t >> 1ULL);

    w = wh & 0x00ffffffffffff00ULL;
    t = w & (bl << 8ULL);
    t |= w & (t << 8ULL);
    t |= w & (t << 8ULL);
    t |= w & (t << 8ULL);
    t |= w & (t << 8ULL);
    t |= w & (t << 8ULL);

    mob |= blank & (t << 8ULL);

    w = wh & 0x00ffffffffffff00ULL;
    t = w & (bl >> 8ULL);
    t |= w & (t >> 8ULL);
    t |= w & (t >> 8ULL);
    t |= w & (t >> 8ULL);
    t |= w & (t >> 8ULL);
    t |= w & (t >> 8ULL);

    mob |= blank & (t >> 8ULL);

    w = wh & 0x007e7e7e7e7e7e00ULL;
    t = w & (bl << 9ULL);
    t |= w & (t << 9ULL);
    t |= w & (t << 9ULL);
    t |= w & (t << 9ULL);
    t |= w & (t << 9ULL);
    t |= w & (t << 9ULL);

    mob |= blank & (t << 9ULL);

    w = wh & 0x007e7e7e7e7e7e00ULL;
    t = w & (bl >> 9ULL);
    t |= w & (t >> 9ULL);
    t |= w & (t >> 9ULL);
    t |= w & (t >> 9ULL);
    t |= w & (t >> 9ULL);
    t |= w & (t >> 9ULL);

    mob |= blank & (t >> 9ULL);

    w = wh & 0x007e7e7e7e7e7e00ULL;
    t = w & (bl << 7ULL);
    t |= w & (t << 7ULL);
    t |= w & (t << 7ULL);
    t |= w & (t << 7ULL);
    t |= w & (t << 7ULL);
    t |= w & (t << 7ULL);

    mob |= blank & (t << 7ULL);

    w = wh & 0x007e7e7e7e7e7e00ULL;
    t = w & (bl >> 7ULL);
    t |= w & (t >> 7ULL);
    t |= w & (t >> 7ULL);
    t |= w & (t >> 7ULL);
    t |= w & (t >> 7ULL);
    t |= w & (t >> 7ULL);

    mob |= blank & (t >> 7ULL);

    return mob;


}

void Board::flip(int point) {
    ull bl = black, wh = white;
    if (is_white()) {
        swap(bl, wh);
    }
    ull f = 0, d, t, pos = 1ULL << point, mo = wh & 0x7e7e7e7e7e7e7e7eULL;
    d = 0x00000000000000feULL * pos;
    t = (mo | ~d) + 1ULL & d & bl;
    f |= t - (t != 0) & d;

    d = 0x8040201008040200ULL * pos;
    t = (mo | ~d) + 1ULL & d & bl;
    f |= t - (t != 0) & d;

    d = 0x0101010101010100ULL * pos;
    t = (wh | ~d) + 1ULL & d & bl;
    f |= t - (t != 0) & d;

    d = 0x0002040810204080ULL * pos;
    t = (mo | ~d) + 1ULL & d & bl;
    f |= t - (t != 0) & d;

    t = pos >> 1ULL & mo;
    t |= t >> 1ULL & mo;
    t |= t >> 1ULL & mo;
    t |= t >> 1ULL & mo;
    t |= t >> 1ULL & mo;
    t |= t >> 1ULL & mo;
    f |= t & -(t >> 1ULL & bl);

    t = pos >> 9ULL & mo;
    t |= t >> 9ULL & mo;
    t |= t >> 9ULL & mo;
    t |= t >> 9ULL & mo;
    t |= t >> 9ULL & mo;
    t |= t >> 9ULL & mo;
    f |= t & -(t >> 9ULL & bl);

    t = pos >> 8ULL & wh;
    t |= t >> 8ULL & wh;
    t |= t >> 8ULL & wh;
    t |= t >> 8ULL & wh;
    t |= t >> 8ULL & wh;
    t |= t >> 8ULL & wh;
    f |= t & -(t >> 8ULL & bl);

    t = pos >> 7ULL & mo;
    t |= t >> 7ULL & mo;
    t |= t >> 7ULL & mo;
    t |= t >> 7ULL & mo;
    t |= t >> 7ULL & mo;
    t |= t >> 7ULL & mo;
    f |= t & -(t >> 7ULL & bl);

    black ^= f;
    white ^= f;
    if (is_black()) {
        black ^= pos;
    } else {
        white ^= pos;
    }

    flip_mask[turn] = f;
    flip_point[turn] = pos;
    turn++;
}

void Board::undo() {
    if (turn == 0) {
        return;
    }
    turn--;
    black ^= flip_mask[turn];
    white ^= flip_mask[turn];
    if (is_black()) {
        black ^= flip_point[turn];
    } else {
        white ^= flip_point[turn];
    }
}

pair<int, int> Board::count() {
    ull b = black, w = white;
    b = (b & 0x5555555555555555ULL) + (b >> 1ULL & 0x5555555555555555ULL);
    b = (b & 0x3333333333333333ULL) + (b >> 2ULL & 0x3333333333333333ULL);
    b = (b & 0x0f0f0f0f0f0f0f0fULL) + (b >> 4ULL & 0x0f0f0f0f0f0f0f0fULL);
    b = (b & 0x00ff00ff00ff00ffULL) + (b >> 8ULL & 0x00ff00ff00ff00ffULL);
    b = (b & 0x0000ffff0000ffffULL) + (b >> 16ULL & 0x0000ffff0000ffffULL);
    b = (b & 0x00000000ffffffffULL) + (b >> 32ULL & 0x00000000ffffffffULL);
    w = (w & 0x5555555555555555ULL) + (w >> 1ULL & 0x5555555555555555ULL);
    w = (w & 0x3333333333333333ULL) + (w >> 2ULL & 0x3333333333333333ULL);
    w = (w & 0x0f0f0f0f0f0f0f0fULL) + (w >> 4ULL & 0x0f0f0f0f0f0f0f0fULL);
    w = (w & 0x00ff00ff00ff00ffULL) + (w >> 8ULL & 0x00ff00ff00ff00ffULL);
    w = (w & 0x0000ffff0000ffffULL) + (w >> 16ULL & 0x0000ffff0000ffffULL);
    w = (w & 0x00000000ffffffffULL) + (w >> 32ULL & 0x00000000ffffffffULL);
    return make_pair(int(b), int(w));

}

vector<int> Board::get_flip_point_vector() {
    ull mob = get_flip_point(), mask = 1;
    vector<int> mob_vector = {};
    for (int i = 0; i < 64; i++) {
        if ((mask & mob) != 0) {
            mob_vector.push_back(i);
        }
        mask <<= 1ULL;
    }
    return mob_vector;
}

bool Board::is_end() {
    if ((black | white) == 0xffffffffffffffffULL) {
        return true;
    }
    if (turn == max_turn - 1) {
        return true;
    }
    return turn > 1 && flip_point[turn - 1] == 0 && flip_point[turn - 2] == 0;

}

void Board::play() {
    black = 0x0000000810000000ULL;
    white = 0x0000001008000000ULL;
    turn = 0;
    black_turn = 0;
    while (true) {
        if (is_end()) {
            break;
        }
        show();
        auto b = get_flip_point_vector();
        for (auto i:b) {
            cout << i << " ";
        }
        cout << endl;
        int c;
        cin >> c;
        if (c == -1) {
            pass();
        } else {
            flip(c);
        }
        auto d = count();
        cout << d.first << " " << d.second << endl;
    }
}

Board Board::copy() {
    Board new_board(black, white, 1 - is_black());
    return new_board;
}

vector<int> Board::get_board_vector() {
    vector<int> res(64, 0);
    for (int i = 0; i < 64; ++i) {
        if (((black >> i) & 1) == 1) {
            res[i] = 1;
        } else if (((white >> i) & 1) == 1) {
            res[i] = -1;
        }
    }

    return res;
}

bool Board::is_black() {
    return (turn & 1) == black_turn;
}

bool Board::is_white() {
    return (turn & 1) != black_turn;
}