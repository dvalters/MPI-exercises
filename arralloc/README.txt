Compile using "make" and run the executable "arralloctest".

Note that the second call to printlinearbuffer is INCORRECT - it is just
there to show what you should NOT do! It runs, but produces strange
results. The first call is the correct one, which is what you should do
when calling an MPI routine or the IO functions.

There is also an example of how to pass the array to a routine that does
expect a multidimensional array, where you also pass the sizes of all
the dimensions. This is what you should do when calling your own
routines.

Note that if you used the more straightforward approach of using many
separate mallocs then you could still pass the array to your own
function which expected a multidimensional array. However, you would NOT
be able to pass the data to a function expecting a single contiguous
block of memory (eg an MPI routine) as your data would be scattered
around in memory with no defined pattern. Using arralloc gets round this
problem as all the data is stored in a single block.
