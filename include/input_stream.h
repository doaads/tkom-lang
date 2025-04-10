#pragma once
#include <fstream>
#include <sstream>
#include "position.h"

class InputStream {
    private:
        Position position;
        void adjust_position(char ch);
    public:
        virtual ~InputStream() = default;
        virtual char get_next_char() = 0;
        virtual char peek() = 0;
};


class FileInputStream : public InputStream {
    private:
        std::ifstream file;
    public:
        FileInputStream(const std::string& path);
        char get_next_char() override;
        char peek() override;
        ~FileInputStream();
};

class StringInputStream : public InputStream {
    private:
        std::istringstream stream;
    public:
        StringInputStream(const std::string& input);
        char get_next_char() override;
        char peek() override;
};
