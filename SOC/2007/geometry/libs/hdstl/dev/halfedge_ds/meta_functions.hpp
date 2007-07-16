struct true_type {};
struct false_type{};

template <typename T1, typename T2>
struct meta_is_same {
    enum {value = false};
    typedef false_type type;
};

template<typename T>
struct meta_is_same<T,T> {
    enum {value = true};
    typedef true_type type;
};

