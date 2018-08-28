# Automated-Conference-Scheduler
Artificial Intelligence - Assignment 1


class descriptions :
session : (set of papers going on parallely)
  int *papers;
  int numPapers;


track : (set of sessions happening in a single time slot)
    session *sessions;
    int numSessions;

conference : (time slot)
    track *tracks;
    int numTracks; (previously parallelTracks)
    int numSessions; (previously sessionsInTrack)
    int numPapers; (previously papersInSession)


additions :
- probabilistic move to bad neighbours
- restart after improvement goes below threshold

- for analysis - plot score vs time

- enforced hill climbing
- simulated enealing


maxima seen = 55978.9
