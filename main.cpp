#include <iostream>
#include <fstream>
#include <sstream>

bool html_is_valid(const std::string &document);

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " file.html" << std::endl;
        return 1;
    }
    std::ifstream in(argv[1]);
    if (!in) {
        std::cerr << "Could not open file" << std::endl;
        return 1;
    }
    std::ostringstream ss;
    ss << in.rdbuf();
    std::string doc = ss.str();
    bool valid = html_is_valid(doc);
    std::cout << (valid ? "valid" : "invalid") << std::endl;
    return 0;
}
