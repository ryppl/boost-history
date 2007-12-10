#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mut;
std::condition_variable cv;
enum {mains_turn, f1s_turn, f2s_turn};
int turn = mains_turn;
std::thread tmp;
typedef std::unique_lock<std::mutex> Lock;

void f2(std::thread& tf1)
{
    Lock lk(mut);
    std::thread f1 = move(tf1);
    while (turn != f2s_turn)
        cv.wait(lk);
    std::cout << "f2 executing\n";
    std::cout << "f2 sees f1::id as " << f1.get_id()
              << " which is the same value both main and f1 saw earlier\n";
    std::cout << "f2 sees std::this_thread::get_id() as "
              << std::this_thread::get_id()
              << " which is the same value main saw earlier\n";
    f1.detach();
    std::cout << "f1 detached by f2\n";
    turn = f1s_turn;
    std::cout << "f2 asking f1 to execute\n";
    cv.notify_all();
    while (turn != f2s_turn)
        cv.wait(lk);
    std::cout << "f2 executing\n";
    std::cout << "f2 sees f1::id as " << f1.get_id() << " because it has detached f1\n";
    std::cout << "f2 sees std::this_thread::get_id() as " << std::this_thread::get_id() << '\n';
    turn = mains_turn;
    std::cout << "f2 asking main to execute\n";
    cv.notify_one();
    std::cout << "f2 ending\n";
}

void f1()
{
    Lock lk(mut);
    while (turn != f1s_turn)
        cv.wait(lk);
    std::cout << "f1 executing\n";
    std::cout << "f1 sees std::this_thread::get_id() as "
              << std::this_thread::get_id()
              << " which is the same value main saw earlier\n";
    turn = mains_turn;
    std::cout << "f1 asking main to execute\n";
    cv.notify_all();
    while (turn != f1s_turn)
        cv.wait(lk);
    std::cout << "f1 executing\n";
    std::cout << "f1 sees std::this_thread::get_id() as "
              << std::this_thread::get_id()
              << " which is the same value both main and f2 saw earlier\n";
    turn = f2s_turn;
    std::cout << "f1 asking f2 to execute\n";
    cv.notify_all();
    std::cout << "f1 ending\n";
}

int main()
{
    std::cout << "main executing\n";
    std::thread tf1(f1);
    std::cout << "main sees f1::id as " << tf1.get_id() << '\n';
    Lock lk(mut);
    turn = f1s_turn;
    std::cout << "main asking f1 to execute\n";
    cv.notify_one();
    tmp = move(tf1);
    std::thread tf2(f2, std::ref(tmp));
    while (turn != mains_turn)
        cv.wait(lk);
    std::cout << "main executing\n";
    std::cout << "main sees f1::id as " << tf1.get_id() << " because it has moved f1 to the f2 thread\n";
    std::cout << "main sees f2::id as " << tf2.get_id() << '\n';
    tf2.detach();
    std::cout << "f2 detached by main\n";
    std::cout << "main sees f2::id as " << tf2.get_id() << " because it has detached f2\n";
    turn = f2s_turn;
    std::cout << "main asking f2 to execute\n";
    cv.notify_all();
    while (turn != mains_turn)
        cv.wait(lk);
    std::cout << "main executing\n";
    std::cout << "main ending\n";
}
