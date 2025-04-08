#include "input_stream.h"
#include <iostream>

FileInputStream::FileInputStream(const std::string& path) {
    file.open(path, std::ios::in);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + path);
    }
}

char FileInputStream::get_next_char() {
    int result;
    result = file.get();
    return result;
}

char FileInputStream::peek() {
    return file.peek();
}

FileInputStream::~FileInputStream() {
    file.close();
}

StringInputStream::StringInputStream(const std::string& input) :
    stream(input) {}

char StringInputStream::get_next_char() {
    return stream.get();
}

char StringInputStream::peek() {
    return stream.peek();
}
