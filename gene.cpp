#include "board.hpp"
#include <random>
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cassert>
#include <cmath>


namespace Gene {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> weight(-1.0f, 1.0);

    const float merge_random_alpha = 0.4;

    struct Gene {
        std::vector<float> gene;

        Gene() {
            gene = std::vector<float>(64);
            for (int i = 0; i < 64; ++i) {
                gene[i] = weight(mt);
            }
        }

        Gene(std::vector<float> &gene) {
            assert(gene.size() == 64);
            this->gene = gene;
        }


        float predict(Board &board) {
            const auto board_vector = board.get_board_vector();

            float res = 0;
            for (int i = 0; i < 64; ++i) {
                res += board_vector[i] * gene[i];
            }

            return res;
        }

        int best_move(Board &board) {
            const auto move = board.get_flip_point_vector();
            if (move.size() == 0) {
                return -1;
            }

            int is_black = 1;
            if (board.is_white()) {
                is_black *= -1;
            }

            std::vector<int> max_move(0);
            float max_score = 0;
            for (const int &m: move) {
                board.flip(m);
                auto score = is_black * predict(board);
                board.undo();

                if (max_move.size() == 0 || max_score == score) {
                    max_move.emplace_back(m);
                    max_score = score;
                } else if (max_score < score) {
                    max_move = {m};
                    max_score = score;
                }
            }
            std::uniform_int_distribution<> ind(0, (int) max_move.size() - 1);
            return max_move[ind(mt)];
        }

        void save(std::string file_name) {
            std::ofstream file;
            file.open(file_name, std::ios::out);

            int i = 0;
            for (int j = 0; j < 8; ++j) {
                for (int k = 0; k < 8; ++k) {
                    file << std::fixed << std::setprecision(8) << gene[i] << " ";
                    i++;
                }
                file << "\n";
            }
        }

        Gene(std::string file_name) {
            std::ifstream file;
            file.open(file_name, std::ios::in);

            gene = std::vector<float>(64);
            for (int i = 0; i < 64; ++i) {
                file >> gene[i];
            }
        }

        void normalize() {
            float mean = 0, mean2 = 0;
            for (auto &i: gene) {
                mean += i;
                mean2 += i * i;
            }
            mean /= 64;
            mean2 /= 64;

            const float std = std::sqrt(mean2 - mean * mean);

            for (auto &i:gene) {
                i = (i - mean) / std;
            }
        }
    };

    Gene merge_gene_symmetry(const Gene &a, const Gene &b, const int score_a, const int score_b) {
        std::vector<float> new_gene(64);
        for (int x = 0; x < 4; ++x) {
            for (int y = 0; y < 4; ++y) {
                int i = x * 8 + y;
                new_gene[i] = (a.gene[i] * score_a + b.gene[i] * score_b) / (score_a + score_b) *
                              (1 + merge_random_alpha * weight(mt));
            }
        }

        for (int x = 0; x < 8; ++x) {
            for (int y = 0; y < 8; ++y) {
                int i = x * 8 + y;
                int j = std::min(x, 7 - x) * 8 + std::min(y, 7 - y);
                new_gene[i] = new_gene[j];
            }
        }

        auto res = Gene(new_gene);
        res.normalize();
        return res;
    }

    Gene merge_gene(const Gene &a, const Gene &b, const int score_a, const int score_b) {
        std::vector<float> new_gene(64);
        for (int i = 0; i < 64; ++i) {
            new_gene[i] = (a.gene[i] * score_a + b.gene[i] * score_b) / (score_a + score_b) *
                          (1 + merge_random_alpha * weight(mt));
        }

        auto res = Gene(new_gene);
        res.normalize();
        return res;
    }

    std::pair<int, int> game(Gene &gene_a, Gene &gene_b) {
        Board board;
        while (true) {
            if (board.is_end()) {
                break;
            }

            int c;
            if (board.is_black()) {
                c = gene_a.best_move(board);
            } else {
                c = gene_b.best_move(board);
            }

            if (c == -1) {
                board.pass();
            } else {
                board.flip(c);
            }
        }
        return board.count();
    }
}