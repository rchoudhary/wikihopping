//
// Created by Rutvik Choudhary on 12/1/15.
//

#pragma once

#include <string>

// Small struct used to represent an HTML link.
// `text` refers to the text contained within the `a` tags.
// `title` contains the value of the `title` attribute, which in turn usually corresponds with thw
// title of the page.
// `href` is the relative url of the page in the form `/wiki/[page]`.
struct Link
{
    std::string text, title, href;
};