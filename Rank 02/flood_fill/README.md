# **The "flood_fill" exercise**

```
Assignment name  : flood_fill
Expected files   : *.c, *.h
Allowed functions: -
--------------------------------------------------------------------------------

Write a function that takes a char ** as a 2-dimensional array of char, a
t_point as the dimensions of this array and a t_point as the starting point.

Starting from the given 'begin' t_point, this function fills an entire zone
by replacing characters inside with the character 'F'. A zone is an group of
the same character delimitated horizontally and vertically by other characters
or the array boundry.

The flood_fill function won't fill diagonally.

The flood_fill function will be prototyped like this:
  void  flood_fill(char **tab, t_point size, t_point begin);

The t_point structure is prototyped like this:

  typedef struct  s_point
  {
    int           x;
    int           y;
  }               t_point;

Example:

$> cat test.c
#include <stdlib.h>
#include <stdio.h>
#include "flood_fill.h"

char** make_area(char** zone, t_point size)
{
	char** new;

	new = malloc(sizeof(char*) * size.y);
	for (int i = 0; i < size.y; ++i)
	{
		new[i] = malloc(size.x + 1);
		for (int j = 0; j < size.x; ++j)
			new[i][j] = zone[i][j];
		new[i][size.x] = '\0';
	}

	return new;
}

int main(void)
{
	t_point size = {8, 5};
	char *zone[] = {
		"11111111",
		"10001001",
		"10010001",
		"10110001",
		"11100001",
	};

	char**  area = make_area(zone, size);
	for (int i = 0; i < size.y; ++i)
		printf("%s\n", area[i]);
	printf("\n");

	t_point begin = {7, 4};
	flood_fill(area, size, begin);
	for (int i = 0; i < size.y; ++i)
		printf("%s\n", area[i]);
	return (0);
}

$> gcc flood_fill.c test.c -o test; ./test
11111111
10001001
10010001
10110001
11100001

FFFFFFFF
F000F00F
F00F000F
F0FF000F
FFF0000F
$>
```

The subject might seem large and complicated, but if you inspect really well you'll notice a part of the subject that concerns only about how testing the function.

Let's use the previous example to gather the keypoints.

## **Keypoints**

	- The variable "zone" is a character matrix you must fill a certain zone on. 
	- You are given the coordinates (x, y) to start filling from (7, 4) in the shape of a struct called t_point. This struct contains merely 2 integers representing x and y.
	- You also receive another t_point struct containing the size of the matrix.
	Careful, as this variable does not contain the end coordinates of the filling, only the size of the matrix.
	- You can't possibly know how many characters to fill by iterating over the matrix. 
	There are too many possibilities over the matrix zones and you cannot anticipate how many cells you need to fill, unless you explore the nearby cells, from the starting point (only (L)eft, (R)ight, (D)own and U(p) as valid directions).

## **Solution**

So where do we start from? The first thing to notice is: you'll "flood" the zone given by the same characters as the starting point. For instance, in example, the coordinates `(x, y) = (7,4)` belong to the character `'0'`. That means that every adjacent `'0'` (only using valid directions) must be turned into an `'F'`.

Here's an example of a more simple map with valid and invalid directions:

<table>
	<thead>
		<tr align="center">
			<td>Start</td>
			<td>Map</td>
			<td>Valid Solution</td>
			<td>Invalid Solution</td>
		</tr>
	</thead>
	<tbody>
		<tr>
			<td>
				<pre align="center">
{2,2}
</pre>
			</td>
			<td>
				<pre align="center">
1111
1100
1010
1000
</pre >
			</td>
			<td>
				<pre align="center">
1111
1100
10F0
1000
</pre>
			</td>
			<td>
				<pre align="center">
FFFF
FF00
F0F0
F000
</pre>
			</td>
		</tr>
	</tbody>
</table>

We have another issue: **if we can't predict when to stop, how do we stop?** You'll have to use **recursion**, with a path-finding techinique called `Backtracking`. `Backtracking` is nothing more than exploring all the possible paths and go back from a path if anything goes wrong, until no more paths are to be explored or a solution is found.

**Why do we need this?** As I said earlier, you need an exploration approach, going from cell to cell exploring the 4 nearby cells, 1 in each direction.

So the pseudo-code for this exercise would be something like this:

	- If I generate invalid coordinates (negative or higher than the maximum map coordinates) I go back
	- If I find a character other than the one I started on, I go back
        - Paint the current cell with 'F'
	- Recursively paint the cells to the L, R, U and D by generating the coordinates of the nearby cells
	
Here's the source code:

```C
#include <stdlib.h>
#include <stdio.h>
#include "flood_fill.h"

/**
 * @param map The map to use to fill the given zone
 * @param size The size of the char matrix
 * @param curr The coordinates of the current cell
 * @param color The character of the starting cell
 */
void	flood_fill_recursive(char **map, t_point size, t_point curr, char color)
{
	// If I am trying to stop out of bounds, stop
	if (curr.x < 0 || curr.y < 0 || curr.x >= size.x || curr.y >= size.y)
		return ;
	// If I am exploring on a cell that has a different character from the starting one, stop
	if (map[curr.y][curr.x] != color)
		return ;

	//Paint the current cell with 'F'
	map[curr.y][curr.x] = 'F';
	flood_fill_recursive(map, size, (t_point){curr.x - 1, curr.y}, color); //L
	flood_fill_recursive(map, size, (t_point){curr.x + 1, curr.y}, color); //R
	flood_fill_recursive(map, size, (t_point){curr.x, curr.y - 1}, color); //U
	flood_fill_recursive(map, size, (t_point){curr.x, curr.y + 1}, color); //D
}

void	flood_fill(char **tab, t_point size, t_point begin)
{
	if (!tab || !size.x || !size.y || 
		begin.x < 0 || begin.y < 0 || begin.x >= size.x || begin.y >= size.y)
		return ;
	flood_fill_recursive(tab, size, begin, tab[begin.y][begin.x]);
}
```
## `WARNING` 

This line...

```C
	map[curr.y][curr.x] = 'F';
```

...must always come before this block of code:

```C
	flood_fill_recursive(map, size, (t_point){curr.x - 1, curr.y}, color); //L
	flood_fill_recursive(map, size, (t_point){curr.x + 1, curr.y}, color); //R
	flood_fill_recursive(map, size, (t_point){curr.x, curr.y - 1}, color); //U
	flood_fill_recursive(map, size, (t_point){curr.x, curr.y + 1}, color); //D
```

This is necessary since one of the conditions for the backtracking to take effect is to stop whenever it finds a cell that is different from the starting one. By painting the cells you already visit you make sure recursion doesn't re-explore everything you explored before. 

If you were to switch such order, recursion would never reach its base case, ending the program with a stack-smashing error (out of memory).

Also this verification...

```C
	if (curr.x < 0 || curr.y < 0 || curr.x >= size.x || curr.y >= size.y)
			return ;
```

... and this one ...
```C
	if (map[curr.y][curr.x] != color)
			return ;
```

cannot have their order inverted, or even mixed into a single `if` condition. Notice that if the generated coordinates are out of bounds, you cannot access the `map` variable, as you would be indexing wrong memory addresses. So, only with the right conditions you can parse the cell's color, and not otherwise. 
