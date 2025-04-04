#pragma once


class InputStream {
    public:
        virtual char getNextChar() = 0;
        virtual bool eof() const = 0;
};
