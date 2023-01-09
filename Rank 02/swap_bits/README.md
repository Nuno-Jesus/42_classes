# **The "swap_bits" exercise**

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

## **Solution 1**

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

## **Last solution**

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
