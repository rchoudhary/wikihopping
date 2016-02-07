#include <iostream>
#include <unordered_map>
#include <curl/curl.h>
#include "Node.h"
#include "find_path.h"
#include "link_count.h"

using namespace std;

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        cout << "usage: wikihopping [start url] [target url]" << endl;
        cout << endl;
        cout << "Notes on url format:" << endl;
        cout << "1. Urls should look like `/wiki/[page]` (notice the leading slash!)" << endl;
        cout << "2. Urls must be properly escaped (e.g. `/wiki/C++` should be `/wiki/C%2B%2B`)" << endl;
        cout << "3. Urls are also case sensitive (e.g. `/wiki/new_jersey` is NOT the same as `/wiki/New_Jersey`)" << endl;
        cout << "Failure to adhere to these guidelines will cause the program to hang or crash" << endl;
        cout << "PRO TIP: just copy and paste the relevant portion of the url from your browser's address bar" << endl;
        abort();
    }

    string startURL = argv[1];
    string targetURL = argv[2];

    curl_global_init(CURL_GLOBAL_ALL);

    // Create the graph that will contain the pages and reserve an ample amount of space

    unordered_map<string, Node>* linkGraph = new unordered_map<string, Node>();
    linkGraph->reserve(5000000);

    Link start = Link {"", "START", startURL};
    Link target = Link {"", "", targetURL};

    auto startTime = chrono::high_resolution_clock::now();

    // Start the search!

    bool targetFound = findPath(start, target, linkGraph);

    auto endTime = chrono::high_resolution_clock::now();
    long duration = chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();

    // Output some stats

    cout << endl;
    if (targetFound)
        cout << "************ SUCCEEDED *************" << endl;
    else
        cout << "************* FAILED ***************" << endl;
    cout << endl;
    cout << "program took " << duration / 1000000.f << " seconds" << endl;
    cout << "explored " << LINK_COUNT() << " links" << endl;
    cout << endl;

    // Now that the graph is made and a path exists from the start to the target, the graph
    // can easily be traversed from the target to the start to output the path from
    // the start to the target.

    if (targetFound)
    {
        Node targetNode = linkGraph->at(target.href);
        Node* currNode = &targetNode;
        string str;
        while (currNode->parent != nullptr)
        {
            str.insert(0, " > " + currNode->link.title);
            currNode = currNode->parent;
        }
        str.insert(0, currNode->link.title);
        cout << str << endl;
    }

    curl_global_cleanup();

    return 0;
}
