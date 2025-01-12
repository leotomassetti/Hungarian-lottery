# Hungarian-lottery
This problem is related to the Hungarian lottery. In case you are not familiar with it: players pick 5 distinct numbers from 1 to 90. There is a weekly lottery picking event when the lotto organization picks 5 distinct numbers randomly between 1 and 90 – just like the players did. A player’s reward then depends on how many of the player’s numbers match with the ones selected at the lotto picking. A player wins if they have 2, 3, 4 or 5 matching numbers.
Now, the problem: at the lottery event, right after picking the numbers, a computer shall be able to report quickly that how many winners are in each category, for example:

| Numbers matching | Winners |
| ------------- |:-------------:
| 5             | 0             |
| 4             | 12            |
| 3             | 818           |
| 2             | 22613         |

This report shall be generated as soon as possible after picking the winning numbers. The players' numbers are known a few minutes before the show starts. In peak periods, there are currently about 10 million players.

# My Solution
The program reads the input.txt file with the bets, receives the lottery’s picks (input line identical to the file’s lines) 
and reports the result as 4 space separated numbers in the console. It runs for each new lottery’s picks until input 'end'.

- Building:
g++ lottoApp.cpp -o lottoApp

- Runing:
./lottoApp input.txt

- Stopping
end

- Asymptotic Runtime: O(N)
    For:
        N: Number of bets
        M: Numbers per bet (constant of 5)

    ReadBets: O(N)
        Reads the input file containing N bets
        Parsing each bet takes O(M)=O(5)=O(1), making the runtime dependent only on N

    Match Counting: O(N)
        countMatches: O(1) since M=5 (constant) for each bet
        threadWorker: O(N)
            Each thread processes N/T​ bets so O(T/N​)
            For all T threads, runtime is O(N)

    Thread Management: O(1) since the threads number is constant

- Ideas to improve calculation speed or handle a larger number of players:
    Parallelization Improvements: Use a thread pool or a task scheduler to dynamically balance the workload
    GPU: Use GPU for the match-counting operations (CUDA or OpenCL)
    Distributed Computing: Break the work across multiple machines
    Cloud-Based Solutions: Scale up easily by running on cloud services, adjusting power to match the size of the job
    Sorting: Pre-sort bets by certain characteristics 
