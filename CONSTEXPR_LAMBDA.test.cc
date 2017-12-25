#include "CONSTEXPR_LAMBDA.hh"
int main()
{
    auto l = CONSTEXPR_LAMBDA_namespace::make_CONSTEXPR_LAMBDA(true ? nullptr : CONSTEXPR_LAMBDA_namespace::null_address_of([](auto && arg0) {
        struct CONSTEXPR_LAMBDA_arbitrary_hidden_struct_namelkdsjflkafdlksafdja
        {
            constexpr auto
            operator() (decltype(arg0)&& arg0)
            ->decltype(auto)
            {
                return arg0*arg0;
            }
        };
        return (CONSTEXPR_LAMBDA_arbitrary_hidden_struct_namelkdsjflkafdlksafdja*) nullptr;
    }));

    static_assert(l(10) == 100 ,"");
    static_assert(l(0.5) == 0.25 ,"");

    //I can't get packs working on MSVC
    /*
    auto l2 = CONSTEXPR_LAMBDA_namespace::make_CONSTEXPR_LAMBDA(true ? nullptr : CONSTEXPR_LAMBDA_namespace::null_address_of([](auto && ... arg0) {
        struct CONSTEXPR_LAMBDA_arbitrary_hidden_struct_namelkdsjflkafdlksafdja
        {
            constexpr auto
            operator() (decltype(arg0)&& ... arg0)
            ->decltype(auto)
            {
                return sizeof...(arg0);
            }
        };
        return (CONSTEXPR_LAMBDA_arbitrary_hidden_struct_namelkdsjflkafdlksafdja*) nullptr;
    }));

    static_assert(l2(10) == 1 ,"");
    static_assert(l2(1,2,3) == 3 ,"");
    */

    {
        constexpr auto res3 = CONSTEXPR_LAMBDA_with_this_many_args_2(,m)( return m*m;)(3);
        static_assert(res3 == 9 ,"");

        constexpr auto res4 = CONSTEXPR_LAMBDA_with_this_many_args_4(,a,,b)( return a*b;)(21,3) ;
        static_assert(res4 == 63 ,"");

        constexpr auto res5 = CONSTEXPR_LAMBDA_with_this_many_args_2(,m)( return m*m;) (3);
        static_assert(res5 == 9 ,"");
    }
    {
        constexpr auto res3 = CONSTEXPR_LAMBDA(,m)( return m*m;)(3);
        static_assert(res3 == 9 ,"");

        constexpr auto res4 = CONSTEXPR_LAMBDA(,a,,b)( return a*b;)(21,3) ;
        static_assert(res4 == 63 ,"");

        constexpr auto res5 = CONSTEXPR_LAMBDA(,m)( return m*m;) (3);
        static_assert(res5 == 9 ,"");

        constexpr auto res0 = CONSTEXPR_LAMBDA()( return 42;) ();
        static_assert(res0 == 42 ,"");
    }
}
