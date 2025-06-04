
#include "Tag.hpp"
#include <algorithm>
#include <cctype>
#include <memory>
#include <ranges>
#include <sstream>
#include <vector>
#include <cstddef>
#include <string_view>

std::string lower(std::string str);

std::size_t it;

std::size_t advanceitToNextChar(std::string_view str);

[[nodiscard]] bool properEndTag(std::string_view document);

[[nodiscard]] bool validPTag(std::string_view document);

[[nodiscard]] bool validSpanTag(std::string_view document);

[[nodiscard]] bool validDivTag(std::string_view document);

[[nodiscard]] bool validBRTag(std::string_view document);

[[nodiscard]] bool validID(std::string_view document);


[[nodiscard]] bool html_is_valid(const std::string &document) {

    if (document.empty()) {
        return false;
    }
    it = 0;


    it = advanceitToNextChar(document);

    if (document.size() < it + 15) {
        return false;
    }

    std::string docType(document.substr(it, 9)); //<!doctype
    docType = lower(docType);

    if (docType.compare("<!doctype") != 0) {
        return false;
    }
    it += 9;
    it = advanceitToNextChar(document);

    std::string h(document.substr(it, 4)); //html
    h = lower(h);

    if (h.compare("html") != 0) {
        return false;
    }
    it += 4;

    if (!properEndTag(document)) {
        return false;
    }

    it += 1;
    it = advanceitToNextChar(document);

    std::string html(document.substr(it, 5)); //<html
    html = lower(html);

    if (html.compare("<html") != 0) {
        return false;
    }
    it += 5;
    it = advanceitToNextChar(document);
    if (document.substr(it, 2).compare("id") == 0) {
        if (!validID(document)) {
            return false;
        }
    }

    if (!properEndTag(document)) {
        return false;
    }
    it += 1;
    it = advanceitToNextChar(document);

    std::string head(document.substr(it, 5)); //<head
    head = lower(head);

    if (head.compare("<head") != 0) {
        return false;
    }
    it += 5;
    it = advanceitToNextChar(document);
    if (document.substr(it, 2).compare("id") == 0) {
        if (!validID(document)) {
            return false;
        }
    }
    if (!properEndTag(document)) {
        return false;
    }
    it += 1;  // it is now right after '<head>'
    it = advanceitToNextChar(document);

    std::string title(document.substr(it, 6)); //<title
    title = lower(title);

    if (title.compare("<title") != 0) {
        return false;
    }
    it += 6;
    it = advanceitToNextChar(document);
    if (document.substr(it, 2).compare("id") == 0) {
        if (!validID(document)) {
            return false;
        }
    }

    if (!properEndTag(document)) {
        return false;
    }
    it += 1; //it now right after <title>
    it = advanceitToNextChar(document);

    std::string cTitle(document.substr(it, 7));
    cTitle = lower(cTitle);

    if (cTitle.compare("</title") == 0) { //if title is empty
        return false;
    }


    while (cTitle.compare("</title") != 0) { //while searching for closing title tag

        if (document.substr(it, 6).compare("</html") == 0) {
            return false;
        }
        it += 1;
        cTitle = document.substr(it, 7);
        cTitle = lower(cTitle);
        if (it > document.size()) {
            return false;
        }
    }
    it += 7;


    if (!properEndTag(document)) {
        return false;
    }
    it += 1;  //it now after </title>
    it = advanceitToNextChar(document);

    std::string closeHead(document.substr(it, 6)); // </head
    closeHead = lower(closeHead);

    if (closeHead.compare("</head") != 0) {
        return false;
    }
    it += 6;


    if (!properEndTag(document)) {
        return false;
    }

    it += 1; //it now right after </head>
    it = advanceitToNextChar(document);


    std::string oBody(document.substr(it, 5));
    oBody = lower(oBody);

    if (oBody.compare("<body") != 0) {
        return false;
    }


    if (oBody.compare("<body") == 0) {   //if there is a body tag
        it += 5;
        it = advanceitToNextChar(document);
        if (document.substr(it, 2).compare("id") == 0) {
            if (!validID(document)) {
                return false;
            }
        }
        if (!properEndTag(document)) {
            return false;
        }


        it += 1;
        std::string cBody(document.substr(it, 6));
        std::string cBoy(document.substr(it, 100));
        cBody = lower(cBody);
        while (cBody.compare("</body") != 0) { //while searching for the closing body tag

            if (document.substr(it, 2).compare("<p") == 0) { //if there's a p tag
                if (!validPTag(document)) {
                    return false;
                }
            } else if (document.substr(it, 4).compare("<div") == 0) {  //if theres a div tag
                if (!validDivTag(document)) {
                    return false;
                }

            } else if (document.substr(it, 3).compare("<br") == 0) {
                if (!validBRTag(document)) {
                    return false;
                }
            } else if (document.substr(it, 5).compare("<span") == 0) {
                if (!validSpanTag(document)) {
                    return false;
                }
            } else if (document.substr(it, 6).compare("</html") == 0) {
                return false;
            } else if (document.substr(it, 5).compare("</div") == 0) {
                return false;
            }

            it += 1;
            cBody = document.substr(it, 6);
            cBody = lower(cBody);
            cBoy = document.substr(it, 100);
        }

        it += 6;
        if (!properEndTag(document)) {
            return false;
        }
        if (it == document.size() - 1) {
            return false;
        }
        it += 1;  //it now right after </body>
    }

    if (it > document.size() - 5) {
        return false;
    }
    it = advanceitToNextChar(document);
    std::string cHtml(document.substr(it, 6));
    cHtml = lower(cHtml);

    if (cHtml.compare("</html") != 0) {
        return false;
    }

    it += 6;
    if (!properEndTag(document)) {
        return false;
    }
    it += 1;

    while (it < document.size()) {
        if (!std::isspace(static_cast<unsigned char>(document.at(it)))) {
            return false;
        }
        it += 1;
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


std::string lower(std::string str) {
    std::ranges::transform(str, str.begin(),
                           [](unsigned char ch) { return static_cast<char>(std::tolower(ch)); });
    return str;
}

std::size_t advanceitToNextChar(std::string_view str) {
    while (std::isspace(static_cast<unsigned char>(str.at(it)))) {
        ++it;
    }
    return it;
}

[[nodiscard]] bool properEndTag(std::string_view document) {
    it = advanceitToNextChar(document);
    if (document.at(it) == '>') {
        return true;
    }
    return false;
}

[[nodiscard]] bool validPTag(std::string_view document) {
    it += 2;
    it = advanceitToNextChar(document);

    if (document.substr(it, 2).compare("id") == 0) {
        if (!validID(document)) {
            return false;
        }
    }


    if (!properEndTag(document)) {
        return false;
    }
    it += 1;                                  //need to check if there are closing tags inbetween


    std::string cP(document.substr(it, 3));
    cP = lower(cP);

    while (cP.compare("</p") != 0) { //while searching for the closing p tag

        std::string cHtml(document.substr(it, 6));
        cHtml = lower(cHtml);
        if (cHtml.compare("</html") == 0) {
            return false;
        }
        std::string cBody(document.substr(it, 6));
        cBody = lower(cBody);
        if (cBody.compare("</body") == 0) {
            return false;
        }
        std::string oSPan(document.substr(it, 5));
        oSPan = lower(oSPan);
        if (oSPan.compare("<span") == 0) {  //if theres a span tag in the p tag
            if (!validSpanTag(document)) {
                return false;
            }

        }
        std::string br(document.substr(it, 3));
        br = lower(br);
        if (br.compare("<br") == 0) { //if br tag inside div
            if (!validBRTag(document)) {
                return false;
            }

        }

        it += 1;
        cP = document.substr(it, 3);
        cP = lower(cP);
        if (it > document.size()) {
            return false;
        }
    }
    it += 3;

    if (!properEndTag(document)) {
        return false;
    }
    it += 1;
    return true;
}

[[nodiscard]] bool validSpanTag(std::string_view document) {
    it += 5;
    it = advanceitToNextChar(document);

    if (document.substr(it, 2).compare("id") == 0) {
        if (!validID(document)) {
            return false;
        }
    }

    if (!properEndTag(document)) {
        return false;
    }
    it += 1;
    std::string cSPan(document.substr(it, 6));
    cSPan = lower(cSPan);
    while (cSPan.compare("</span") != 0) {

        std::string oP(document.substr(it, 2));
        oP = lower(oP);
        if (oP.compare("<p") == 0) {
            return false;
        }

        std::string oSpan(document.substr(it, 5));
        oSpan = lower(oSpan);
        if (oSpan.compare("<span") == 0) {
            return false;
        }

        std::string cHtml(document.substr(it, 6));
        cHtml = lower(cHtml);
        if (cHtml.compare("</html") == 0) {
            return false;
        }
        std::string cBody(document.substr(it, 6));
        cBody = lower(cBody);
        if (cBody.compare("</body") == 0) {
            return false;
        }
        std::string oDiv(document.substr(it, 4));
        oDiv = lower(oDiv);
        if (oDiv.compare("<div") == 0) {
            return false;
        }
        if (it > document.size()) {
            return false;
        }
        it += 1;
        cSPan = document.substr(it, 6);
        cSPan = lower(cSPan);
    }
    it += 6;

    if (!properEndTag(document)) {
        return false;
    }

    return true;
}

[[nodiscard]] bool validDivTag(std::string_view document) {
    //can have div p br span
    it += 4;
    it = advanceitToNextChar(document);

    if (document.substr(it, 2).compare("id") == 0) {
        if (!validID(document)) {
            return false;
        }
    }

    if (!properEndTag(document)) {
        return false;
    }
    it += 1;
    std::string cDiv(document.substr(it, 5));
    cDiv = lower(cDiv);
    while (cDiv.compare("</div") != 0) { //while searching for ending div tag

        std::string cHtml(document.substr(it, 6));
        cHtml = lower(cHtml);
        if (cHtml.compare("</html") == 0) {
            return false;
        }
        std::string cBody(document.substr(it, 6));
        cBody = lower(cBody);
        if (cBody.compare("</body") == 0) {
            return false;
        }

        std::string oP(document.substr(it, 2));
        oP = lower(oP);
        if (oP.compare("<p") == 0) { //if p tag inside div
            if (!validPTag(document)) {
                return false;
            }
        }
        std::string oSPan(document.substr(it, 5));
        oSPan = lower(oSPan);
        if (oSPan.compare("<span") == 0) { //if span tag inside div
            if (!validSpanTag(document)) {
                return false;
            }
        }
        std::string br(document.substr(it, 3));
        br = lower(br);
        if (br.compare("<br") == 0) { //if br tag inside div
            if (!validBRTag(document)) {
                return false;
            }

        }


        std::string oDiv(document.substr(it, 4));
        oDiv = lower(oDiv);
        if (oDiv.compare("<div") == 0) {
            if (!validDivTag(document)) {
                return false;
            }
        }

        it += 1;
        cDiv = document.substr(it, 5);
        cDiv = lower(cDiv);
    }
    it += 5;
    if (!properEndTag(document)) {
        return false;
    }
    return true;
}

[[nodiscard]] bool validBRTag(std::string_view document) {

    it += 3;
    it = advanceitToNextChar(document);

    if (document.substr(it, 2).compare("id") == 0) {  //if theres an id
        it += 2;
        it = advanceitToNextChar(document);

        if (document.at(it) != '=') {
            return false;
        }
        it += 1;
        it = advanceitToNextChar(document);
        if (document.at(it) != '"') {
            return false;
        }
        it += 1;
        while (document.at(it) != '"') {

            if (std::isspace(static_cast<unsigned char>(document.at(it)))) {
                return false;
            }
            it += 1;
        }
        it += 1;
        it = advanceitToNextChar(document);
    }

    if (document.at(it) == '/') {
        it += 1;
        if (document.at(it) != '>') {
            return false;
        }
    } else {
        if (document.at(it) != '>') {
            return false;
        }
    }
    return true;
}

[[nodiscard]] bool validID(std::string_view document) {
    it += 2;
    it = advanceitToNextChar(document);

    if (document.at(it) != '=') {
        return false;
    }
    it += 1;
    it = advanceitToNextChar(document);
    if (document.at(it) != '"') {
        return false;
    }
    it += 1;
    while (document.at(it) != '"') {

        if (document.at(it) == '/') {
            return false;
        }
        if (std::isspace(static_cast<unsigned char>(document.at(it)))) {
            return false;
        }
        it += 1;
    }
    it += 1;
    it = advanceitToNextChar(document);

    if (document.at(it) != '>') {
        return false;
    }

    return true;
}