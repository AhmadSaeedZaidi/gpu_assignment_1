NVCC = nvcc
NVCCFLAGS = -arch=sm_75 -Xcompiler -Wall,-Wextra

all: matadd_gpu

matadd_gpu: task03.cu
	$(NVCC) $(NVCCFLAGS) -o matadd_gpu task03.cu

clean:
	rm -f matadd_gpu