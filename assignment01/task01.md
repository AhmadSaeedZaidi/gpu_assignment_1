setting up environment on colab. set environment to t4 gpu, and check nvcc and nvidia smi

![alt text](images/image.png)

the `!` can be used to run shell commands in colab, and adding `%%writefile task02.cpp` at the start of the file can be used to make a cpp file. `%%` is used to write block wide commands. Then `!g++` can be used to compile it. example added below:

![alt text](images/image2.png)