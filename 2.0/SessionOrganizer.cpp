/*
 * File:   SessionOrganizer.cpp
 * Author: Kapil Thakkar
 *
 */

#include "SessionOrganizer.h"
#include "Util.h"

#include <stdlib.h>
#include <time.h>

SessionOrganizer::SessionOrganizer()
{
    p = 0;
    k = 0;
    t = 0;
    c = 1.0;
    processingTime = 0;
}

SessionOrganizer::SessionOrganizer(string filename)
{
    readInInputFile(filename);
    conference = new Conference(p, t, k);
}

void SessionOrganizer::organizePapers()
{
    // number of neighbours from which to select the next node
    int numNeighbours = 10;

    // to initialize random seed
    srand(time(NULL));

    int exchangeIndices[numNeighbours][6];
    // order = trackIndex1, sessionIndex1, paperIndex1, trackIndex2, sessionIndex2, paperIndex2;

    double *scoreChange = (double*) malloc(numNeighbours * sizeof(double));
    double maxScoreChange;
    int maxScoreIndex;

    for(int i=0;i<numNeighbours;i++){
        exchangeIndices[i][0] = rand() % (conference -> gett()) ;
        exchangeIndices[i][1] = rand() % (conference -> getp()) ;
        exchangeIndices[i][2] = rand() % (conference -> getk()) ;

        exchangeIndices[i][3] = rand() % (conference -> gett()) ;
        exchangeIndices[i][4] = rand() % (conference -> getp()) ;
        exchangeIndices[i][5] = rand() % (conference -> getk()) ;

        double[i] = swapCostChange(exchangeIndices[i][0],exchangeIndices[i][1],exchangeIndices[i][2],exchangeIndices[i][3],exchangeIndices[i][4],exchangeIndices[i][5],);
    }

    maxScoreChange = scoreChange[0];
    maxScoreIndex = 0;
    for(int i=1;i<numNeighbours; i++){
        if(maxScoreChange<scoreChange[i]){
            maxScoreChange = scoreChange[i];
            maxScoreIndex = i;
        }
    }

}

void SessionOrganizer::organizePapers()
{
    int paperCounter = 0;
    for(int i = 0; i < conference -> gett(); i++)
    {
        for(int j = 0; j < conference -> getp(); j++)
        {
            for (int k = 0; k < conference -> getk(); k++ )
            {
                conference -> setPaper(j, i, k, paperCounter);
                paperCounter++;
            }
        }
    }
}

void SessionOrganizer::readInInputFile(string filename)
{
    vector<string> lines;
    string line;
    ifstream myfile(filename.c_str());
    if (myfile.is_open())
    {
        while(getline(myfile, line))
        {
            lines.push_back ( line );
        }
        myfile.close();
    }

    processingTime = atof(lines[0].c_str());
    k = atoi(lines[1].c_str());
    p = atoi(lines[2].c_str());
    t = atoi(lines[3].c_str());
    c = atof(lines[4].c_str());

    int n = lines.size() - 5;
    distance = new double*[n];
    for(int i = 0; i < n; i++)
    {
        distance[i] = new double[n];
    }

    for(int i = 0; i < n; i++)
    {
        string tempLine = lines[i + 5];
        string elements[n];
        splitString(tempLine, " ", elements, n);

        for (int j = 0; j < n; j++)
        {
            distance[i][j] = atof(elements[j].c_str());
        }
    }
}

double** SessionOrganizer::getdistance()
{
    return distance;
}

void SessionOrganizer::printSessionOrganiser(char * filename)
{
    conference -> printConference(filename);
}

double SessionOrganizer::scoreOrganization()
{
    // Sum of pairwise similarities per session.
    double score1 = 0.0;
    for(int i = 0; i < conference -> getp(); i++)
    {
        Track tmpTrack = conference -> getTrack(i);
        for(int j = 0; j < tmpTrack.getNumberOfSessions(); j++)
        {
            Session tmpSession = tmpTrack.getSession(j);
            for(int l = 0; l < tmpSession.getNumberOfPapers(); l++)
            {
                int index1 = tmpSession.getPaper(l);
                for(int m = l + 1; m < tmpSession.getNumberOfPapers(); m++)
                {
                    int index2 = tmpSession.getPaper(m);
                    score1 += 1 - distance[index1][index2];
                }
            }
        }
    }

    // Sum of distances for competing papers.
    double score2 = 0.0;
    for(int i = 0; i < conference -> getp(); i++ )
    {
        Track tmpTrack1 = conference -> getTrack(i);
        for (int j = 0; j < tmpTrack1.getNumberOfSessions(); j++)
        {
            Session tmpSession1 = tmpTrack1.getSession(j);
            for(int l = 0; l < tmpSession1.getNumberOfPapers(); l++)
            {
                int index1 = tmpSession1.getPaper(l);

                // Get competing papers.
                for(int m = i + 1; m < conference -> getp(); m++)
                {
                    Track tmpTrack2 = conference -> getTrack(m);
                    Session tmpSession2 = tmpTrack2.getSession(j);
                    for(int n = 0; n < tmpSession2.getNumberOfPapers(); n++)
                    {
                        int index2 = tmpSession2.getPaper(n);
                        score2 += distance[index1][index2];
                    }
                }
            }
        }
    }

    double score = score1 + c * score2;
    return score;
}
