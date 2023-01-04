# Rank 02 Exam Class Exercises

## **The "print_bits" exercise**
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

### **Solution 1**

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

### **Solution 2**

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

### **Last solution**

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

-------------------------

## **The "swap_bits" exercise**

```
Assignment name  : swap_bits
Expected files   : swap_bits.c
Allowed functions:
--------------------------------------------------------------------------------

Write a function that takes a byte, swaps its halves (like the example) and
returns the result.

Your function must be declared as follows:

unsigned char	swap_bits(unsigned char octet);

Example:

  1 byte
_____________
 0100 | 0001
     \ /
     / \
 0001 | 0100
```

For this one the strategies are limited, unless you create a really complicated and confusing program to do it (keep it simple).

### **Solution 1**

**1.** First we're gonna use the binary number `1111 0000` and the `&` operator to extract the left half of the byte. Don't forget that this is not enough. We need to take this half and right-shift 4 times to place it in the right position. Lets use the example above:

		  0100 0001	(octet)
		& 1111 0000	(auxiliary binary)
		============
		  0100 0000	(left half)

		  0100 0000	(left half)
		  0000 0100	(left half >> 4)

**2.** Now we need to do the same to the right half. This time we must use the `0000 1111` binary and left-shift the result 4 times:

		  0100 0001	(octet)
		& 0000 1111	(auxiliary binary)
		============
		  0000 0001	(right half)

		  0000 0001	(right half)
		  0001 0000	(right half << 4)

**3.** Now all we need to now is to merge the two results. For that, you **MUST** use the `|` operator. You cannot use the `&` operators, since `1 & 0` will never produce the 1's you need.

		   0000 0100	(left half >> 4)
		|  0001 0000	(right half << 4)
		============
		   0001 0100	(output)

Here's the source code:

```C
unsigned char	swap_bits(unsigned char octet)
{
	unsigned char left;
	unsigned char right;

	left = (octet & 0b11110000) >> 4;	//You can use 0xF0 instead of the binary
	right = (octet & 0b00001111) << 4;	//You can use 0x0F instead of the binary
	
	return (left | right);
}
```

... or simply:

```C
unsigned char	swap_bits(unsigned char octet)
{
	return (((octet & 0b11110000) >> 4) | ((octet & 0b00001111) << 4));
}
```

### **Last solution**

This is a variant of the first solution: you don't need to use the `&` operator at all. You can do as follows:

```C
unsigned char	swap_bits(unsigned char octet)
{
	unsigned char left;
	unsigned char right;

	left = octet >> 4;
	right = octet << 4;
	
	return (left | right);
}
```

... or simply:

```C
unsigned char	swap_bits(unsigned char octet)
{	
	return ((octet >> 4) | (octet << 4));
}
```

-------------------------

## **The "reverse_bits" exercise**

```
Assignment name  : reverse_bits
Expected files   : reverse_bits.c
Allowed functions:
--------------------------------------------------------------------------------

Write a function that takes a byte, reverses it, bit by bit (like the
example) and returns the result.

Your function must be declared as follows:

unsigned char	reverse_bits(unsigned char octet);

Example:

  1 byte
_____________
 0010  0110
	 ||
	 \/
 0110  0100
```

Let's take a look at possibilities to solve this:

### **Solution 1**

By using the module and division by 2 we can successfully extract the LSB from the original octet and push it into the new variable.

**1.** We store the result of `octet % 2`, which will give us the LSB from the original octet. Using the number 39 (0010 0111) as an example:

	0010 0111	(octet)
	0000 0001	(octet % 2)
	
**2.** We then use the new variable (which I called `bits`) and left-shift it 1 time, to make space for the new bit. In the first iteration this change the value inside, since it will be 0 anyway, but this will of major importance.

**3.** We "concatenate" the bit we extracted in step 1 with our `bits` variable using the `|` operator

		 0000 0000 (bits)
	   | 0000 0001 (octet % 2)
	   ============
	     0000 0001	

**4.** We right-shift our original octet to discard the bit we just used:

		0010 0111	(octet)
		0001 0011	(octet >> 1)

**5.** Repeat this process 8 times, 1 for each bit. 

Here's a representation of what will happen over each iteration using the binary of 39 (0010 0111):

| Iteration | Bits | Octet | Octet % 2 | (Bits << 1) \| (Octet % 2) | Octet >> 1 |
|:--:|:--:|:--:|:--:|:--:|:--:|
| **0** | 0000 0000 | `0010 0111` | 1 | 0000 0001 | 0001 0011 |
| **1** | 0000 0001 | 0001 0011 | 1 | 0000 0011 | 0000 1001 |
| **2** | 0000 0011 | 0000 1001 | 1 | 0000 0111 | 0000 0100 |
| **3** | 0000 0111 | 0000 0100 | 0 | 0000 1110 | 0000 0010 |
| **4** | 0000 1110 | 0000 0010 | 0 | 0001 1100 | 0000 0001 |
| **5** | 0001 1100 | 0000 0001 | 1 | 0011 1001 | 0000 0000 |
| **6** | 0011 1001 | 0000 0000 | 0 | 0111 0010 | 0000 0000 |
| **7** | 0111 0010 | 0000 0000 | 0 | `1110 0100` | 0000 0000 |

Source code:

