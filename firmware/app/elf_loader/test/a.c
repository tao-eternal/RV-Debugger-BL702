int add(int a, int b)   //无重定位的最简单情况
{
    return a+b;
}

int fib(int n)          //需要重定位，实现约三条内部跳转的重定位码
{
    if(n<=1) return 1;
    else return fib(n-1) + fib(n-2);
}
