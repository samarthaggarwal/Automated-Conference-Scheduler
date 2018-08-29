#include "SessionOrganizer.h"
#include "Util.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>
// #include <queue>
#include <stack>
#include <vector>
#include <pair>

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

    for(int i=0;i<t;i++){
        for(int j=0;j<p;j++){
            for(int l=0;l<k;l++){
                randPaper = rand()%numPapers;
                while(papersAssigned[randPaper]==true)
                    randPaper = (randPaper + 1)%numPapers;

                papersAssigned[randPaper] = true;
                conference->setPaper(i,j,l,randPaper);
            }
        }
    }

    conference->setScore( scoreOrganization() );

    copyConference(conference, bestConference);
    // printing the initialized state and score to console
    // cout<<"randomly initialized new state\n";
    // cout<<"score = "<<conference->getScore()<<endl;
}

void SessionOrganizer::organizePapers(double timer)
{
  // Orderly initialization
/*
    int paperCounter = 0;
    for(int i = 0; i < conference -> gett(); i++)
    {
        for(int j = 0; j < conference -> getp(); j++)
        {
            for (int k = 0; k < conference -> getk(); k++)
            {
                conference -> setPaper(i, j, k, paperCounter);
                paperCounter++;
            }
        }
    }

    conference->setScore( scoreOrganization());

    cout<<"=========================ordered initialization of new state\n";
    conference->printConferenceToConsole();
    cout<<"score = "<<conference->getScore()<<endl;
*/

    // to initialize random seed
    srand(time(NULL));
    // srand(2523);
    randomInitialization();

    // number of neighbours from which to select the next node
    int numNeighbours = 250;

    int exchangeIndices[numNeighbours][6];
    // order = trackIndex1, sessionIndex1, paperIndex1, trackIndex2, sessionIndex2, paperIndex2;

    double *scoreChange = (double*) malloc((numNeighbours) * sizeof(double));

    double maxScoreChange;
    int maxScoreIndex;
    double random;
    int count=0;
    int localOptimaMovesCount=1000;

    while(true)
    {
        for(int i = 0; i < numNeighbours; i++)
        {
            exchangeIndices[i][0] = rand() % (conference -> gett());
            exchangeIndices[i][1] = rand() % (conference -> getp());
            exchangeIndices[i][2] = rand() % (conference -> getk());

            do
            {
                exchangeIndices[i][3] = rand() % (conference -> gett()) ;
                exchangeIndices[i][4] = rand() % (conference -> getp()) ;
                exchangeIndices[i][5] = rand() % (conference -> getk()) ;
            } while(exchangeIndices[i][0] == exchangeIndices[i][3] && exchangeIndices[i][1] == exchangeIndices[i][4]);

            scoreChange[i] = swapCostChange(exchangeIndices[i][0], exchangeIndices[i][1], exchangeIndices[i][2], exchangeIndices[i][3], exchangeIndices[i][4], exchangeIndices[i][5]);
            // totalScoreChange += scoreChange[i];
        }

        // probabilistic - movement to neighbour
        // random = rand();
        // random -= floor(random / totalScoreChange) * totalScoreChange;
        // totalScoreChange = ceil(totalScoreChange);
        // while(random > 0)
        //     random -= scoreChange[maxScoreIndex++];
        // maxScoreIndex = max(maxScoreIndex--, 0);
        // selecting the best neighbour - deterministic

        // maxScoreChange1 = -INFINITY;
        // maxScoreChange2 = -INFINITY;
        // for(int i = 0; i < numNeighbours; i++)
        // {
        //     if(maxScoreChange1 < scoreChange[i])
        //     {
        //         maxScoreChange2 = maxScoreChange1;
        //         maxScoreChange1 = scoreChange[i];
        //         maxScoreIndex2 = maxScoreIndex1;
        //         maxScoreIndex1 = i;
        //     }
        //     else if(maxScoreChange2 < scoreChange[i])
        //     {
        //         maxScoreChange2 = scoreChange[i];
        //         maxScoreIndex2 = i;
        //     }
        // }

        maxScoreChange = -INFINITY;
        for(int i = 0; i < numNeighbours; i++)
            if(maxScoreChange < scoreChange[i])
            {
                maxScoreChange = scoreChange[i];
                maxScoreIndex = i;
            }

        if(maxScoreChange >= 0)
        {
            count=0;
            int paperId1 = conference->getTrack(exchangeIndices[maxScoreIndex][0]).getSession(exchangeIndices[maxScoreIndex][1]).getPaper(exchangeIndices[maxScoreIndex][2]);
            int paperId2 = conference->getTrack(exchangeIndices[maxScoreIndex][3]).getSession(exchangeIndices[maxScoreIndex][4]).getPaper(exchangeIndices[maxScoreIndex][5]);

            conference -> setPaper(exchangeIndices[maxScoreIndex][0], exchangeIndices[maxScoreIndex][1], exchangeIndices[maxScoreIndex][2], paperId2);
            conference -> setPaper(exchangeIndices[maxScoreIndex][3], exchangeIndices[maxScoreIndex][4], exchangeIndices[maxScoreIndex][5], paperId1);

            conference->setScore( conference->getScore() + maxScoreChange );
            // printing new score to console when state changes
            cout << conference->getScore() << endl;
        }

        count++;

        if(conference->getScore() > bestConference->getScore())
            copyConference(conference, bestConference);

        if(count>=localOptimaMovesCount){
            // stuck at local optima
            cout<<"stuck at local optima at time = "<<((double)clock() - timer) / CLOCKS_PER_SEC<<"\n";
            cout<<"before iddfs, best score obtained = "<<bestConference->getScore()<<endl;
            iddfs(timer);
            cout<<"from iddfs, best score obtained = "<<bestConference->getScore()<<endl;
            // break;
        }

        if(((double)clock() - timer) / CLOCKS_PER_SEC > 60 * processingTime - 0.01)
            return;
    }
}

