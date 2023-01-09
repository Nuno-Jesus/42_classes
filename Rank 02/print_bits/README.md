# **The "print_bits" exercise**
```
Assignment name  : print_bits
Expected files   : print_bits.c
Allowed functions: write
--------------------------------------------------------------------------------

Write a function that takes a byte, and prints it in binary WITHOUT A NEWLINE
AT THE END.

Your function must be declared as follows:

void	print_bits(unsigned char octet);

Example, if you pass 2 to print_bits, it will print "00000010"
```

## **Solution 1**

We can extract the **Most Significant Bit (MSB)**, print it and left-shift the octet 8 times, one per each bit to print. Using the number 168 (1010 1000) the procedure is as follows:

**1.** We use the number 1, left-shifted 7 times (1 << 7), or in other words: 1000 0000
	
	0000 0001	(1)
	1000 0000	(1 << 7)

**2.** We take the octet we want to print and we use the & operator between 1 << 7 and the number 168. The rightmost zeros will nullify all the bits we don't care about (for now) and the 1 will take the MSB from the octet (which is zero).

	  1010 1000	(168)
	& 1000 0000	(1 << 7)
	=====================
	  1000 0000 (result)

**3.** Since we need to print the result of the previous operation and we need to print either a 1 or a 0 (1 in this case), we can't simply output this raw result. The number 1 must be shifted to the position of the **Least Significant Bit (LSB)** which the position of index 0. To do this, we simply right-shift the result 7 times:

	1000 0000	(result)
	0000 0001	(result >> 7)

**4.** Now we can print the bit and go onto the next one. How do we do this? Well, we simply left-shift the original octet 1 time, like this:

	1010 1000	(original binary)
	0101 0000	(1 left shift)

**5.** Repeat this procedure 8 times, 1 per each bit and you'll get the right output

This is the first solution code: 

```C
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MSB 1 << 7

void	print_bits(unsigned char octet)
{
	unsigned char c;
	
	for (int i = 0; i < 8; i++)
	{
		c = ((octet & MSB) >> 7) + '0';
		write(STDOUT_FILENO, &c, 1);
		octet = octet << 1;
	}
}
```

## **Solution 2**

Another way of doing it, is not modifying the original octet, but the number 1 we were using to get the MSB. This time, instead of always left-shifting the number 1, 7 times, we will shift it `i` times, where `i` is the index of the bit we need.

Over the iterations, the number 1 will look like this:

| Value of i | Shift Representation | Binary | Decimal Representation|
|:--:|:--:|:--:|:--:|
| **7** | 1 << 7 | `1`000 0000 | 128 |
| **6** | 1 << 6 | 0`1`00 0000 |  64 |
| **5** | 1 << 5 | 00`1`0 0000 |  32 |
| **4** | 1 << 4 | 000`1` 0000 |  16 |
| **3** | 1 << 3 | 0000 `1`000 |   8 |
| **2** | 1 << 2 | 0000 0`1`00 |   4 |
| **1** | 1 << 1 | 0000 00`1`0 |   2 |
| **0** | 1 << 0 | 0000 000`1` |   1 |

Here's the source code:

```C
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BIT(n) 1 << n

void	print_bits(unsigned char octet)
{
	unsigned char c;
	
	for (int i = 7; i >= 0; i--)
	{
		c = ((octet & BIT(i)) >> i) + '0';
		write(STDOUT_FILENO, &c, 1);
	}
}
```

## **Last solution**

The last solution I want to bring here is a recursive one. Although you don't really need recursion for this subject, since you always now the number of digits you need to print, you might find it easier to understand because of the way you now the `putnbr` function. These are the steps:

**1.** We take the octet and pass it to an auxiliary recursive function. That function will receive the `octet` and a `counter`. The `counter` will be responsible for representing the base case of the recursion. The `counter` will be increment over the recursive calls until it reaches the base case (index 7 of the byte).

**2.** We recursively call the function, passing `octet / 2` and `counter + 1`, since we need to cut out bit by bit (just like in putnbr we need to cut out digit by digit).

**3.** When we reach the base case (counter = 7), we call the `write()` function to print `octet % 2` and backtrack from all the recursive calls, printing all the bits.

Here's the source code:

```C
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void	print_bits_recursive(unsigned char octet, int counter)
{
	unsigned char c = (octet % 2) + '0';
	
	if (counter < 7)
		print_bits_recursive(octet / 2, counter + 1);
	write(STDOUT_FILENO, &c, 1);
}

void	print_bits(unsigned char octet)
{
	print_bits_recursive(octet, 0);
}
```
