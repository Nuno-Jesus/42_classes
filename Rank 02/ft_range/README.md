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