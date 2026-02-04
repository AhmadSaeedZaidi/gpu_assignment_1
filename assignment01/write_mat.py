import random

def createRandomMatrices(filename, rows, cols, max_val=100):
    with open(filename, 'w') as f:
        f.write(f"{rows} {cols}\n")
        
        matrix_A = [str(random.randint(0, max_val)) for _ in range(rows * cols)]
        f.write(" ".join(matrix_A) + "\n")
        
        matrix_B = [str(random.randint(0, max_val)) for _ in range(rows * cols)]
        f.write(" ".join(matrix_B) + "\n")

createRandomMatrices("input.txt", 10, 10,20)
print("input.txt created successfully!")