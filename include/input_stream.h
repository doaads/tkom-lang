#pragma once


class InputStream {
    public:
        virtual char get_next_char() = 0;
        virtual bool eof() const = 0;
};
