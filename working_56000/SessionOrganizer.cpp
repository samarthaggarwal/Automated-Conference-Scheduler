/*
 * File: SessionOrganizer.cpp
 * Author: Kapil Thakkar
 */

#include "SessionOrganizer.h"
#include "Util.h"
#include <math.h>
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
    conference = new Conference(t, p, k);
    bestConference = new Conference(t, p, k);
}

void SessionOrganizer::updateBestConference()
{
    for(int i = 0; i < t; i++){
        for(int j=0;j<p;j++){
            for(int l=0;l<k;l++){
                bestConference -> setPaper(i, j, l, conference -> getTrack(i).getSession(j).getPaper(l) );
            }
        }
    }
    bestConference -> setScore(conference -> getScore());
}

void SessionOrganizer::randomInitialization()
{
    int t = conference->gett();
    int p = conference->getp();
    int k = conference->getk();
    int numPapers = t*p*k;

    bool papersAssigned[numPapers] = {0};
    int randPaper;

    for(int i = 0; i < t; i++)
        for(int j = 0; j < p; j++)
            for(int l=0;l<k;l++)
            {
                randPaper = rand() % numPapers;
                while(papersAssigned[randPaper] == true)
                    randPaper = (randPaper + 1) % numPapers;
                papersAssigned[randPaper] = true;
                conference -> setPaper(i, j, l, randPaper);
            }
    conference -> setScore(scoreOrganization());
    updateBestConference();
}

void SessionOrganizer::organizePapers(double timer)
{
    srand(time(NULL));
    randomInitialization();
    int numNeighbours = 225;
    while(true)
    {
        organizePapersGreedyHillClimbing(timer, numNeighbours += 15);
        organizePapersSimulatedAnnealing(timer);
        if((((double)clock() - timer) / CLOCKS_PER_SEC) > (60 * processingTime - 0.1))
            return;
    }
}

void SessionOrganizer::organizePapersGreedyHillClimbing(double timer, int numNeighbours)
{
    int exchangeIndices[numNeighbours][6];
    double *scoreChange = (double*) malloc((numNeighbours) * sizeof(double));

    double maxScoreChange;
    int maxScoreIndex;

    double random;

    for(int count = 0; ; count++)
    {
        for(int i = 0; i < numNeighbours; i++)
        {
            exchangeIndices[i][0] = rand() % (conference -> gett());
            exchangeIndices[i][1] = rand() % (conference -> getp());
            exchangeIndices[i][2] = rand() % (conference -> getk());

            do
            {
                exchangeIndices[i][3] = rand() % (conference -> gett());
                exchangeIndices[i][4] = rand() % (conference -> getp());
                exchangeIndices[i][5] = rand() % (conference -> getk());
            } while(exchangeIndices[i][0] == exchangeIndices[i][3] && exchangeIndices[i][1] == exchangeIndices[i][4]);

            scoreChange[i] = swapCostChange(exchangeIndices[i][0], exchangeIndices[i][1], exchangeIndices[i][2], exchangeIndices[i][3], exchangeIndices[i][4], exchangeIndices[i][5]);
        }

        maxScoreChange = -INFINITY;
        for(int i = 0; i < numNeighbours; i++)
            if(maxScoreChange < scoreChange[i])
            {
                maxScoreChange = scoreChange[i];
                maxScoreIndex = i;
            }

        if(maxScoreChange >= 0)
        {
            count = 0;
            int paperId1 = conference -> getTrack(exchangeIndices[maxScoreIndex][0]).getSession(exchangeIndices[maxScoreIndex][1]).getPaper(exchangeIndices[maxScoreIndex][2]);
            int paperId2 = conference -> getTrack(exchangeIndices[maxScoreIndex][3]).getSession(exchangeIndices[maxScoreIndex][4]).getPaper(exchangeIndices[maxScoreIndex][5]);

            conference -> setPaper(exchangeIndices[maxScoreIndex][0], exchangeIndices[maxScoreIndex][1], exchangeIndices[maxScoreIndex][2], paperId2);
            conference -> setPaper(exchangeIndices[maxScoreIndex][3], exchangeIndices[maxScoreIndex][4], exchangeIndices[maxScoreIndex][5], paperId1);

            conference -> setScore(conference -> getScore() + maxScoreChange);
            // cout << conference -> getScore() << endl;
        }

        if(conference-> getScore() > bestConference -> getScore())
            updateBestConference();

        if(count == 1000)
            break;

        if((((double)clock() - timer) / CLOCKS_PER_SEC) > (60 * processingTime - 0.1))
            return;
    }
}

