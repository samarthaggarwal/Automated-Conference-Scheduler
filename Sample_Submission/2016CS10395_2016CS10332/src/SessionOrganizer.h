/*
 * File:   SessionOrganizer.h
 * Author: Kapil Thakkar
 */

#ifndef SESSIONORGANIZER_H
#define	SESSIONORGANIZER_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include "Conference.h"
#include "Track.h"
#include "Session.h"

using namespace std;

/*
 * SessionOrganizer reads in a similarity matrix of papers, and organizes them
 * into sessions and tracks.
 * @author Kapil Thakkar
 */
class SessionOrganizer
{
private:
    double ** distance;
    int t;
    int p;
    int k;
    double c;
    double processingTime;

    Conference *conference;
    Conference *bestConference;

public:
    SessionOrganizer();
    SessionOrganizer(string filename);

    /*
     * Read in the number of parallel tracks, papers in session, sessions
     * in a track, and the similarity matrix from the specified filename.
     * @param filename is the name of the file containing the matrix.
     * @return the similarity matrix.
     */
    void readInInputFile(string filename);

    // Updates bestConference to the current conference
    void copyConference(Conference* src, Conference* dest);

    /*
    * Initializes the conference to a random state
    */
    void randomInitialization();

    /*
     * Organize the papers according to some algorithm.
     */
    void organizePapers(double timer);

    /*
     * Organize the papers using Greedy Hill Climbing
     */
    void organizePapersGreedyHillClimbing(double timer, int numNeighbours);

    /*
     * Organize the papers using Simulated Annealing
     */
    void organizePapersSimulatedAnnealing(double timer, double heat);

    /*
     * Perform iddfs from local localOptima
     */
     void organizePapersIdDFS(double timer);

    /*
     * Get the distance matrix.
     * @return the distance matrix.
     */
    double** getdistance();

    /*
     * Score the organization.
     * @return the score.
     */
    double scoreOrganization();

    // Gets the score of best conference
    double getBestScore();

    /*
    * Calculates the change in score on a single swap
    *
    */
    double swapCostChange(int trackIndex1, int sessionIndex1, int paperIndex1, int trackIndex2, int sessionIndex2, int paperIndex2);

    void printSessionOrganiser(char *);
};

#endif	/* SESSIONORGANIZER_H */
