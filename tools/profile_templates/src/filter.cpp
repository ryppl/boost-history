#include <string>
#include <cstdio>

const char* search("template_profiler");

int main() {
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
