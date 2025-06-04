

#include "Tag.hpp"
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <memory>
#include <string_view>
#include <vector>

namespace {

[[nodiscard]] bool iequals(std::string_view a, std::string_view b);

void advanceToNextChar(std::string_view str, std::size_t &pos);

[[nodiscard]] bool properEndTag(std::string_view document, std::size_t &pos);

[[nodiscard]] bool validPTag(std::string_view document, std::size_t &pos);

[[nodiscard]] bool validSpanTag(std::string_view document, std::size_t &pos);

[[nodiscard]] bool validDivTag(std::string_view document, std::size_t &pos);

[[nodiscard]] bool validBRTag(std::string_view document, std::size_t &pos);


[[nodiscard]] bool validID(std::string_view document, std::size_t &pos);

} // namespace

[[nodiscard]] bool html_is_valid(const std::string &document) {

    if (document.empty()) {
        return false;
    }
    std::size_t pos = 0;
    std::string_view doc(document);

    advanceToNextChar(doc, pos);

    if (document.size() < pos + 15) {
        return false;
    }

    std::string_view docType = doc.substr(pos, 9); //<!doctype

    if (!iequals(docType, "<!doctype")) {
        return false;
    }
    pos += 9;
    advanceToNextChar(doc, pos);

    std::string_view h = doc.substr(pos, 4); //html

    if (!iequals(h, "html")) {
        return false;
    }
    pos += 4;

    if (!properEndTag(doc, pos)) {
        return false;
    }

    pos += 1;
    advanceToNextChar(doc, pos);

    std::string_view html = doc.substr(pos, 5); //<html

    if (!iequals(html, "<html")) {
        return false;
    }
    pos += 5;
    advanceToNextChar(doc, pos);
    if (doc.substr(pos, 2).compare("id") == 0) {
        if (!validID(doc, pos)) {
            return false;
        }
    }

    if (!properEndTag(doc, pos)) {
        return false;
    }
    pos += 1;
    advanceToNextChar(doc, pos);

    std::string_view head = doc.substr(pos, 5); //<head

    if (!iequals(head, "<head")) {
        return false;
    }
    pos += 5;
    advanceToNextChar(doc, pos);
    if (doc.substr(pos, 2).compare("id") == 0) {
        if (!validID(doc, pos)) {
            return false;
        }
    }
    if (!properEndTag(doc, pos)) {
        return false;
    }
    pos += 1;  // it is now right after '<head>'
    advanceToNextChar(doc, pos);

    std::string_view title = doc.substr(pos, 6); //<title

    if (!iequals(title, "<title")) {
        return false;
    }
    pos += 6;
    advanceToNextChar(doc, pos);
    if (doc.substr(pos, 2).compare("id") == 0) {
        if (!validID(doc, pos)) {
            return false;
        }
    }

    if (!properEndTag(doc, pos)) {
        return false;
    }
    pos += 1; //it now right after <title>
    advanceToNextChar(doc, pos);

    std::string_view cTitle = doc.substr(pos, 7);

    if (iequals(cTitle, "</title")) { //if title is empty
        return false;
    }


    while (!iequals(cTitle, "</title")) { //while searching for closing title tag

        if (iequals(doc.substr(pos, 6), "</html")) {
            return false;
        }
        pos += 1;
        cTitle = doc.substr(pos, 7);
        if (pos > doc.size()) {
            return false;
        }
    }
    pos += 7;


    if (!properEndTag(doc, pos)) {
        return false;
    }
    pos += 1;  //it now after </title>
    advanceToNextChar(doc, pos);

    std::string_view closeHead = doc.substr(pos, 6); // </head

    if (!iequals(closeHead, "</head")) {
        return false;
    }
    pos += 6;


    if (!properEndTag(doc, pos)) {
        return false;
    }

    pos += 1; //it now right after </head>
    advanceToNextChar(doc, pos);


    std::string_view oBody = doc.substr(pos, 5);

    if (!iequals(oBody, "<body")) {
        return false;
    }


    if (iequals(oBody, "<body")) {   //if there is a body tag
        pos += 5;
        advanceToNextChar(doc, pos);
        if (doc.substr(pos, 2).compare("id") == 0) {
            if (!validID(doc, pos)) {
                return false;
            }
        }
        if (!properEndTag(doc, pos)) {
            return false;
        }


        pos += 1;
        std::string_view cBody = doc.substr(pos, 6);
        while (!iequals(cBody, "</body")) { //while searching for the closing body tag

            if (iequals(doc.substr(pos, 2), "<p")) { //if there's a p tag
                if (!validPTag(doc, pos)) {
                    return false;
                }
            } else if (iequals(doc.substr(pos, 4), "<div")) {  //if theres a div tag
                if (!validDivTag(doc, pos)) {
                    return false;
                }

            } else if (iequals(doc.substr(pos, 3), "<br")) {
                if (!validBRTag(doc, pos)) {
                    return false;
                }
            } else if (iequals(doc.substr(pos, 5), "<span")) {
                if (!validSpanTag(doc, pos)) {
                    return false;
                }
            } else if (iequals(doc.substr(pos, 6), "</html")) {
                return false;
            } else if (iequals(doc.substr(pos, 5), "</div")) {
                return false;
            }

            pos += 1;
            cBody = doc.substr(pos, 6);
        }

        pos += 6;
        if (!properEndTag(doc, pos)) {
            return false;
        }
        if (pos == doc.size() - 1) {
            return false;
        }
        pos += 1;  //it now right after </body>
    }

    if (pos > doc.size() - 5) {
        return false;
    }
    advanceToNextChar(doc, pos);
    std::string_view cHtml = doc.substr(pos, 6);

    if (!iequals(cHtml, "</html")) {
        return false;
    }

    pos += 6;
    if (!properEndTag(doc, pos)) {
        return false;
    }
    pos += 1;

    while (pos < doc.size()) {
        if (!std::isspace(static_cast<unsigned char>(doc.at(pos)))) {
            return false;
        }
        pos += 1;
    }

    return true;

}