```C
unsigned char	reverse_bits(unsigned char octet)
{
	unsigned char bits = 0;
	unsigned char aux;

	for (int i = 0; i < 8; i++)
	{
		aux = octet % 2;
		bits = bits << 1;
		bits = bits | aux;
		octet = octet >> 1;
	}

	return bits;
}
```

... or simply:

```C
unsigned char	reverse_bits(unsigned char octet)
{
	unsigned char bits = 0;

	/* Usually you use only change the counter variable in the for loop header. In 
	this case I used 2, separated by a comma to indicate that I want to run two 
	commands each iteration: increment the counter and right-shift our octet 1 time */
	for (int i = 0; i < 8; i++, octet >>= 1)
		bits = (bits << 1) | (octet % 2);

	return bits;
}
```

### **Solution 2**

Instead of left-shifting our `bits` variable, one could think of taking bit by bit, from position 7 to 0. Take a look:

```C
#define BIT(n) 1 << n

unsigned char	reverse_bits(unsigned char octet)
{
	unsigned char bits = 0;
	unsigned char aux;

	for (int i = 7; i >= 0; i--)
	{
		aux = (octet & BIT(i)) >> i; 	// Take the bit of index i
		bits = bits | (aux << (7 - i)); //Place it backwards on the bits variable
	}
		
	return bits;
}
```

### **Last Solution**

If you really don't want to think of loops, here's a REALLY hard-coded solution:

```C
#define BIT(n) 1 << n

unsigned char	reverse_bits(unsigned char octet)
{
	return (
		((octet & BIT(7)) >> 7) | ((octet & BIT(6)) >> 5) |
		((octet & BIT(5)) >> 3) | ((octet & BIT(4)) >> 1) |
		((octet & BIT(3)) << 1) | ((octet & BIT(2)) << 3) |
		((octet & BIT(1)) << 5) | ((octet & BIT(0)) << 7) 
	);
}
```

This is basically like un-rolling a `for` loop and placing each iteration on the `return` statement.

## **The "ft_range" exercise**

```
Assignment name  : ft_range
Expected files   : ft_range.c
Allowed functions: malloc
--------------------------------------------------------------------------------

Write the following function:

int     *ft_range(int start, int end);

It must allocate (with malloc()) an array of integers, fill it with consecutive
values that begin at start and end at end (Including start and end !), then
return a pointer to the first value of the array.

Examples:

- With (1, 3) you will return an array containing 1, 2 and 3.
- With (-1, 2) you will return an array containing -1, 0, 1 and 2.
- With (0, 0) you will return an array containing 0.
- With (0, -3) you will return an array containing 0, -1, -2 and -3.
```

**Solution**

1. If you're gonna return a dynamically allocated array, the first thing to know is the number of elements your array is gonna take. Simply notice that you achieve that by subtracting the `end` and the `start` parameter and add 1. 

```C
	int size = end - start + 1;
```

Notice that not always this will work. If `start` > `end`, the subtraction will turn out negative, meaning we **need to compute the absolute result** of that subtraction and THEN add 1. When you have this done, allocate the array.

2. Now we can fill the array with the elements we need. Notice that if start > end (for instance start = 3 and end = 1) we need to do it backwards. Otherwise we can count upwards. So make sure you check this before you fill the array.

Here's the code to do so:

```C
/**
 * @brief Given a number n, it returns its absolute value
 * @param n The number to return the absolute value from
 * @return The absolute value of n
 */
int ft_abs(int n)
{
	// This is a ternary operator, used to squish the "if else" notation
	return n < 0 ? -n : n;
}

int     *ft_range(int start, int end)
{
	int range = ft_abs(start - end) + 1;
	int *array;

	if (!(array = malloc(range * sizeof(int))))
		return NULL;

	//Count upwards
	if (start < end)
		for (int i = 0; i < range; i++)
			array[i] = start + i; 
	//Count downwards
	else
		for (int i = 0; i < range; i++)
			array[i] = start - i;
	
	return array; 
}

/* 
void print_array(int *array, size_t size)
{
	for (size_t i = 0; i < size; i++)
		printf("%d ", array[i]);
	puts("");
}

int main(int argc, char **argv)
{
	(void)argc;

	int start = atoi(argv[1]);
	int end = atoi(argv[2]);

	int *array = ft_range(start, end);
	print_array(array, ft_abs(start - end) + 1);
	free(array);
} 
*/
```

... or simply:

```C

#define ABS(n) (n < 0) ? -(n) : (n)

int     *ft_range(int start, int end)
{
	int *array;
	int range = ABS(start - end);

	if (!(array = malloc((range + 1) * sizeof(int))))
		return NULL;

	if (start < end)
		for (int i = 0; i < range + 1; i++)
			array[i] = start + i; 
	else
		for (int i = 0; i < range + 1; i++)
			array[i] = start - i;
	return array; 
}
/* 
void print_array(int *array, size_t size)
{
	for (size_t i = 0; i < size; i++)
		printf("%d ", array[i]);
	puts("");
}

int main(int argc, char **argv)
{
	(void)argc;

	int start = atoi(argv[1]);
	int end = atoi(argv[2]);
	int range = ABS(start - end);

	int *array = ft_range(start, end);
	print_array(array, range + 1);
	free(array);
}*/
```