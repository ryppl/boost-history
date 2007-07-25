package boost;

public class A
{
    static {
        java.lang.System.out.println("stub/boost/A/static_init...");
        java.lang.System.loadLibrary("boost_A.dll");
    }
}
