#ifndef DATASTRUCT_HPP
#define DATASTRUCT_HPP

#include "Errors.hpp"
#include "Garfield/Track.hh"

// data structure containing ionized electrons clusters at points on the track.
typedef struct
{
    // eventId and trackId
    unsigned int evtId, trkId;
    // the number of clusters
    unsigned int nc;
    // the array of cluster coordinates
    double *xc, *yc, *zc, *tc, *ec;
    // the array of the number of electrons of clusters
    int *ne;
} TrackClustersPtr;

// data structure containing information on both ends of a drift line stated at a cluster.
typedef struct
{
    // clusterId
    unsigned int cltId;
    // the number of total avalanche size in a cluster point.
    unsigned int ne;
    // position, time and energy at a start point
    double *xe1, *ye1, *ze1, *te1, *e1;
    // position, time and energy at an end point
    double *xe2, *ye2, *ze2, *te2, *e2;
    int *status;
} DriftEndPointsPtr;

// allocate memory to pointers in structures.
static void allocateTrackClustersPtr(TrackClustersPtr *trackClusters, unsigned int buffSize)
{
    try
    {
        trackClusters->xc = new double[buffSize];
        trackClusters->yc = new double[buffSize];
        trackClusters->zc = new double[buffSize];
        trackClusters->tc = new double[buffSize];
        trackClusters->ec = new double[buffSize];
        trackClusters->ne = new int[buffSize];
    }
    catch (std::exception &e)
    {
        printError("global", "allocateTrackClustersPtr(TrackClustersPtr *, unsigned int)", e.what());
    }
}

static void fillTrackClustersPtr(Garfield::Track *track, TrackClustersPtr *clusters)
{
    unsigned int ptr = 0;
    double extra = 0;
    try
    {
        while (track->GetCluster(clusters->xc[ptr], clusters->yc[ptr], clusters->zc[ptr], clusters->tc[ptr], clusters->ne[ptr], clusters->ec[ptr], extra))
        {
            ++ptr;
        }
    }
    catch (std::exception &e)
    {
        printError("global", "fillTrackClustersPtr(Garfield::Track *, TrackClustersPtr *)", e.what());
    }
    clusters->nc = ptr;
}

static void allocateDriftEndPointsPtr(DriftEndPointsPtr *endPoints, unsigned int buffSize)
{
    try
    {
        endPoints->xe1 = new double[buffSize];
        endPoints->ye1 = new double[buffSize];
        endPoints->ze1 = new double[buffSize];
        endPoints->te1 = new double[buffSize];
        endPoints->e1 = new double[buffSize];
        endPoints->xe2 = new double[buffSize];
        endPoints->ye2 = new double[buffSize];
        endPoints->ze2 = new double[buffSize];
        endPoints->te2 = new double[buffSize];
        endPoints->e2 = new double[buffSize];
        endPoints->status = new int[buffSize];
    }
    catch (std::exception &e)
    {
        printError("global", "allocateDriftEndPointsPtr(DriftEndPointsPtr *, unsigned int)", e.what());
    }
}

// free memory allocated to pointers in structures.
static void freeTrackClustersPtr(TrackClustersPtr *trackClusters)
{
    delete[] trackClusters->xc;
    delete[] trackClusters->yc;
    delete[] trackClusters->zc;
    delete[] trackClusters->tc;
    delete[] trackClusters->ec;
}

static void freeDriftEndPointsPtr(DriftEndPointsPtr *endPoints)
{
    delete[] endPoints->xe1;
    delete[] endPoints->ye1;
    delete[] endPoints->ze1;
    delete[] endPoints->te1;
    delete[] endPoints->e1;
    delete[] endPoints->xe2;
    delete[] endPoints->ye2;
    delete[] endPoints->ze2;
    delete[] endPoints->te2;
    delete[] endPoints->e2;
    delete[] endPoints->status;
}

#endif // DATASTRUCT_HP