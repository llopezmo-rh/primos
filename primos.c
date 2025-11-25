// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.


#include "find_divisor.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <gmp.h>
#include <string.h>


// Initial size of strings where the results will be stored
#define INIT_LENGTH 10

// Strings will be reallocated when the results exceed their size. 
// The new size of the string will be the size of the results plus EXTEND_LENGTH
#define EXTEND_LENGTH 2

// Colour code used for lines showing found prime numbers
#define RED "31"

// Radix for numbers in string format
#define RADIX 10


void usage()
	{
	fprintf
		(
		stderr,
		"Usage: <executable_file> [-b] [-h] [-n <opt_n_output>] [-p] <starting_value>\n\n\n"
		"Argument (mandatory):\n"
		"<starting_value>: first number to calculate whether it is a prime or not.\n\n"	
		"Options:\n"
		"-b: Stop execution when a prime number is found.\n"
		"-h: show this help text.\n"
		"-n <opt_n_output>: limit amount of numbers to calculate. With -p only prime numbers will be count.\n"
		"-p: show only prime numbers.\n"
		);
	}

int main(int argc, char *argv[])
	{
	// Removing buffer to send output to stdout immediately
	setbuf(stdout, NULL);
	
	// Getting options
	// strtoul returns 0 if the string is not a number, but it returns a big
	// number if the string is a negative number (it starts with "-")
	int opt;
	int opt_prime_break = 0;
	int opt_only_prime = 0;
	unsigned long int opt_n_output = 0;
	while ((opt = getopt (argc, argv, "bhn:p")) != -1)
		switch (opt)
			{
			case 'b':
				opt_prime_break = 1;
				break;
			case 'h':
				usage();
				return 0;
			case 'n':
				opt_n_output = strtoul(optarg, NULL, RADIX);
				if (optarg[0] == '-' || opt_n_output == 0)
					{
					fprintf (stderr, "Option -n wrong. It must be a positive unsigned long integer\n");
					return 1;
					}
				break;
			case 'p':
				opt_only_prime = 1;
				break;
			default:
				usage();
				return 1;
			}	
	
	// Setting first value of "i", which will be the loop index
	mpz_t i;
	if (optind >= argc)
		{
		usage();
		return 1;
		}
	mpz_init_set_str(i, argv[optind], RADIX);
		
		
	// Validating first value of "i"
	if (mpz_cmp_ui(i, 2) < 0)
		{
		fprintf(stderr, "Fatal error: starting number must be an integer greater than or equal to 2\n");
		return 2;
		}
		
	// Checking whether "i" is equal to 2 and initialize variable to
	// count how many numbers are being calculated
	unsigned long int count = 0;
	if (mpz_cmp_ui(i, 2) == 0)
		{
		printf("\033[0;%sm2 is prime\n\033[0m", RED);
		if (opt_prime_break)
			return 0;
		if (opt_n_output > 0 && ++count >= opt_n_output)
			return 0;
		}		
	
	// Making sure that "i" is odd before starting the loop to calculate
	// numbers greater than 2
	if (mpz_even_p(i))
		mpz_add_ui(i, i, 1);
	
	// Starting the loop. "i" will be increased 2 units per interaction in
	// order to skip even numbers.
	// As well as even numbers, the ones whose last cipher is 5 are never prime
	// and will not be printed.
	unsigned int length = INIT_LENGTH;
	char* i_str = (char*) malloc(sizeof(char) * length);
	char* divisor_str = (char*) malloc(sizeof(char) * length);
	mpz_t divisor;
	mpz_init(divisor);
	for (;;)
		{
		if (mpz_sizeinbase(i, RADIX) > length)
			{
			length = mpz_sizeinbase(i, RADIX) + EXTEND_LENGTH;
			i_str = (char*) realloc(i_str, sizeof(char) * length);
        		divisor_str = (char*) realloc(divisor_str, sizeof(char) * length);
			}
		mpz_get_str(i_str, RADIX, i);
		if (i_str[strlen(i_str) - 1] == '5')
			{
			mpz_add_ui(i, i, 2);
			continue;
			}
		if (find_divisor(divisor, i))
			{
			mpz_get_str(divisor_str, RADIX, divisor);
			if (opt_only_prime)
				count--;
			else 
				printf("%s is divisible by %s\n", i_str, divisor_str);
			}
		else
			{
			printf("\033[0;%sm%s is prime\n\033[0m", RED, i_str);
			if (opt_prime_break)
				break;
			}
		//mpz_clear(divisor);
		mpz_add_ui(i, i, 2);
		if (opt_n_output > 0 && ++count >= opt_n_output)
			break;
		}
	return 0;	
	}
