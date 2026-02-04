import subprocess
import matplotlib.pyplot as plt
import random


def createRandomMatrices(filename, rows, cols, max_val=100):
    with open(filename, 'w') as f:
        f.write(f"{rows} {cols}\n")
        
        matrix_A = [str(random.randint(0, max_val)) for _ in range(rows * cols)]
        f.write(" ".join(matrix_A) + "\n")
        
        matrix_B = [str(random.randint(0, max_val)) for _ in range(rows * cols)]
        f.write(" ".join(matrix_B) + "\n")


sizes = [2, 4, 16, 64,100, 500, 1000, 2000, 5000]
cpu_times = []
gpu_times = []

for n in sizes:
    print(f"Running N={n}")
    createRandomMatrices("temp.txt", n, n)

    # Run CPU
    res_cpu = subprocess.run(["./matadd_cpu", "temp.txt"], capture_output=True, text=True)
    cpu_times.append(float(res_cpu.stderr.strip().splitlines()[-1]))

    # Run GPU
    res_gpu = subprocess.run(["./matadd_gpu", "temp.txt"], capture_output=True, text=True)
    gpu_times.append(float(res_gpu.stderr.strip().splitlines()[-1]))

# Plot
plt.plot(sizes, cpu_times, label='CPU')
plt.plot(sizes, gpu_times, label='GPU')
plt.xlabel('Size')
plt.ylabel('Time (ms)')
plt.legend()
plt.savefig('task04_plot.png')
print("Plot saved to task04_plot.png")

# 