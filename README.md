# WikiHopping

There is a game on the internet known as "WikiHopping" where the goal is to get from one article to another one in as few steps as possible. I wrote this program to automate this process and determine the shortest path between any two given articles.

# How to Build and Install

Simply `cd` into the project's directory and run `./build.sh` and `./install.sh` in that order. This will install the executable in `/usr/local/bin` and the necessary libraries in `/usr/local/lib`.

# How to Use

Using this program is very simple. Just use the following template:

    $ wikihopping [start url] [target url]

Where `start url` and `target url` both are in the form `/wiki/<page>`

So let's say you want to get from "C++" to "New Jersey". Simply run:

    $ wikihopping '/wiki/C%2B%2B' '/wiki/New_Jersey'

This will generate the following output

    ************ SUCCEEDED *************
    
    program took 3.17664 seconds
    explored 1915 links
    
    START > Bell Labs > New Jersey

(Of course, the output will look different if it fails and the time taken depends on your internet connection's speed)

###A Note About URLs

Note that there are some things to consider when entering URLs:

1. Notice the leading slash in `/wiki/<page>`. This leading slash is *mandatory*!
2. The URLs must match exactly with what appears in your address bar. Considering the example run given earlier:
  1. `/wiki/C%2B%2B` is not equal to `/wiki/C++`, `/wiki/c++`, or even `/wiki/c%2b%2b` for that matter.
  2. Similiarly, `/wiki/New_Jersey` is not equal to `/wiki/NewJersey`, `/wiki/new_Jersey`, or anything else.
  
If these rules aren't adhered to 100%, then the program *will* crash!

<b>Tip:</b> To avoid all this hassle, just copy and paste the relevant portion of the url from your browser's address bar!

# References

* The cURL library [libcurl](https://curl.haxx.se/libcurl/) is used to retrieve the webpages.
* Google's [Gumbo HTML5 parser](https://github.com/google/gumbo-parser) is used to scrape the source code. 
* The multithreaded version of Breadth-First Search implemented in this program is based on the one described in the paper *Designing Multithreaded Algorithms for Breadth-First Search and st-connectivity on the Cray MTA-2* written by David A. Bader and Kamesh Madduri found [here](http://www.cc.gatech.edu/~bader/papers/MultithreadedBFS.html).
