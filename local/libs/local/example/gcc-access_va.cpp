
#include <boost/local/function.hpp>

void bar(int* array, int offset, int size) {
    int BOOST_LOCAL_FUNCTION_PARAMS(int* array, int index, const bind offset) {
        return array[index + offset];
    }

    for (int i = 0; i < size; ++i) {
        std::cout << access(array, i) << std::endl;
    }
}


