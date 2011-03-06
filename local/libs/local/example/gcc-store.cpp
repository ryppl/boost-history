
void intermediate(void (*store_func)(int, int), int size) {
    store_func(size - 1, -1);
}

void hack(int* array, int size) {
    void BOOST_LOCAL_FUNCTION_PARAMS(int index, int value, bind array) {
        array[index] = value;
    } BOOST_LOCAL_FUNCTION_NAME(store)

    intermediate(store, size);
}


