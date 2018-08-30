# Automated-Conference-Scheduler
Artificial Intelligence - Assignment 1

A Conference scheduler that uses a combination of greedy hill climbing and simulated annealing to find the best schedule.
Also tried enforced hill climbing, hill climbing with uniform cost search, hill climbing with Iterative Deepening Depth First Search.

class descriptions :
session : (set of papers going on parallely)
    int *papers
    int numPapers
track : (set of sessions happening in a single time slot)
    session *sessions
    int numSessions
conference : (time slot)
    track *tracks
    int numTracks (previously parallelTracks)
    int numSessions (previously sessionsInTrack)
    int numPapers (previously papersInSession)
# additions :
- probabilistic move to bad neighbours
- restart after improvement goes below threshold

- for analysis - plot score vs time

- enforced hill climbing
- simulated enealing

in3.txt
# Observations :
better to boil at lower temperature, increase neighbors, increase break count for ghc
545109 - if(Temperature > 2)
            Temperature = 200 - itr / 4;
        else
            Temperature = 800 / itr;
	and neighbors = 250, count = 1000
545570- if(Temperature > 2)
            Temperature = 100 - itr / 5;
        else
            Temperature = 500 / itr;
	and neighbors = 250, count = 1000

maxima seen = 56136