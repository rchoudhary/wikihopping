//
// Created by Rutvik Choudhary on 12/1/15.
//

#include "find_path.h"
#include <list>
#include <queue>
#include <thread>
#include <iostream>
#include <future>
#include <gumbo.h>
#include "CurlObj.h"
#include "get_internal_links.h"
#include "link_count.h"

using namespace std;

bool findPath(Link start, Link target, unordered_map<string, Node>* linkGraph)
{
    // get the maximum number of threads that can be used
    const int NUM_THREADS = (int)(thread::hardware_concurrency() * 1.5);

    // start by inserting the starting node into the graph
    linkGraph->insert(make_pair(start.href, Node {start, nullptr, 0}));

    // insert the start link into the queue
    queue<Link> queue;
    queue.push(start);

    // mutexes used for accessing the queue and graph
    mutex qMutex;
    mutex graphMutex;

    // begin the actual searching in the BFS algorithm
    while (!queue.empty())
    {
        // these futures hold bools representing whether a certain thread has found the target link
        vector<future<bool>> futures;

        // start N threads where N is the max number of threads allowed by the hardware
        for (int i = 0; i < NUM_THREADS; i++)
        {
            futures.push_back(async(launch::async, [&]() {
                unique_lock<mutex> qLock(qMutex);
                // If the queue is empty and the target has not previously been found,
                // then just return false
                if (queue.empty())
                {
                    return false;
                }
                Link currLink = queue.front();
                queue.pop();
                qLock.unlock();

                list<Link> adjLinks;

                // Use the `CurlObj` class and `getInternalLinks()` get a list of the adjacent articles.
                CurlObj curlObj;
                string url = "http://en.wikipedia.org" + currLink.href;
                std::string webpage;
                curlObj.getWebpage(url, webpage);
                GumboOutput* output = gumbo_parse(webpage.c_str());
                getInternalLinks(output, adjLinks);
                gumbo_destroy_output(&kGumboDefaultOptions, output);

                unique_lock<mutex> graphLock(graphMutex);

                // Loop through the adjacent articles, adding them to the graph and to the queue. If the
                // current adjacent article happens to be the target article, then return true
                for (Link adjLink : adjLinks)
                {
                    if (linkGraph->find(adjLink.href) == linkGraph->end())
                    {
                        LINK_COUNT() += 1;
                        Node& parentNode = linkGraph->at(currLink.href);
                        linkGraph->insert(
                            make_pair(adjLink.href, Node {adjLink, &parentNode, parentNode.distance + 1}));
                        if (adjLink.href == target.href)
                            return true;

                        qLock.lock();
                        queue.push(adjLink);
                        qLock.unlock();
                    }
                }
                graphLock.unlock();

                // At this point it can be assumed that this thread has failed to find the target
                return false;
            }));
        }

        // Wait until all N threads are done searching, then check to see if one of them found the target.
        // If so, return true, otherwise continue searching.
        for (int i = 0; i < NUM_THREADS; i++)
        {
            futures[i].wait();
            if (futures[i].get())
                return true;
        }
    }

    // If at this point the target article has not been found, we can assume that the path does not exist.
    return false;
}
