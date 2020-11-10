Pretty fast, could be made faster by handling cases such as this in set_cell ('1' in the 3rd block must be on the left side, so in the 4th row, 1 must be in the first cell, and 7 in the 7th):

```
. . .   . . .   . 2 3
. . .   . . .   . 5 6
. . .   . . .   . 9 8

. 2 3   4 5 6   . 8 9
. . .   . . .   . . .
. . .   . . .   . . .

. . .   . . .   . . .
. . .   . . .   . . .
. . .   . . .   . . .
".......23.......56.......98.23456.89............................................."
```