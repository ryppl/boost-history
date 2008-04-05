//[listing2
//...
movable f(int x, int y)
{ return movable(x * y); }

int main()
{
    movable x = f(10, 5);
    movable y;
    y = f(4, 3);

    return 0;
}
//]
