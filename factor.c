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
#include <string.h>
#include <gmp.h>


// Radix of numbers expressed as strings
#define RADIX 10


// print_power prints a power with the format "(<base>^<exponent)". If the
// exponent is 1, it just prints the base
static void print_power(const mpz_t base, const mpz_t exp)
    {
    if (mpz_cmp_ui(exp, 1) == 0)
        mpz_out_str(stdout, RADIX, base);
    else
        {
        printf("(");
        mpz_out_str(stdout, RADIX, base);
        printf("^");
        mpz_out_str(stdout, RADIX, exp);
        printf(")");
        }
    }

int main(int argc, char *argv[])
    {
    // Variable to store the exponent to be printed
    mpz_t exp;

    // Variable to keep the number whose divisor will be searched for.
    // Initialized to the CLI argument
    mpz_t dividend;

    // When a divisor of "dividend" is found, it is stored on "divisor"
    mpz_t divisor;

    // Variable to keep the divisor found on the previous iteraction of the
    // loop. When a new divisor is found and stored in "divisor", it will be
    // compared with "previous divisor". If both values are the same, "exp"
    // will be increased. Otherwise, "previous_divisor" will be printed with
    // its corresponding exponent ("exp"), the value of "previous_divisor"
    // will be overwritten with the one of "divisor" and "exp" will be
    // reinitialized to 1.
    mpz_t previous_divisor;

    // Removing buffer in order to send output to stdout immediately
    setbuf(stdout, NULL);

	// Check arguments
	if (argc != 2)
		{
		fprintf(stderr,"Usage: <executable_file> <natural_number>\n");
		return 1;
		}

    // Initializing and validating variables
    if (mpz_init_set_str(dividend, argv[1], RADIX) == -1)
        {
        fprintf(stderr, "Fatal error: \"%s\" does not seem to be a correct "
            "integer value\n", argv[1]);
        return 1;
        }
    if (mpz_cmp_ui(dividend, 1) < 0)
        {
        fprintf(stderr, "Fatal error: only values greater than 0 can be "
            "factored\n");
        return 1;
        }

    // Initializing
    mpz_init(exp);
    mpz_init_set_ui(previous_divisor, 0);
    mpz_init(divisor);

    // Printing CLI argument, which will be the number to be factored
    printf("%s = ", argv[1]);

    // Loop where factors will be found and printed
    for(;;)
        {
        if (find_divisor(divisor, dividend))
            {
            if (mpz_cmp(previous_divisor, divisor) != 0)
                {
                if (mpz_cmp_ui(previous_divisor, 0) != 0)
                    {
                    print_power(previous_divisor, exp);
                    printf(" * ");
                    }
                mpz_set(previous_divisor, divisor);
                mpz_set_ui(exp, 1);
                }
            else
                mpz_add_ui(exp, exp, 1);
            mpz_cdiv_q(dividend, dividend, divisor);
            }
        else
            {
		  if (mpz_cmp(dividend, previous_divisor) == 0)
                mpz_add_ui(exp, exp, 1);
            else
                {
                if (mpz_cmp_ui(previous_divisor, 0) != 0)
                    {
                    print_power(previous_divisor, exp);
                    printf(" * ");
                    }
                // As this dividend is prime and it is different to the
			 // previous divisor found, the exponent is set to 1 so
			 // that only the number is printed before the loop
			 // is finished
                mpz_set_ui(exp, 1);
                }
            print_power(dividend, exp);
            printf("\n");
            // As a prime factor has been found, the loop has to be finished
            break;
            }
        }
    }
