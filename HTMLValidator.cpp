

#include "Tag.hpp"
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <memory>
#include <ranges>
#include <sstream>
#include <string_view>
#include <vector>

namespace {

[[nodiscard]] std::string lower(std::string str);

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


    advanceToNextChar(document, pos);

    if (document.size() < pos + 15) {
        return false;
    }

    std::string docType(document.substr(pos, 9)); //<!doctype
    docType = lower(docType);

    if (docType.compare("<!doctype") != 0) {
        return false;
    }
    pos += 9;
    advanceToNextChar(document, pos);

    std::string h(document.substr(pos, 4)); //html
    h = lower(h);

    if (h.compare("html") != 0) {
        return false;
    }
    pos += 4;

    if (!properEndTag(document, pos)) {
        return false;
    }

    pos += 1;
    advanceToNextChar(document, pos);

    std::string html(document.substr(pos, 5)); //<html
    html = lower(html);

    if (html.compare("<html") != 0) {
        return false;
    }
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
    advanceToNextChar(document, pos);

    std::string head(document.substr(pos, 5)); //<head
    head = lower(head);

    if (head.compare("<head") != 0) {
        return false;
    }
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
    pos += 1;  // it is now right after '<head>'
    advanceToNextChar(document, pos);

    std::string title(document.substr(pos, 6)); //<title
    title = lower(title);

    if (title.compare("<title") != 0) {
        return false;
    }
    pos += 6;
    advanceToNextChar(document, pos);
    if (document.substr(pos, 2).compare("id") == 0) {
        if (!validID(document, pos)) {
            return false;
        }
    }

    if (!properEndTag(document, pos)) {
        return false;
    }
    pos += 1; //it now right after <title>
    advanceToNextChar(document, pos);

    std::string cTitle(document.substr(pos, 7));
    cTitle = lower(cTitle);

    if (cTitle.compare("</title") == 0) { //if title is empty
        return false;
    }


    while (cTitle.compare("</title") != 0) { //while searching for closing title tag

        if (document.substr(pos, 6).compare("</html") == 0) {
            return false;
        }
        pos += 1;
        cTitle = document.substr(pos, 7);
        cTitle = lower(cTitle);
        if (pos > document.size()) {
            return false;
        }
    }
    pos += 7;


    if (!properEndTag(document, pos)) {
        return false;
    }
    pos += 1;  //it now after </title>
    advanceToNextChar(document, pos);

    std::string closeHead(document.substr(pos, 6)); // </head
    closeHead = lower(closeHead);

    if (closeHead.compare("</head") != 0) {
        return false;
    }
    pos += 6;


    if (!properEndTag(document, pos)) {
        return false;
    }

    pos += 1; //it now right after </head>
    advanceToNextChar(document, pos);


    std::string oBody(document.substr(pos, 5));
    oBody = lower(oBody);

    if (oBody.compare("<body") != 0) {
        return false;
    }


    if (oBody.compare("<body") == 0) {   //if there is a body tag
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
        std::string cBody(document.substr(pos, 6));
        std::string cBoy(document.substr(pos, 100));
        cBody = lower(cBody);
        while (cBody.compare("</body") != 0) { //while searching for the closing body tag

            if (document.substr(pos, 2).compare("<p") == 0) { //if there's a p tag
                if (!validPTag(document, pos)) {
                    return false;
                }
            } else if (document.substr(pos, 4).compare("<div") == 0) {  //if theres a div tag
                if (!validDivTag(document, pos)) {
                    return false;
                }

            } else if (document.substr(pos, 3).compare("<br") == 0) {
                if (!validBRTag(document, pos)) {
                    return false;
                }
            } else if (document.substr(pos, 5).compare("<span") == 0) {
                if (!validSpanTag(document, pos)) {
                    return false;
                }
            } else if (document.substr(pos, 6).compare("</html") == 0) {
                return false;
            } else if (document.substr(pos, 5).compare("</div") == 0) {
                return false;
            }

            pos += 1;
            cBody = document.substr(pos, 6);
            cBody = lower(cBody);
            cBoy = document.substr(pos, 100);
        }

        pos += 6;
        if (!properEndTag(document, pos)) {
            return false;
        }
        if (pos == document.size() - 1) {
            return false;
        }
        pos += 1;  //it now right after </body>
    }

