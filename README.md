In this repository we place our implementation of OCT kernel by S. Kratsch and M. Wahlstrom, from "Representative sets and irrelevant vertices: New tools for kernelization", treated as a preprocessing routine.

It can be compiled by simple `make` command. The GNU MP library is necessary.

On the first line of an input it reads two numbers, where the first, n, is the number of vertices
and second, m, is the number of edges. The next m lines contain two integers each, where a pair stored in the k-th line
corrsepond to the vertices of the k-th edge.

On the output it returns two lines: in first there is a single integer equal to the number of undeletable vertices
and the second contains the numbers of those vertices.
