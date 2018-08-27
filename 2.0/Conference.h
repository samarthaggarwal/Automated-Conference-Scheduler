/*
 * File:   Conference.h
 * Author: Kapil Thakkar
 *
 * Created on 9 August, 2015, 10:07 AM
 */

#ifndef CONFERENCE_H
#define	CONFERENCE_H

#include<iostream>
#include<fstream>
using namespace std;

#include "Track.h"

class Conference {
private:

    // The array of tracks.
    Track *tracks;

    // The number of parallel tracks.
    int p;

    // The number of sessions in a track.
    int t;

    // The number of papers in a session.
    int k;

public:
    Conference();

    /*
     * Constructor for Conference.
     *
     * @param p is the number of parallel tracks.
     * @param t is the number of sessions in a track.
     * @param k is the number of papers in a session.
     */
    Conference(int p, int t, int k);

    /*
     * Initialize the tracks.
     * @param p is the number of parallel tracks.
     * @param t is the number of sessions in a track.
     * @param k is the number of papers in a session.
     */
    void initTracks(int p, int t, int k);

    /*
     * Gets the number of parallel tracks.
     * @return the number of parallel tracks.
     */
    int getp();

    /*
     * Gets the number of sessions in a track.
     * @return the number of sessions in a track.
     */
    int gett();

    /*
     * Gets the number of papers in a session.
     * @return the number of papers in a session.
     */
    int getk();

    /*
     * Gets the track at the specified index.
     * @param index is the index of the specified track.
     * @return the track
     */
    Track getTrack(int index);

    /*
     * Sets the paper in the specified slot to the given paper id.
     * @param trackIndex is the track index.
     * @param sessionIndex is the session index.
     * @param paperIndex is the paper index.
     * @param paperId is the id of the paper.
     */
    void setPaper(int trackIndex, int sessionIndex, int paperIndex, int paperId);

    void printConference(char *);
};

#endif	/* CONFERENCE_H */