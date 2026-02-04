#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <stdexcept>
#include <cuda_runtime.h>

__global__ void addKernel(const int* A, const int* B, int* C, int size) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < size) {
        C[i] = A[i] + B[i];
    }
}

void checkCuda(cudaError_t result, const char* func) {
    if (result != cudaSuccess) {
        std::cerr << "CUDA Error in " << func << ": " << cudaGetErrorString(result) << std::endl;
        exit(1);
    }
}

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

void addMatricesGPU(int rows, int cols, const std::vector<int>& h_A,
                    const std::vector<int>& h_B, std::vector<int>& h_C) {
    int size = rows * cols;
    size_t bytes = size * sizeof(int);
    h_C.resize(size);

    int *d_A, *d_B, *d_C;
    checkCuda(cudaMalloc(&d_A, bytes), "cudaMalloc A");
    checkCuda(cudaMalloc(&d_B, bytes), "cudaMalloc B");
    checkCuda(cudaMalloc(&d_C, bytes), "cudaMalloc C");

    checkCuda(cudaMemcpy(d_A, h_A.data(), bytes, cudaMemcpyHostToDevice), "Memcpy H2D A");
    checkCuda(cudaMemcpy(d_B, h_B.data(), bytes, cudaMemcpyHostToDevice), "Memcpy H2D B");

    int threadsPerBlock = 256;
    int blocksPerGrid = (size + threadsPerBlock - 1) / threadsPerBlock;
    addKernel<<<blocksPerGrid, threadsPerBlock>>>(d_A, d_B, d_C, size);
    
    checkCuda(cudaGetLastError(), "Kernel Launch");
    checkCuda(cudaDeviceSynchronize(), "Kernel Sync");

    checkCuda(cudaMemcpy(h_C.data(), d_C, bytes, cudaMemcpyDeviceToHost), "Memcpy D2H C");

    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);
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
        std::vector<int> h_A, h_B, h_C;

        readInput(inputObj, rows, cols, h_A, h_B);
        addMatricesGPU(rows, cols, h_A, h_B, h_C);
        writeOutput(outputObj, rows, cols, h_C);

    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
    return 0;
}