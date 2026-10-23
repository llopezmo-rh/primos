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


// Colour code used for lines showing found prime numbers
#define RED "31"
#define YELLOW "93"

// Radix for numbers in string format
#define RADIX 10

// "reps" argument for mpz_probab_prime_p
// https://gmplib.org/manual/Number-Theoretic-Functions#index-mpz_005fprobab_005fprime_005fp
#define REPS 50


static void usage(void)
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
		"-o: show only prime numbers.\n"
		"-p: if a number is probably prime, notify it and continue.\n"
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
	bool opt_prime_break = false;
	bool opt_only_prime = false;
	bool opt_probab = false;
	unsigned long int opt_n_output = 0;
	while ((opt = getopt (argc, argv, "bhn:op")) != -1)
		switch (opt)
			{
			case 'b':
				opt_prime_break = true;
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
			case 'o':
				opt_only_prime = true;
				break;
			case 'p':
				opt_probab = true;
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
		
	// Checking whether "i" is equal to 2 or 5 and initialize variable to
	// count how many numbers are being calculated
	unsigned long int count = 0;
	if (mpz_cmp_ui(i, 2) == 0 || mpz_cmp_ui(i, 5) == 0)
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
	
	// Find divisors
	mpz_t divisor;
	mpz_init(divisor);
	for (;;)
		{
		// Continue if the (odd) number finishes with 5 because it will never be prime (multiple of 5)
		if (mpz_fdiv_ui(i, 10) == 5)
			{
			mpz_add_ui(i, i, 2);
			continue;
			}

		// Try with mpz_probab_prime_p before searching for a divisor
		int probab = mpz_probab_prime_p(i, REPS);
		if (probab == 2)
			{
			// gmp_printf uses the specifier %Zd
			gmp_printf("\033[0;%sm%Zd is prime\n\033[0m", RED, i);
			if (opt_prime_break)
				break;
			}
		else if (opt_probab && probab == 1)
			{
			gmp_printf("\033[0;%sm%Zd is very likely prime\n\033[0m", YELLOW, i);
			if (opt_prime_break)
				break;
			}
		else if (find_divisor(divisor, i))
			{
			if (opt_only_prime)
				count--;
			else 
				gmp_printf("%Zd is divisible by %Zd\n", i, divisor);
			}
		else
			{
			gmp_printf("\033[0;%sm%Zd is prime\n\033[0m", RED, i);
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
