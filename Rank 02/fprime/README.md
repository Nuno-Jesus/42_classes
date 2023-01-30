# **The fprime exercise**

```
Assignment name  : fprime
Expected files   : fprime.c
Allowed functions: printf, atoi
--------------------------------------------------------------------------------

Write a program that takes a positive int and displays its prime factors on the
standard output, followed by a newline.

Factors must be displayed in ascending order and separated by '*', so that
the expression in the output gives the right result.

If the number of parameters is not 1, simply display a newline.

The input, when there is one, will be valid.

Examples:

$> ./fprime 225225 | cat -e
3*3*5*5*7*11*13$
$> ./fprime 8333325 | cat -e
3*3*5*5*7*11*13*37$
$> ./fprime 9539 | cat -e
9539$
$> ./fprime 804577 | cat -e
804577$
$> ./fprime 42 | cat -e
2*3*7$
$> ./fprime 1 | cat -e
1$
$> ./fprime | cat -e
$
$> ./fprime 42 21 | cat -e
$
```

## **Solution**

Your task is to display the prime numbers that multiplied can give us the original number. It seems you have a lot to think about in this subject. Let's split it into tasks:

	- First things first, validate the arguments. Only perform calculations if you received 1 argument (aside from the executable)
	
	- Second we should find which numbers are prime (or should we?). Remember that the prime numbers begin in 2 and the input is ALWAYS valid (n > 0)

	- Finally, we must deal with the format they want (Number1*Number2* ...) and stop printing '*' when the number is no longer divisible by any number

I believe dealing with the arguments is pretty straight forward to you, so I'm skipping that part.

Given a number `n`, we can follow this pseudo-code:

	- For i starting at 2 and ending at n (including n):
		- While n is divisible by i
			- Divide n by i
			- Print the number i
			- If n reaches 1 (case where there are no more divisions to perform)
				- Stop the program
			- Otherwise:
				- Print '*' (to proceed to the next number)

And that works! But how does it work? Noticed I'm not calculating the prime numbers at all? Let's take `42` as an example. 

``` C
	42 = 6*7 = 14*3 = 21*2 = 2*3*7
```

The number `42` is divisible by `6`, which is not a prime number. But if that's so, why isn't the above algorithm also printing `6`? Because we start in the first prime number (`2`) and `42` is divisible by `2`, then we divide it by `2`, cutting off many numbers that would divide `42`, just like `6`. Here's a more schematic way to see it:

|Iteration| Value of n | Possible dividers | Next operation
|:-------:|:----------:|:-----------------:|:--------------:
|0        |42          |2, 3, 6, 7, 14, 21 | 42 / 2 = 21
|1        |21          |3, 7, 21           | 21 / 3 = 7
|2        |7           |7                  | 7 / 7 = 1

In iteration 1, `6` is no longer a valid divider, because we divided `42` by `2`, and `6` needs `n` to be both divisible by `2` and `3` at the same time to be divisible by `6`! That's why you don't need to calculate the prime numbers at all: because the divisions from `2` to `n` already remove non-prime numbers!

Here's the source code:

```C
#include <stdio.h>
#include <stdlib.h>

void fprime(int n)
{
	// This needs to be here, otherwise this will never print 1 when n is 1...
	if (n == 1)
		printf("1");

	for (int i = 2; i <= n; i++)
	{
		while (n % i == 0)
		{
			printf("%d", i);
			n /= i;
			if (n == 1)
				return ;
			printf("*");
		}
	}
}

int main (int argc, char **argv)
{
	if (argc - 1 == 1)
		fprime(atoi(argv[1]));
	printf("\n");
}
```

## **Tests**

I believe the subject already provides you many useful tests, but here are a couple more:

```
$> ./fprime 2147483647 | cat -e
2147483647$
$> ./fprime 2147483646 | cat -e
2*3*3*7*11*31*151*331$
$> ./fprime 45 | cat -e
3*3*5$
$> ./fprime 90 | cat -e
2*3*3*5$
$> ./fprime 64 | cat -e
2*2*2*2*2*2$
$> ./fprime 1 | cat -e
1$
```