
#include <boost/local/function.hpp>
#include <iostream>

int print(int* array, int offset, int size) {
    int BOOST_LOCAL_FUNCTION_PARAMS( (int* array) (int index)
            (const bind offset) (const bind size) ) {
        if (index > size) goto failure;
        return array[index + offset];
    } BOOST_LOCAL_FUNCTION_NAME(access)

    for (int i = 0; i < size; ++i) {
        std::cout << access(array, i) << std::endl;
    }
    return 0;

failure:
    return -1;
}

int main() {
    int a[] = {1, 2, 3, 4, 5};
    print(a, 1, 2);
    return 0;
}

