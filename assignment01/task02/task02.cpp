#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <stdexcept>
#include <chrono>

void readInput(const std::string& filename, int& rows, int& cols,
               std::vector<int>& A, std::vector<int>& B) {
    std::ifstream infile(filename);
    if (!infile) throw std::runtime_error("Error: Could not open input file.");
    if (!(infile >> rows >> cols)) throw std::runtime_error("Error: Reading dims.");

    int size = rows * cols;
    A.resize(size);
    B.resize(size);
    for (int i = 0; i < size; ++i) infile >> A[i];
    for (int i = 0; i < size; ++i) infile >> B[i];
}

void addMatrices(int rows, int cols, const std::vector<int>& A,
                 const std::vector<int>& B, std::vector<int>& C) {
    int size = rows * cols;
    C.resize(size);
    for (int i = 0; i < size; ++i) {
        C[i] = A[i] + B[i];
    }
}

void writeOutput(const std::string& filename, int rows, int cols,
                 const std::vector<int>& C) {
    std::ostream* out = &std::cout;
    std::ofstream outfile;
    if (!filename.empty()) {
        outfile.open(filename);
        if (!outfile) throw std::runtime_error("Error: Could not open output file.");
        out = &outfile;
    }
    *out << rows << " " << cols << "\n";
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            *out << C[i * cols + j] << " ";
        }
        *out << "\n";
    }
}

int main(int argc, char* argv[]) {
    try {
        if (argc < 2) {
            std::cerr << "Usage: " << argv[0] << " <input_file> [output_file]\n";
            return 1;
        }

        std::string inputObj = argv[1];
        std::string outputObj = (argc >= 3) ? argv[2] : "";

        int rows, cols;
        std::vector<int> A, B, C;

        readInput(inputObj, rows, cols, A, B);

        // --- TIMER START ---
        auto start = std::chrono::high_resolution_clock::now();
        
        addMatrices(rows, cols, A, B, C);
        
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
        std::cerr << elapsed.count() << "\n";
        // --- TIMER END ---

        writeOutput(outputObj, rows, cols, C);

    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
    return 0;
}