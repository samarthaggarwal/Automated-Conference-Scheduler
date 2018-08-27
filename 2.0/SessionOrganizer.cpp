/*
 * File:   SessionOrganizer.cpp
 * Author: Kapil Thakkar
 *
 */

#include "SessionOrganizer.h"
#include "Util.h"

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
}

void SessionOrganizer::organizePapers()
{
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
    cout << swapCostChange(1, 0, 1, 2, 1, 1) << endl;
    conference -> setPaper(2, 1, 1, 5);
    conference -> setPaper(1, 0, 1, 11);
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