void SessionOrganizer::organizePapersSimulatedAnnealing(double timer)
{
    int exchangeIndices[6];

    double scoreChange;
    double Temperature = INFINITY;

    double random;

    for(int itr = 1; Temperature > 0.01; itr++)
    {
        if(Temperature > 2)
            Temperature = 100 - itr / 5;
        else
            Temperature = 500 / itr;

        exchangeIndices[0] = rand() % (conference -> gett());
        exchangeIndices[1] = rand() % (conference -> getp());
        exchangeIndices[2] = rand() % (conference -> getk());

        do
        {
            exchangeIndices[3] = rand() % (conference -> gett()) ;
            exchangeIndices[4] = rand() % (conference -> getp()) ;
            exchangeIndices[5] = rand() % (conference -> getk()) ;
        } while(exchangeIndices[0] == exchangeIndices[3] && exchangeIndices[1] == exchangeIndices[4]);

        scoreChange = swapCostChange(exchangeIndices[0], exchangeIndices[1], exchangeIndices[2], exchangeIndices[3], exchangeIndices[4], exchangeIndices[5]);

        random = (rand() % 1000) / 1000.0;

        if(scoreChange >= 0 || random < exp(scoreChange / Temperature))
        {
            int paperId1 = conference -> getTrack(exchangeIndices[0]).getSession(exchangeIndices[1]).getPaper(exchangeIndices[2]);
            int paperId2 = conference -> getTrack(exchangeIndices[3]).getSession(exchangeIndices[4]).getPaper(exchangeIndices[5]);

            conference -> setPaper(exchangeIndices[0], exchangeIndices[1], exchangeIndices[2], paperId2);
            conference -> setPaper(exchangeIndices[3], exchangeIndices[4], exchangeIndices[5], paperId1);

            conference -> setScore(conference -> getScore() + scoreChange);
            // cout << conference -> getScore() << endl;
        }

        if(conference-> getScore() > bestConference -> getScore())
            updateBestConference();

        if((((double)clock() - timer) / CLOCKS_PER_SEC) > (60 * processingTime - 0.1))
            return;
    }
}

void SessionOrganizer::readInInputFile(string filename)
{
    vector<string> lines;
    string line;
    ifstream myfile(filename.c_str());
    if(myfile.is_open())
    {
        while(getline(myfile, line))
            lines.push_back(line);
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
        distance[i] = new double[n];

    for(int i = 0; i < n; i++)
    {
        string tempLine = lines[i + 5];
        string elements[n];
        splitString(tempLine, " ", elements, n);

        for (int j = 0; j < n; j++)
            distance[i][j] = atof(elements[j].c_str());
    }
}

double** SessionOrganizer::getdistance()
{
    return distance;
}

void SessionOrganizer::printSessionOrganiser(char * filename)
{
    bestConference -> printConference(filename);
}

double SessionOrganizer::scoreOrganization()
{
    double score1 = 0.0;
    for(int i = 0; i < conference -> gett(); i++)
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

    double score2 = 0.0;
    for(int i = 0; i < conference -> gett(); i++ )
    {
        Track tmpTrack1 = conference -> getTrack(i);
        for (int j = 0; j < tmpTrack1.getNumberOfSessions(); j++)
        {
            Session tmpSession1 = tmpTrack1.getSession(j);
            for(int l = 0; l < tmpSession1.getNumberOfPapers(); l++)
            {
                int index1 = tmpSession1.getPaper(l);
                for(int m = j + 1; m < conference -> getp(); m++)
                {
                    Session tmpSession2 = tmpTrack1.getSession(m);
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

double SessionOrganizer::getBestScore()
{
    return bestConference -> getScore();
}

double SessionOrganizer::swapCostChange(int trackIndex1, int sessionIndex1, int paperIndex1, int trackIndex2, int sessionIndex2, int paperIndex2)
{
    Track tempTrack1 = conference -> getTrack(trackIndex1);
    Track tempTrack2 = conference -> getTrack(trackIndex2);
    Session tempSession1 = tempTrack1.getSession(sessionIndex1);
    Session tempSession2 = tempTrack2.getSession(sessionIndex2);
    int paperId1 = tempSession1.getPaper(paperIndex1);
    int paperId2 = tempSession2.getPaper(paperIndex2);

    double change = 0.0;

    for(int i = 0; i < k; i++)
        change += distance[paperId1][tempSession1.getPaper(i)] - distance[paperId1][tempSession2.getPaper(i)] + distance[paperId2][tempSession2.getPaper(i)] - distance[paperId2][tempSession1.getPaper(i)];
    change *= (c + 1);

    if(trackIndex1 == trackIndex2)
      change += (c + 1) * 2 * distance[paperId1][paperId2];
    else
    {
        change += 2 * distance[paperId1][paperId2];
        double extraChange = 0.0;
        for(int i = 0; i < p; i++)
            for(int j = 0; j < k; j++)
                extraChange += distance[paperId2][tempTrack1.getSession(i).getPaper(j)] + distance[paperId1][tempTrack2.getSession(i).getPaper(j)] - distance[paperId1][tempTrack1.getSession(i).getPaper(j)] - distance[paperId2][tempTrack2.getSession(i).getPaper(j)];
        extraChange *= c;
        change += extraChange;
    }

    return change;
}
