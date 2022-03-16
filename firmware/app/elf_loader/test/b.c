extern int add(int a, int b);   //无重定位的最简单情况


extern int fib(int n);          //需要重定位，实现约三条内部跳转的重定位码

int test_fib_add1(int n) {
    return add(1, fib(n));
}