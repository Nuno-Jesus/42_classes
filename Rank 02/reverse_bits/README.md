# **The "reverse_bits" exercise**

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

## **Solution 1**

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

## **Solution 2**

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

## **Last Solution**

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
