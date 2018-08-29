/*
 * File:   main.cpp
 * Author: Kapil Thakkar
 */

#include <cstdlib>
#include "SessionOrganizer.h"
#include <ctime>

using namespace std;

int main(int argc, char** argv)
{
    clock_t start = clock();
    string inputfilename(argv[1]);

    // Initialize the conference organizer.
    SessionOrganizer *organizer  = new SessionOrganizer(inputfilename);

    // Organize the papers into tracks based on similarity.
    organizer -> organizePapers((double)start);
    organizer -> printSessionOrganiser(argv[2]);

    // Score the organization against the gold standard.
    double score = organizer -> getBestScore();
    cout << "Score: " << score << endl;

    return 0;
}
