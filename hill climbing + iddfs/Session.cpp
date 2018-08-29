/*
 * File:   Session.cpp
 * Author: Kapil Thakkar
 */

#include <errno.h>
#include <vector>
#include "Session.h"

Session::Session()
{
    k = 0;
}

Session::Session(int k)
{
    this -> k = k;
    initPapers(k);
}

// Session::~Session()
// {
//     free(papers);
// }

void Session::initPapers(int k)
{
    this -> papers = (int *)malloc(sizeof(int) * k);
    // this->papers = new int[k];
    for(int i = 0; i < k; i++)
    {
        papers[i] = -1;
    }
}

int Session::getNumberOfPapers()
{
    return k;
}

int Session::getPaper(int index)
{
    return papers[index];
}

void Session::setPaper(int index, int paperId)
{
    papers[index] = paperId;
}

void Session::printSession()
{
    for (int i = 0; i < k; i++)
    {
        cout << papers[i] << " ";
    }
    cout << endl;
}
