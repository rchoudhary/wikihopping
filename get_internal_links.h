//
// Created by Rutvik Choudhary on 12/1/15.
//

#pragma once

#include "gumbo.h"
#include <string>
#include <list>
#include "Link.h"


// This function, given a GumboOutput pointer (created from HTML content) populates a user-provided reference to a
// list with all[1] internal links (i.e. links within the article itself that lead to other Wikipedia articles).
//
// This function utilizes the Gumbo HTML parser by Google.
//
// [1] To make the web scraping faster, it excludes searching through tables and photo captions

void getInternalLinks(GumboOutput* gumboOutput, std::list<Link>& links);