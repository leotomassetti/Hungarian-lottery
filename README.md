# Hungarian-lottery
The problem
This problem is related to the Hungarian lottery. In case you are not familiar with it: players pick 5
distinct numbers from 1 to 90. There is a weekly lottery picking event when the lotto organization
picks 5 distinct numbers randomly between 1 and 90 – just like the players did. A player’s reward
then depends on how many of the player’s numbers match with the ones selected at the lotto
picking. A player wins if they have 2, 3, 4 or 5 matching numbers.
Now, the problem: at the lottery event, right after picking the numbers, a computer shall be able to
report quickly that how many winners are in each category, for example:
Numbers matching | Winners
5                  0
4                  12
3                  818
2                  22613

This report shall be generated as soon as possible after picking the winning numbers. The players'
numbers are known a few minutes before the show starts. In peak periods, there are currently about
10 million players.
