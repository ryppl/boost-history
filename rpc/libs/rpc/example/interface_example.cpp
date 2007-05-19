template<typename Registry>
class remote_interface
{
public:
    remote_interface(rpc::client<Registry> &client) : client(client) {};

    boost::future<void> nothing()
    {   
        return client(rpc::call<int, void()> (0));
    }
    boost::future<int> inc(int i)
    {   
        return client(rpc::call<int, int(int)> (1, i));
    }
    boost::future<int> add2(int i, int j)
    {   
        return client(rpc::call<int, int(int, int)> (2, i,j));
    }
    boost::future<void> inc_inplace(int &i)
    {
        return client(rpc::call<int, void(int &)> (3, i));
    }
private:
    rpc::client<Registry> &client;
};

    // make calls using the interface class...
    remote_interface<rpc::registry<int> > remote(client);

    remote.nothing();

    BOOST_CHECK_EQUAL(remote.inc(5), 6);
    BOOST_CHECK_EQUAL(remote.add2(10, 20), 30);
    int x=20;
    remote.inc_inplace(x);
    BOOST_CHECK_EQUAL(x, 21);

