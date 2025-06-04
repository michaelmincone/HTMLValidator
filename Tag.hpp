#ifndef TAG_HPP
#define TAG_HPP

#include <memory>
#include <string>
#include <vector>

struct Tag {
    std::string _name;
    std::string _id;
    std::vector<std::unique_ptr<Tag>> _children;
};

#endif // TAG_HPP
