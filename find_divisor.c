#include <gmp.h>
#include <stdbool.h>

// find_divisor stores on "divisor" the first divisor of "n" found different to 1.
// If none is found, it will set "divisor" to 1.
// It returns false if no divisor different to 1 has been found. Otherwise true.
// "divisor" must have been initialized before calling the function
bool find_divisor(mpz_t divisor, const mpz_t n)
	{
	mpz_t i, last_divisor;
	if (mpz_cmp_ui(n, 2) == 0)
		{       
		mpz_set_ui(divisor, 1);
		return false;
		}
	if (mpz_even_p(n))
		{
		mpz_set_ui(divisor, 2);
		return true;
		}     
	mpz_set_ui(divisor, 1);
	mpz_init_set_ui(i, 3);
	mpz_init(last_divisor);
	mpz_sqrt(last_divisor, n);
	while (mpz_cmp(i, last_divisor) <= 0)
		{
		if (mpz_divisible_p(n, i))
			{
			mpz_set(divisor, i);
			break;
			}
		else
			mpz_add_ui(i, i, 2);
		}
	mpz_clear(i);
	mpz_clear(last_divisor);
	return mpz_cmp_ui(divisor, 1) > 0;
	}
