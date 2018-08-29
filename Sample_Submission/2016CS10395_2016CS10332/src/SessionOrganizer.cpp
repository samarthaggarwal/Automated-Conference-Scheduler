/*
 * File: SessionOrganizer.cpp
 * Author: Kapil Thakkar
 */

#include "SessionOrganizer.h"
#include "Util.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stack>
// #include <vector>
#include <utility>

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

void SessionOrganizer::copyConference(Conference *src, Conference *dest)
{
  for(int i = 0; i < t; i++)
      for(int j = 0; j < p; j++)
          for(int l = 0;l < k; l++)
              dest -> setPaper(i, j, l, src -> getTrack(i).getSession(j).getPaper(l));

    dest->setScore( src->getScore() );
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
            for(int l = 0; l < k; l++)
            {
                randPaper = rand() % numPapers;
                while(papersAssigned[randPaper] == true)
                    randPaper = (randPaper + 1) % numPapers;
                papersAssigned[randPaper] = true;
                conference -> setPaper(i, j, l, randPaper);
            }
    conference -> setScore(scoreOrganization());
    copyConference(conference, bestConference);
}

void SessionOrganizer::organizePapers(double timer)
{
    srand(time(NULL));
    randomInitialization();
    int numNeighbours = 0;
    double heat = 1.0;
    double temp_heat = 0.0;

    int n = k * t * p;
    if(n <= 1000)
        numNeighbours = 235;
    else if(n <= 2000)
        numNeighbours = 210;
    else if(n <= 3000)
        numNeighbours = 185;
    else if(n <= 4000)
        numNeighbours = 170;
    else if(n <= 5000)
        numNeighbours = 145;
    else if(n <= 6000)
        numNeighbours = 120;
    else if(n <= 7000)
        numNeighbours = 95;
    else if(n <= 8000)
        numNeighbours = 70;
    else if(n <= 9000)
        numNeighbours = 65;
    else if(n <= 10000)
        numNeighbours = 50;
    else
        numNeighbours = 40;

    if(n <= 1000)
        heat = 500.0;
    else if(n <= 5000)
        heat = 400.0;
    else
        heat = 200.0;

    int increment = 10;

    double score1 = 0.0;
    double score2 = 0.0;

    organizePapersGreedyHillClimbing(timer, numNeighbours + (increment+=10));

    while(true)
    {
        score1 = bestConference -> getScore();
        organizePapersSimulatedAnnealing(timer, heat + temp_heat);
        organizePapersGreedyHillClimbing(timer, numNeighbours + (increment+=10));
        numNeighbours = 250;
        if((((double)clock() - timer) / CLOCKS_PER_SEC) > (60 * processingTime - 1))
            return;

        score2 = bestConference -> getScore();
        if(score2 - score1 < 0.1)
            temp_heat += 5 * heat;
        else
            temp_heat = 0.0;

        // organizePapersGreedyHillClimbing(timer, numNeighbours + (increment+=10));
        // organizePapersIdDFS(timer);
        // conference = new Conference(t,p,k);
        // copyConference(bestConference, conference);
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
            copyConference(conference, bestConference);

        if(count == 1000)
            break;

        if((((double)clock() - timer) / CLOCKS_PER_SEC) > (60 * processingTime - 1))
            return;
    }
}

void SessionOrganizer::organizePapersIdDFS(double timer)
{
    stack< pair<Conference*,int> > frontier;
    int b = 3;

    Conference* tempConference;
    Conference* originalConference = new Conference(t,p,k);
    copyConference(conference, originalConference);
    int trackIndex1, trackIndex2, sessionIndex1, sessionIndex2, paperIndex1, paperIndex2, paperId1, paperId2;
    double scoreChange;
    pair<Conference*,int> node, tempnode;

    int depth = 2;
    while(depth < 11){
        conference = new Conference(t,p,k);
        copyConference(originalConference, conference);
        // conference -> printConferenceToConsole();
        node.first = conference;
        node.second = 0;

        frontier.push(node);
        while(!frontier.empty()){
            node = frontier.top();
            frontier.pop();
            conference = node.first;

            if(node.second < depth){

                for(int i=0;i<b;i++){ // generating and pushing b random neighbours of conference
                    trackIndex1 = rand()%t;
                    sessionIndex1 = rand()%p;
                    paperIndex1 = rand()%k;

                    do{
                        trackIndex2 = rand()%t;
                        sessionIndex2 = rand()%p;
                        paperIndex2 = rand()%k;
                    }while(trackIndex1==trackIndex2 && sessionIndex1==sessionIndex2);

                    scoreChange = swapCostChange(trackIndex1, sessionIndex1, paperIndex1, trackIndex2, sessionIndex2, paperIndex2);

                    tempConference = new Conference(t,p,k);
                    copyConference(conference, tempConference);
                    paperId1 = conference->getTrack(trackIndex1).getSession(sessionIndex1).getPaper(paperIndex1);
                    paperId2 = conference->getTrack(trackIndex2).getSession(sessionIndex2).getPaper(paperIndex2);
                    tempConference -> setPaper(trackIndex1, sessionIndex1, paperIndex1, paperId2);
                    tempConference -> setPaper(trackIndex2, sessionIndex2, paperIndex2, paperId1);

                    tempConference->setScore( conference->getScore() + scoreChange );
                    tempnode.first = tempConference;
                    tempnode.second = node.second + 1;
                    frontier.push(tempnode);

                    if(((double)clock() - timer) / CLOCKS_PER_SEC > 60 * processingTime - 0.01)
                        return;
                }
            }

            if(conference->getScore() <= bestConference->getScore())
                delete(conference);
            else
            {
                copyConference(conference, bestConference);
                return;
            }
        }
        depth++;
    }

}

void SessionOrganizer::organizePapersSimulatedAnnealing(double timer, double heat)
{
    int exchangeIndices[6];

    double scoreChange;
    double Temperature = INFINITY;

    double random;

    for(int itr = 1; Temperature > 0.01; itr++)
    {
        if(Temperature > 2)
            Temperature = 50.0 - itr / 10.0;
        else
            Temperature = 1000.0 * heat / itr;

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
            //cout << conference -> getScore() << endl;
        }

        if(conference-> getScore() > bestConference -> getScore())
            copyConference(conference, bestConference);

        if((((double)clock() - timer) / CLOCKS_PER_SEC) > (60 * processingTime - 1))
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
