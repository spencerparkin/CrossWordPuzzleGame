# CrossWordPuzzleGame

This program can generate crossword puzzles.  You can then try to solve them by
selecting the word locations and typing out the letters.  Hints are given by
querying freedictionaryapi.com.  A screenshot follows.

![snapshot](https://github.com/spencerparkin/CrossWordPuzzleGame/blob/main/screen_shot.png?raw=true)

The algorithm for generating the puzzle is interesting and was the impetus for the project.
I went through several different ideas, none of which panned out.  A breakthrough came when
I realized that instead of letting the shape of the puzzle be a consequence of the words chosen,
I needed to flip that around.  That is, the shape is chosen first, and then the words are chosen
to fit the shape.  This means that I first decide which locations of the matrix are to hold a letter,
and which are to be blocked out (or black.)  This makes the words selection process easier, because
the words lengths, locations and orientations are now all determined.  What remains, then, is just a
question of whether there exists a set of words that will fit this arrangement.

So I started with the naive algorithm that, if a solutions exists, is guarenteed to find it, provided
the computer can run indefinitely.  Of course, there-in lies the problem.  If the algorithm doesn't
terminate in a reasonable amount of time, then even if it's correct, it's impractical.  Interestingly
enough, this was still the right approach, because optimizations could be found.

So what was the naive algorithm?  It was simply to try all possible words in a location, and
for each one, recursively try words in other locations.  So you're just doing a search in a
massive tree of possibilities and backtracking when necessary.  The trick is two fold.  First,
you should cache and hit that cache whenever possible so as not to duplciate work.  Second,
there is a really important way to cull branches of the search tree.  At any given point in
the search, there is no point is going deeper into the tree if among the set of partially
completed word locations, any one of them has zero associated possibilities.  If you don't detect
this, then you will be descending the tree from this point in vain and wasting a lot of time.

And with that, the algorithm is surprising fast and successful.  I haven't yet run into a case
where it gets stuck.  Before the optimizations, it was getting stuck all the time.  I can't be
completely sure this is fool-proof, but it seems to work pretty well so far.