Tag *getElementByID(Tag *const root, const std::string &id) {

    if (root == nullptr || id.empty()) {
        return nullptr;
    }

    if (root->_id.compare(id) == 0) {
        return root;
    }

    for (const auto &child : root->_children) {
        Tag *result = getElementByID(child.get(), id);
        if (result != nullptr) {
            return result;
        }
    }

    return nullptr;
}


namespace {


bool iequals(std::string_view a, std::string_view b) {
    return a.size() == b.size() &&
           std::equal(a.begin(), a.end(), b.begin(), b.end(),
                      [](char x, char y) {
                          return std::tolower(static_cast<unsigned char>(x)) ==
                                 std::tolower(static_cast<unsigned char>(y));
                      });
}

void advanceToNextChar(std::string_view str, std::size_t &pos) {
    while (std::isspace(static_cast<unsigned char>(str.at(pos)))) {
        ++pos;
    }
}

[[nodiscard]] bool properEndTag(std::string_view document, std::size_t &pos) {
    advanceToNextChar(document, pos);
    if (document.at(pos) == '>') {
        return true;
    }
    return false;
}

[[nodiscard]] bool validPTag(std::string_view document, std::size_t &pos) {
    pos += 2;
    advanceToNextChar(document, pos);

    if (document.substr(pos, 2).compare("id") == 0) {
        if (!validID(document, pos)) {
            return false;
        }
    }


    if (!properEndTag(document, pos)) {
        return false;
    }
    pos += 1;                                  //need to check if there are closing tags inbetween


    std::string_view cP = document.substr(pos, 3);

    while (!iequals(cP, "</p")) { //while searching for the closing p tag

        std::string_view cHtml = document.substr(pos, 6);
        if (iequals(cHtml, "</html")) {
            return false;
        }
        std::string_view cBody = document.substr(pos, 6);
        if (iequals(cBody, "</body")) {
            return false;
        }
        std::string_view oSPan = document.substr(pos, 5);
        if (iequals(oSPan, "<span")) {  //if theres a span tag in the p tag
            if (!validSpanTag(document, pos)) {
                return false;
            }

        }
        std::string_view br = document.substr(pos, 3);
        if (iequals(br, "<br")) { //if br tag inside div
            if (!validBRTag(document, pos)) {
                return false;
            }

        }

        pos += 1;
        cP = document.substr(pos, 3);
        if (pos > document.size()) {
            return false;
        }
    }
    pos += 3;

    if (!properEndTag(document, pos)) {
        return false;
    }
    pos += 1;
    return true;
}


[[nodiscard]] bool validSpanTag(std::string_view document, std::size_t &pos) {
    pos += 5;
    advanceToNextChar(document, pos);

    if (document.substr(pos, 2).compare("id") == 0) {
        if (!validID(document, pos)) {
            return false;
        }
    }

    if (!properEndTag(document, pos)) {
        return false;
    }
    pos += 1;
    std::string_view cSPan = document.substr(pos, 6);
    while (!iequals(cSPan, "</span")) {

        std::string_view oP = document.substr(pos, 2);
        if (iequals(oP, "<p")) {
            return false;
        }

        std::string_view oSpan = document.substr(pos, 5);
        if (iequals(oSpan, "<span")) {
            return false;
        }

        std::string_view cHtml = document.substr(pos, 6);
        if (iequals(cHtml, "</html")) {
            return false;
        }
        std::string_view cBody = document.substr(pos, 6);
        if (iequals(cBody, "</body")) {
            return false;
        }
        std::string_view oDiv = document.substr(pos, 4);
        if (iequals(oDiv, "<div")) {
            return false;
        }
        if (pos > document.size()) {
            return false;
        }
        pos += 1;
        cSPan = document.substr(pos, 6);
    }
    pos += 6;

    if (!properEndTag(document, pos)) {
        return false;
    }

    return true;
}

[[nodiscard]] bool validDivTag(std::string_view document, std::size_t &pos) {
    //can have div p br span
    pos += 4;
    advanceToNextChar(document, pos);

    if (document.substr(pos, 2).compare("id") == 0) {
        if (!validID(document, pos)) {
            return false;
        }
    }

    if (!properEndTag(document, pos)) {
        return false;
    }
    pos += 1;
    std::string_view cDiv = document.substr(pos, 5);
    while (!iequals(cDiv, "</div")) { //while searching for ending div tag

        std::string_view cHtml = document.substr(pos, 6);
        if (iequals(cHtml, "</html")) {
            return false;
        }
        std::string_view cBody = document.substr(pos, 6);
        if (iequals(cBody, "</body")) {
            return false;
        }

        std::string_view oP = document.substr(pos, 2);
        if (iequals(oP, "<p")) { //if p tag inside div
            if (!validPTag(document, pos)) {
                return false;
            }
        }
        std::string_view oSPan = document.substr(pos, 5);
        if (iequals(oSPan, "<span")) { //if span tag inside div
            if (!validSpanTag(document, pos)) {
                return false;
            }
        }
        std::string_view br = document.substr(pos, 3);
        if (iequals(br, "<br")) { //if br tag inside div
            if (!validBRTag(document, pos)) {
                return false;
            }

        }


        std::string_view oDiv = document.substr(pos, 4);
        if (iequals(oDiv, "<div")) {
            if (!validDivTag(document, pos)) {
                return false;
            }
        }

        pos += 1;
        cDiv = document.substr(pos, 5);
    }
    pos += 5;
    if (!properEndTag(document, pos)) {
        return false;
    }
    return true;
}

[[nodiscard]] bool validBRTag(std::string_view document, std::size_t &pos) {

    pos += 3;
    advanceToNextChar(document, pos);

    if (document.substr(pos, 2).compare("id") == 0) {  //if theres an id
        pos += 2;
        advanceToNextChar(document, pos);

        if (document.at(pos) != '=') {
            return false;
        }
        pos += 1;
        advanceToNextChar(document, pos);
        if (document.at(pos) != '"') {
            return false;
        }
        pos += 1;
        while (document.at(pos) != '"') {

            if (std::isspace(static_cast<unsigned char>(document.at(pos)))) {
                return false;
            }
            pos += 1;
        }
        pos += 1;
        advanceToNextChar(document, pos);
    }

    if (document.at(pos) == '/') {
        pos += 1;
        if (document.at(pos) != '>') {
            return false;
        }
    } else {
        if (document.at(pos) != '>') {
            return false;
        }
    }
    return true;
}

[[nodiscard]] bool validID(std::string_view document, std::size_t &pos) {
    pos += 2;
    advanceToNextChar(document, pos);

    if (document.at(pos) != '=') {
        return false;
    }
    pos += 1;
    advanceToNextChar(document, pos);
    if (document.at(pos) != '"') {
        return false;
    }
    pos += 1;
    while (document.at(pos) != '"') {

        if (document.at(pos) == '/') {
            return false;
        }
        if (std::isspace(static_cast<unsigned char>(document.at(pos)))) {
            return false;
        }
        pos += 1;
    }
    pos += 1;
    advanceToNextChar(document, pos);

    if (document.at(pos) != '>') {
        return false;
    }

    return true;
}

} // namespace
