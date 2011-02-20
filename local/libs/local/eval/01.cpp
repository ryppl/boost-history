

struct X {
};

template<typename T>
void f() {}

int main() {
    f<struct Y {}>();
    return 0;
}

