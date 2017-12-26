# CONSTEXPR_LAMBDA
Simple macro to allow 'constexpr' generic no-capture lambdas in C++14

*{ Written by [Aaron McDaid](https://aaronmcdaid.github.io/) - aaron.mcdaid@gmail.com }*

In C++14, lambdas are very useful but they have some restrictions. They can't be used in certain contexts and they aren't very friendly with `constexpr`.
This code works in C++17, but not C++14 (`error: call to non-constexpr function 'main()::<lambda(int)>'`):

```
    constexpr int a =
            [](int x){return x*x;}  // a lambda
            (4)                     // call the lambda
            ;
    static_assert(a == 16 ,"");
```
With this library, you can write this in C++14:
```
    constexpr auto a =
            CONSTEXPR_LAMBDA(,x)( return x*x; ) // a macro created a constexpr pseudo-lambda
            (4);                                // call this pseudo-lambda
    static_assert(a == 16 ,"");
```

Within the first set of parentheses, you specify a 'capture type' (by &-reference, by value, or by && forwarding reference) as well as a name for each parameter.
This essentially creates a generic lambda, i.e. as if all parameters are `auto` or `auto &` or `auto &&`.
For example:
```
CONSTEXPR_LAMBDA(&&,a,&&,b)  ....// capture two args called 'a' and 'b', both of them by forwarding reference
CONSTEXPR_LAMBDA( &,x,  ,y)  ....// capture two args called 'x' and 'y', 'x' is by reference, 'y' by value

// the above are like:

    (auto && a , auto && b)
    (auto  & x , auto    y)
```

You can even use the last parameter to capture a pack, `CONSTEXPR_LAMBDA(...,pack)`, but I can't get it to work on MSVC (tested on gcc.godbolt.org).
You can also use `&...` and `&&...` to capture a pack of references or a pack of forwarding references.

Here is a more complicated demo showing the reference capture:
```
constexpr auto
foo()
{
    int A = 10;
    int B = 100;
    int C = 1000;
    int product = CONSTEXPR_LAMBDA(  &,a  ,  ,b  ,  ,c)
                    (
                        int product = a*b*c;
                        a=2; // 'a' was captured by reference
                        b=0; // 'b' and 'c' were captured by value,
                        c=0; // so these assignments don't realy do anything
                        return product;
                    )
                    (A,B,C);
    // 'product' should be 1000000 now
    // 'A' was captured by reference, and hence is now 2
    return A + B + C + product;
}
static_assert(foo() == 2 + 100 + 1000 + 1000000 ,"");
```

## How it works
To begin, you would write a new class with the appropriate call operator:
```
    struct x {
        constexpr auto
        operator() (int x)
        { return x * x; }
    };
```
But we want to write this inside another expression. First, we create a lambda which
return this 'type' indirectly via a pointer. This allows us to write our type anywhere
we want: (except in *unevaluated contexts*, for which you should consider my {crazy!} [cambda](https://github.com/aaronmcdaid/cambda) library)

```
    auto * ptr =
    [](){

        struct x {
            constexpr auto
            operator() (int x)
            { return x * x; }
        };
        return (x*)nullptr;

    } // define a lambda
    (); // call it, returning the nullptr pointer-to-x
```
But we want a generic function call operator. However, we can't write templates inside these local classes. i.e. this isn't allowed.
```
    auto * ptr =
    [](){

        struct x {
            template<typename T> // templates not allowed in local classes
            constexpr auto
            operator() (int x)
            { return x * x; }
        };
        return (x*)nullptr;

    } // define a lambda
    (); // call it, returning the nullptr pointer-to-x
```
... so we use the outer lambda to 'encode' the type of the parameter
```
    auto funny_lambda_returning_our_pseudo_lambda =
    [](auto outer_x){

        struct x {
            constexpr auto
            operator() (decltype(outer_x) x)
            { return x * x; }
        };
        return (x*)nullptr;

    };
```
Now, we can extract an instance of 'x' for the correct argument type:
```
    using x_int     = std::decay_t<decltype(*funny_lambda_returning_our_pseudo_lambda(std::declval<int>()))>;
    static_assert(std::is_same< int     , decltype(x_int{}(3)) >{} ,"");
    using x_double  = std::decay_t<decltype(*funny_lambda_returning_our_pseudo_lambda(std::declval<double>()))>;
    static_assert(std::is_same< double  , decltype(x_double{}(3)) >{} ,"");
// next, construct and call these two objects. The second one is
// just to confirm that it truly is the int-to-int version
    static_assert(2.25 == x_double{} (1.5) ,"");
    static_assert(1    == x_int   {} (1.5) ,"");
```
