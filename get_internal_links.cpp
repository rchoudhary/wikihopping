#include <ostream>
#include <gumbo.h>
#include <iostream>
#include <list>
#include "get_internal_links.h"

using namespace std;

// PRIVATE FREE FUNCTIONS:
// Functions beginning with '__' are private helper functions that were made so that
// `getInternalLinks()` doesn't get cluttered

// Helper method for adding an internal link to the list of links given a GumboNode representing
// the `a` element that has an internal `href`.
//
// This method mostly serves to de-clutter the main `getInternalLinks` function by removing the
// boilerplate required to extract info from a node.
void __addLinkToList(GumboNode* node, list<Link>& links)
{
    GumboAttribute* hrefAttr = gumbo_get_attribute(&node->v.element.attributes, "href");
    GumboAttribute* titleAttr = gumbo_get_attribute(&node->v.element.attributes, "title");

    if (hrefAttr && titleAttr)
    {
        string href = hrefAttr->value;
        if (href.find("/wiki/") == 0 && href.find(":", 6) == string::npos)
        {
            if (node->v.element.children.length > 0)
            {
                GumboNode* child = (GumboNode*) (node->v.element.children.data[0]);
                if (child->type == GUMBO_NODE_TEXT || child->type == GUMBO_NODE_WHITESPACE)
                {
                    string text = child->v.text.text;
                    string title = titleAttr->value;
                    links.push_back({text, title, href});
                }
            }
        }
    }
}

// Searches the children of a node one by one.
void __linearSearch(GumboNode* node, list<Link>& links)
{
    GumboVector children = node->v.element.children;
    for (int i = 0; i < children.length; i++)
    {
        GumboNode* currChild = (GumboNode*) children.data[i];
        if (currChild->type == GUMBO_NODE_ELEMENT && currChild->v.element.tag == GUMBO_TAG_A)
            __addLinkToList(currChild, links);
    }
}

// Searches the children of each child node of a parent node
void __secondLevelLinearSearch(GumboNode* node, list<Link>& links)
{
    GumboVector children = node->v.element.children;
    for (int i = 0; i < children.length; i++)
    {
        GumboNode* firstLevelChild = (GumboNode*) children.data[i];
        if (firstLevelChild->type == GUMBO_NODE_ELEMENT)
        {
            __linearSearch(firstLevelChild, links);
        }
    }
}


void getInternalLinks(GumboOutput* output, list<Link>& links)
{
    // This basically directly goes to a certain `div` element that is the lowest level element that contains all
    // internal links. This basically limits the scope of the scrape and saves time by avoiding scraping sections
    // of the HTML that don't contain internal links, e.g. the `head` section or `footer` section.
    GumboNode* body = (GumboNode*) output->root->v.element.children.data[2];
    GumboNode* content = (GumboNode*) body->v.element.children.data[5];
    GumboNode* bodyContent = (GumboNode*) content->v.element.children.data[9];
    GumboNode* rootNode = (GumboNode*) bodyContent->v.element.children.data[7];

    // Loop through the children of this root `div` and based on its type, search this child differently.

    GumboVector children = rootNode->v.element.children;

    for (int i = 0; i < children.length; i++)
    {
        GumboNode* currChild = (GumboNode*) children.data[i];
        if (currChild->type != GUMBO_NODE_ELEMENT)
            continue;

        switch (currChild->v.element.tag)
        {
            // If the child is a `p`, then simply search through it linearly for internal links
            case GUMBO_TAG_P:
            {
                __linearSearch(currChild, links);
                break;
            }
            // If the child is a `div`, check to see if it has a CSS class 'relarticle' (indicating it is the
            // area under section headers that contains related articles). If so, linearly search it for links.
            case GUMBO_TAG_DIV:
            {
                GumboAttribute* classAttr = gumbo_get_attribute(&currChild->v.element.attributes, "class");
                if (classAttr)
                {
                    string classListStr = classAttr->value;
                    if (classListStr.find("relarticle") != string::npos)
                    {
                        __linearSearch(currChild, links);
                        break;
                    }
                }
            }
            // If the tag is an `ol` or `ul`, both of which usually contain a decent number of links in Wikipedia
            // pages, then perform a second level linear search. This is just my way of saying that instead of
            // linearly searching the tag, linearly search the tag's children.
            case GUMBO_TAG_OL:
            case GUMBO_TAG_UL:
            {
                __secondLevelLinearSearch(currChild, links);
                break;
            }
            default:
                break;
        }
    }
}
