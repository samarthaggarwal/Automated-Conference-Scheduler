/*
 * File:   Track.cpp
 * Author: Kapil Thakkar
 */

#include <stdlib.h>
#include "Track.h"

Track::Track()
{
    this -> t = 0;
}

Track::Track ( int t )
{
    this -> t = t;
    sessions = (Session *)malloc(sizeof(Session) * t);
}

void Track::setPaper(int sessionIndex, int paperIndex, int paperId)
{
    Session curSession = sessions[sessionIndex];
    curSession.setPaper(paperIndex, paperId);
}

int Track::getNumberOfSessions()
{
    return this -> t;
}

Session Track::getSession(int index)
{
    return sessions[index];
}

void Track::setSession(int index, Session session)
{
    sessions[index] = session;
}
