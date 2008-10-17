// filter.cpp
//
// Copyright (c) 2008
// Steven Watanabe
// 
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <string>
#include <cstdio>
#include <cstring>

const char* search("template_profiler");

#if defined(_MSC_VER)
    const char* back_trace_search("see reference to");
#elif defined(__GNUC__)
    const char* back_trace_search("instantiated from");
#else
    #error only Microsoft and gcc are supported.
#endif

void copy_flat_only() {
    std::string buffer;
    int ch;
    int pos = 0;
    bool matched = false;
    int counter = 0;
    while((ch = std::getchar()) != EOF) {
        buffer.push_back(static_cast<char>(ch));
        if(ch == '\n') {
            if(matched) {
                for(std::size_t i = 0; i < buffer.size(); ++i) {
                    std::putchar(buffer[i]);
                }
                ++counter;
#ifdef _MSC_VER
                int instantiations = counter / 2;
#else
                int instantiations = counter;
#endif
                if(++instantiations % 100 == 0) {
                    std::fprintf(stderr, "On Instantiation %d\n", instantiations);
                }
            }
            buffer.clear();
            matched = false;
        }
        if(ch == search[pos]) {
            ++pos;
            if(search[pos] == '\0') {
                matched = true;
            }
        } else {
            pos = 0;
        }
    }
}

void copy_call_graph() {
#ifdef _MSC_VER
    std::string buffer;
    int ch;
    int pos = 0;
    bool matched = false;
    int counter = 0;
    while((ch = std::getchar()) != EOF) {
        buffer.push_back(static_cast<char>(ch));
        if(ch == '\n') {
            if(matched) {
                for(std::size_t i = 0; i < buffer.size(); ++i) {
                    std::putchar(buffer[i]);
                }
                ++counter;
#ifdef _MSC_VER
                int instantiations = counter / 2;
#else
                int instantiations = counter;
#endif
                if(++instantiations % 100 == 0) {
                    std::fprintf(stderr, "On Instantiation %d\n", instantiations);
                }
                buffer.clear();
                matched = false;
                // process instantiation back-trace
                pos = 0;
                while((ch = std::getchar()) != EOF) {
                    if(ch == ' ') {
                        buffer.push_back(static_cast<char>(ch));
                        while((ch = std::getchar()) != EOF) {
                            buffer.push_back(static_cast<char>(ch));
                            if(ch == '\n') {
                                if(matched) {
                                    for(std::size_t i = 0; i < buffer.size(); ++i) {
                                        std::putchar(buffer[i]);
                                    }
                                }
                                buffer.clear();
                                matched = false;
                                pos = 0;
                                break;
                            }
                            if(ch == back_trace_search[pos]) {
                                ++pos;
                                if(back_trace_search[pos] == '\0') {
                                    matched = true;
                                }
                            } else {
                                pos = 0;
                            }
                        }
                    } else {
                        std::ungetc(ch, stdin);
                        break;
                    }
                }
            }
            buffer.clear();
            matched = false;
            pos = 0;
        }
        if(ch == search[pos]) {
            ++pos;
            if(search[pos] == '\0') {
                matched = true;
            }
        } else {
            pos = 0;
        }
    }
#elif defined(__GNUC__)
    // trying to figure out what we should copy is too hard.
    int ch;
    while((ch = std::getchar()) != EOF) {
        std::putchar(ch);
    }
#else
    #error Unknown compiler
#endif
}

int main(int argc, char** argv) {
    if(argc == 2 && std::strcmp(argv[1], "--call-graph") == 0) {
        copy_call_graph();
    } else {
        copy_flat_only();
    }
}
