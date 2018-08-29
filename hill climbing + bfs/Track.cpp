/*
 * File:   Track.cpp
 * Author: Kapil Thakkar
 */

#include <stdlib.h>
#include "Track.h"

Track::Track()
{
    this -> p = 0;
}

Track::Track(int p)
{
    this -> p = p;
    sessions = (Session *)malloc(sizeof(Session) * p);
}

// Track::~Track()
// {
//     free(sessions);
// }

void Track::setPaper(int sessionIndex, int paperIndex, int paperId)
{
    Session curSession = sessions[sessionIndex];
    curSession.setPaper(paperIndex, paperId);
}

int Track::getNumberOfSessions()
{
    return this -> p;
}

Session Track::getSession(int index)
{
    return sessions[index];
}

void Track::setSession(int index, Session session)
{
    sessions[index] = session;
}
