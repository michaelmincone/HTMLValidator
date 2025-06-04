#ifndef TAG_HPP
#define TAG_HPP

#include <string>
#include <list>

struct Tag {
    std::string _name;
    std::string _id;
    std::list<Tag*> _children;
};

#endif // TAG_HPP
