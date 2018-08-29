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
    this -> score = 0;
}

Conference::Conference(int t, int p, int k)
{
    this -> t = t;
    this -> p = p;
    this -> k = k;
    this -> score = 0;
    initTracks(t, p, k);
}

void Conference::initTracks(int t, int p, int k)
{
    tracks = (Track *) malloc(sizeof(Track) * t);
    for(int i = 0; i < t; i++)
    {
        Track tempTrack(p);
        for(int j = 0; j < p; j++)
        {
            Session tempSession(k);
            tempTrack.setSession(j, tempSession);
        }
        tracks[i] = tempTrack;
    }
}

int Conference::gett()
{
    return t;
}

int Conference::getp()
{
    return p;
}

int Conference::getk()
{
    return k;
}

double Conference::getScore()
{
    return score;
}

void Conference::setScore(double newScore)
{
    this -> score = newScore;
}

Track Conference::getTrack(int index)
{
    return tracks[index];
}

void Conference::setTrack(Track track, int i)
{
    tracks[i] =track;
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
                ofile << tracks[i].getSession(j).getPaper(l) << " ";
            if(j != p - 1)
                ofile << "| ";
        }
        ofile << "\n";
    }
    ofile.close();
}
