#include "log.h"

using namespace boost::logging;

namespace {

    int val() {
        static int i = 0;
        return ++i;
    }

    void use() {
        int i = (!std::cout);
        LDBG_ << "this is so cool " << i++;
        LDBG_ << "this is so cool again " << i++;
        LERR_ << "first error " << i++;

        std::string hello = "hello", world = "world";
        LAPP_ << hello << ", " << world;

        g_l_filter()->set_enabled(level::error);
        LDBG_ << "this will not be written anywhere";
        LAPP_ << "this won't be written anywhere either";

        g_l_filter()->set_enabled(level::info);
        LAPP_ << "good to be back ;) " << i++;
        LERR_ << "second error " << i++;

        LAPP_ << "this is so cool " << i++;
        LAPP_ << "this is so cool again " << i++;
        LERR_ << "third error " << i++;



        LAPP_ << "this is so cool " << val();
        LAPP_ << "this is so cool again " << val();
        LERR_ << "first error " << val();

        LAPP_ << hello << ", " << world;

        g_l_filter()->set_enabled(level::error);
        LAPP_ << "this will not be written anywhere";
        LAPP_ << "this won't be written anywhere either";

        g_l_filter()->set_enabled(level::info);
        LAPP_ << "good to be back ;) " << val();
        LERR_ << "second error " << val();

        LDBG_ << "this is so cool " << val();
        LAPP_ << "this is so cool again " << val();
        LERR_ << "third error " << val();
    }
}
