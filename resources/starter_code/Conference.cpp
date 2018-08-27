/*
 * File:   Conference.cpp
 * Author: Kapil Thakkar
 */

#include "Conference.h"

Conference::Conference()
{
    this -> p = 0;
    this -> t = 0;
    this -> k = 0;
}

Conference::Conference(int p, int t, int k)
{
    this -> p = p;
    this -> t = t;
    this -> k = k;
    initTracks(p, t, k);
}

void Conference::initTracks(int p, int t, int k)
{
    tracks = (Track *)malloc(sizeof(Track) * p);
    for(int i = 0; i < p; i++)
    {
        Track tempTrack(t);
        for (int j = 0; j < t; j++)
        {
            Session tempSession(k);
            tempTrack.setSession(j, tempSession);
        }
        tracks[i] = tempTrack;
    }
}

int Conference::getp()
{
    return p;
}

int Conference::gett()
{
    return t;
}

int Conference::getk()
{
    return k;
}

Track Conference::getTrack(int index)
{
    return tracks[index];
}

void Conference::setPaper(int trackIndex, int sessionIndex, int paperIndex, int paperId)
{
    Track curTrack = tracks[trackIndex];
    curTrack.setPaper(sessionIndex, paperIndex, paperId);
}

void Conference::printConference(char* filename)
{
    ofstream ofile(filename);

    for(int i = 0; i < t; i++)
    {
        for(int j = 0; j < p; j++)
        {
            for(int l = 0; l < k; l++)
            {
                ofile << tracks[j].getSession(i).getPaper(l) << " ";
            }
            if(j != p - 1)
            {
                ofile << "| ";
            }
        }
        ofile << "\n";
    }
    ofile.close();
    cout << "Organization written to ";
    printf("%s :)\n", filename);
}
