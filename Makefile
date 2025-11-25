# Compiler settings - Can be customized
CC = gcc
CFLAGS = -O3 -march=native -flto -Wall -Wextra
LIBS = -lgmp

# Targets - What we want to build
all: factor primos

# 1. Linking the executables
# We tell make that 'factor' depends on factor.o and find_divisor.o
factor: factor.o find_divisor.o
	$(CC) $(CFLAGS) -o factor factor.o find_divisor.o $(LIBS)

# We tell make that 'primos' depends on primos.o and find_divisor.o
primos: primos.o find_divisor.o
	$(CC) $(CFLAGS) -o primos primos.o find_divisor.o $(LIBS)

# 2. Compiling source files into object files
# By adding find_divisor.h as a dependency, we ensure that if the header
# changes, the source files relying on it will be recompiled.

factor.o: factor.c find_divisor.h
	$(CC) $(CFLAGS) -c factor.c

primos.o: primos.c find_divisor.h
	$(CC) $(CFLAGS) -c primos.c

find_divisor.o: find_divisor.c find_divisor.h
	$(CC) $(CFLAGS) -c find_divisor.c

# 3. Clean up
# Deletes binaries and object files to start fresh
clean:
	rm -f *.o factor primos
