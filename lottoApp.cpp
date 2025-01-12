/*
Author: Leonardo Neto

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
*/

#include <iostream>
#include <vector>
#include <thread>
#include <array>
#include <fstream>
#include <sstream>
#include <atomic>
#include <chrono>

using namespace std;

//counts the matches for each value of the bet line returning the 
//total matches found in the lookup table 'isDrawn'
int countMatches(const array<bool, 91>& isDrawn, const vector<int>& bet) {
    int matches = 0;
    for (int num : bet) {
        //increment matches if the number is in `isDrawn`
        matches += isDrawn[num]; 
    }
    return matches;
}

//thread worker function
void threadWorker(
    const vector<vector<int>>& bets,
    const array<bool, 91>& isDrawn,
    array<int, 4>& localCounts,
    size_t startIdx,
    size_t endIdx
) {
    //loop through the startIdx and endIdx (bets to be processed by the thread)
    for (size_t i = startIdx; i < endIdx; ++i) {
        int matches = countMatches(isDrawn, bets[i]);
        //if found matches from 2 to 5, increments localCounts
        if (matches >= 2 && matches <= 5) {
            localCounts[matches - 2]++;
        }
    }
}

//reads the bets values and saves to bets vector
void readBets(const string& filename, vector<vector<int>>& bets) {
    ifstream inputFile(filename);
    string line;

    while (getline(inputFile, line)) {
        istringstream iss(line);
        vector<int> dataLine;
        int number;
        while (iss >> number) {
            dataLine.push_back(number);
        }
        bets.push_back(std::move(dataLine));
    }
    inputFile.close();
    cout << "READY\n";
}

//reads the drawn values and saves to drawn vector
void readDrawnValues(const std::string& inputLine, std::vector<int>& drawn) {
    std::istringstream inputStream(inputLine);
    for (int i = 0; i < 5; ++i) {
        inputStream >> drawn[i];
    }
    return;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << argv[0] << ": wrong arguments.\n";
        return 1;
    }

    //Gets the input file name
    string inputFileName = argv[1];

    //bets: vector of vector of bets from the input file
    vector<vector<int>> bets;

    //reads bets
    readBets(inputFileName, bets);

    //Finds the number of threads
    size_t numThreads = thread::hardware_concurrency();
    
    //calculates the total of bets to be processed by each thread
    size_t betsPerThread = bets.size() / numThreads;

    std::string inputLine;
    while (std::getline(std::cin, inputLine)) {
        //verifies program finishing token
        if (inputLine == "end") break;

        //drawn: vector of drawn values to be searched
        std::vector<int> drawn(5);

        //reads the drawn values
        readDrawnValues(inputLine, drawn);

        //initialize the lookup table 'isDrawn' for drawn numbers
        //isDrawn allows constant-time checks using array indexing.
        array<bool, 91> isDrawn = {};
        for (int num : drawn) {
            isDrawn[num] = true;
        }

        // Result counts for 2, 3, 4, and 5 matches
        array<int, 4> matchCounts = {0, 0, 0, 0};

        //start timing variable
        //auto start = chrono::high_resolution_clock::now();
        
        //prepares threads
        vector<thread> threads;
        //creates 'localCounts' a thread-local accumulator to count matches
        vector<array<int, 4>> localCounts(numThreads);

        //creates and launches threads
        for (size_t t = 0; t < numThreads; ++t) {
            size_t startIdx = t * betsPerThread;
            size_t endIdx = (t == numThreads - 1) ? bets.size() : (startIdx + betsPerThread);

            threads.emplace_back(threadWorker, cref(bets), cref(isDrawn), ref(localCounts[t]), startIdx, endIdx);
        }

        //join threads
        for (auto& thread : threads) {
            thread.join();
        }

        //aggregate final results from localCounts in matchCounts
        for (const auto& local : localCounts) {
            for (size_t i = 0; i < 4; ++i) {
                matchCounts[i] += local[i];
            }
        }
        
        //finishes timning and shows the duration
        //auto end = chrono::high_resolution_clock::now();
        //chrono::duration<double> duration = end - start;
        //long long duration_ms = chrono::duration_cast<chrono::milliseconds>(duration).count();
        //cout << duration_ms << " ms" << endl;

        //print results
        for (size_t i = 0; i < 4; ++i) {
            cout << matchCounts[i] << (i < 3 ? " " : "\n");
        }
    }

    return 0;
}
