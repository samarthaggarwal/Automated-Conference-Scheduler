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
    clock_t startTime = clock();
    string inputfilename(argv[1]);

    // Initialize the conference organizer.
    SessionOrganizer *organizer  = new SessionOrganizer(inputfilename);

    // Organize the papers into tracks based on similarity.
    organizer -> organizePapers();
    organizer -> printSessionOrganiser(argv[2]);

    // Score the organization against the gold standard.
    double score = organizer -> scoreOrganization();
    cout<< "score:"<<score<<endl;

    clock_t endTime = clock();
    double elapsedTime = double(endTime - startTime) / CLOCKS_PER_SEC;
    cout<<"Total time = "<<elapsedTime<<endl;
    return 0;
}
