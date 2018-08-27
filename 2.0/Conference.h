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

    // The number of tracks.
    int t;

    // The number of sessions in a track.
    int p;

    // The number of papers in a session.
    int k;

    // cost of the current conference state
    double score;

public:
    Conference();

    /*
     * Constructor for Conference.
     *
     * @param t is the number of tracks.
     * @param p is the number of sessions in a track.
     * @param k is the number of papers in a session.
     */
    Conference(int t, int p, int k);

    /*
     * Initialize the tracks.
     * @param t is the number of tracks.
     * @param p is the number of sessions in a track.
     * @param k is the number of papers in a session.
     */
    void initTracks(int t, int p, int k);

    /*
     * Gets the number of tracks.
     * @return the number of tracks.
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
    * Gets the score of a conference
    */
    double getScore();

    /*
    * Sets the score of a conference
    */
    void setScore(double newScore);

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