    if (pos > document.size() - 5) {
        return false;
    }
    advanceToNextChar(document, pos);
    std::string cHtml(document.substr(pos, 6));
    cHtml = lower(cHtml);

    if (cHtml.compare("</html") != 0) {
        return false;
    }

    pos += 6;
    if (!properEndTag(document, pos)) {
        return false;
    }
    pos += 1;

    while (pos < document.size()) {
        if (!std::isspace(static_cast<unsigned char>(document.at(pos)))) {
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

std::string lower(std::string str) {
    std::ranges::transform(str, str.begin(),
                           [](unsigned char ch) { return static_cast<char>(std::tolower(ch)); });
    return str;
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


    std::string cP(document.substr(pos, 3));
    cP = lower(cP);

    while (cP.compare("</p") != 0) { //while searching for the closing p tag

        std::string cHtml(document.substr(pos, 6));
        cHtml = lower(cHtml);
        if (cHtml.compare("</html") == 0) {
            return false;
        }
        std::string cBody(document.substr(pos, 6));
        cBody = lower(cBody);
        if (cBody.compare("</body") == 0) {
            return false;
        }
        std::string oSPan(document.substr(pos, 5));
        oSPan = lower(oSPan);
        if (oSPan.compare("<span") == 0) {  //if theres a span tag in the p tag
            if (!validSpanTag(document, pos)) {
                return false;
            }

        }
        std::string br(document.substr(pos, 3));
        br = lower(br);
        if (br.compare("<br") == 0) { //if br tag inside div
            if (!validBRTag(document, pos)) {
                return false;
            }

        }

        pos += 1;
        cP = document.substr(pos, 3);
        cP = lower(cP);
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
    std::string cSPan(document.substr(pos, 6));
    cSPan = lower(cSPan);
    while (cSPan.compare("</span") != 0) {

        std::string oP(document.substr(pos, 2));
        oP = lower(oP);
        if (oP.compare("<p") == 0) {
            return false;
        }

        std::string oSpan(document.substr(pos, 5));
        oSpan = lower(oSpan);
        if (oSpan.compare("<span") == 0) {
            return false;
        }

        std::string cHtml(document.substr(pos, 6));
        cHtml = lower(cHtml);
        if (cHtml.compare("</html") == 0) {
            return false;
        }
        std::string cBody(document.substr(pos, 6));
        cBody = lower(cBody);
        if (cBody.compare("</body") == 0) {
            return false;
        }
        std::string oDiv(document.substr(pos, 4));
        oDiv = lower(oDiv);
        if (oDiv.compare("<div") == 0) {
            return false;
        }
        if (pos > document.size()) {
            return false;
        }
        pos += 1;
        cSPan = document.substr(pos, 6);
        cSPan = lower(cSPan);
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
    std::string cDiv(document.substr(pos, 5));
    cDiv = lower(cDiv);
    while (cDiv.compare("</div") != 0) { //while searching for ending div tag

        std::string cHtml(document.substr(pos, 6));
        cHtml = lower(cHtml);
        if (cHtml.compare("</html") == 0) {
            return false;
        }
        std::string cBody(document.substr(pos, 6));
        cBody = lower(cBody);
        if (cBody.compare("</body") == 0) {
            return false;
        }

        std::string oP(document.substr(pos, 2));
        oP = lower(oP);
        if (oP.compare("<p") == 0) { //if p tag inside div
            if (!validPTag(document, pos)) {
                return false;
            }
        }
        std::string oSPan(document.substr(pos, 5));
        oSPan = lower(oSPan);
        if (oSPan.compare("<span") == 0) { //if span tag inside div
            if (!validSpanTag(document, pos)) {
                return false;
            }
        }
        std::string br(document.substr(pos, 3));
        br = lower(br);
        if (br.compare("<br") == 0) { //if br tag inside div
            if (!validBRTag(document, pos)) {
                return false;
            }

        }


        std::string oDiv(document.substr(pos, 4));
        oDiv = lower(oDiv);
        if (oDiv.compare("<div") == 0) {
            if (!validDivTag(document, pos)) {
                return false;
            }
        }

        pos += 1;
        cDiv = document.substr(pos, 5);
        cDiv = lower(cDiv);
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