// bool Conference::operator<(Conference* c1, Conference* c2)
// {
//     return c1->getScore() < c2->getScore();
// }

void SessionOrganizer::iddfs(double timer)
{
    // priority_queue<Conference*, vector<Conference*> > frontier;
    stack< pair<Conference*,int> > frontier;
    // branching factor
    int b = 5;
    // frontier.push(conference);
    Conference* tempConference;
    int trackIndex1, trackIndex2, sessionIndex1, sessionIndex2, paperIndex1, paperIndex2, scoreChange, paperId1, paperId2;
    pair<Conference*,int> rootnode, node, tempnode;
    rootnode.first = conference;
    rootnode.second = 0;

    // limit for depth
    int depth = 2;
    while(true){
        frontier.push(rootnode);
        while(!frontier.empty()){
            // cout<<conference->getScore()<<endl;
            // getchar();

            node = frontier.top();
            frontier.pop();
            conference = node.first;

            // if(conference->getScore() > bestConference->getScore()){
            //     copyConference(conference, bestConference);
            //     cout<<bestConference->getScore()<<endl;
            // }

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
            else{
                copyConference(conference, bestConference);
                // cout<<2<<endl;
                // cout<<"ended iddfs on score = ";
                // cout<<bestConference->getScore()<<endl;
                // bestConference->printConferenceToConsole();
                return;
            }
        }

        depth++;
        cout<<"depth = "<<depth<<endl;
    }

    // DELETE ALL FRONTIER ELEMENTS
    // cout<<"hi\n";

    // if(conference->getScore() > bestConference->getScore()){
    //     // cout<<1<<endl;
    //     copyConference(conference, bestConference);
    //     // cout<<2<<endl;
    //     cout<<"ended bfs on score = ";
    //     cout<<bestConference->getScore()<<endl;
    //     bestConference->printConferenceToConsole();
    // }

    // cout<<"hi2\n";
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
    bestConference -> printConference(filename);
}

double SessionOrganizer::scoreOrganization()
{
    // Sum of pairwise similarities per session.
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

    // Sum of distances for competing papers.
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

                // Get competing papers.
                for(int m = j + 1; m < conference -> getp(); m++)
                {
                    // Track tmpTrack2 = conference -> getTrack(i);
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
    return bestConference->getScore();
}

double SessionOrganizer::swapCostChange(int trackIndex1, int sessionIndex1, int paperIndex1, int trackIndex2, int sessionIndex2, int paperIndex2)
{
    Track tempTrack1 = conference -> getTrack(trackIndex1);
    Track tempTrack2 = conference -> getTrack(trackIndex2);
    Session tempSession1 = tempTrack1.getSession(sessionIndex1);
    Session tempSession2 = tempTrack2.getSession(sessionIndex2);
    int paperId1 = tempSession1.getPaper(paperIndex1);
    int paperId2 = tempSession2.getPaper(paperIndex2);
    double change = 0;

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
        {
            tempSession1 = tempTrack1.getSession(i);
            tempSession2 = tempTrack2.getSession(i);
            for(int j = 0; j < k; j++)
                extraChange += distance[paperId2][tempSession1.getPaper(j)] + distance[paperId1][tempSession2.getPaper(j)] - distance[paperId1][tempSession1.getPaper(j)] - distance[paperId2][tempSession2.getPaper(j)];
        }
        extraChange *= c;
        change += extraChange;
    }

    return change;
}
