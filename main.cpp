#include "board.cpp"
#include "gene.cpp"
#include <chrono>


const int gene_n = 1000;
std::random_device rnd;
std::mt19937 mt(rnd());
std::uniform_int_distribution rand_n(0, gene_n - 1);

const int epoch_number = 100000;

std::vector<Gene::Gene> next_gen(std::vector<Gene::Gene> &parents) {
    std::vector<Gene::Gene> children(gene_n);

    for (int i = 0; i < gene_n; ++i) {
        int index_a = rand_n(mt), index_b = rand_n(mt);
        while (index_a == index_b) {
            index_a = rand_n(mt);
            index_b = rand_n(mt);
        }
        auto parent_a = parents[index_a], parent_b = parents[index_b];

        auto game = Gene::game(parent_a, parent_b);
        children[i] = Gene::merge_gene(parent_a, parent_b, game.first, game.second);
    }

    return children;
}


int main() {
    std::vector<Gene::Gene> gene(gene_n);
    for (int i = 0; i < gene_n; ++i) {
        gene[i] = Gene::Gene();
    }

    std::string file_name;
    printf("Save file name?\n");
    std::getline(std::cin, file_name);

    for (int epoch = 0; epoch < epoch_number; ++epoch) {
        gene = next_gen(gene);

        gene[0].save(file_name);

        auto end = std::chrono::system_clock::now();
        auto end_time = std::chrono::system_clock::to_time_t(end);
        printf("%d th loop finished at %s", epoch, std::ctime(&end_time));
    }
}