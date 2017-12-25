#include <type_traits>
#include <utility>

namespace CONSTEXPR_LAMBDA_namespace
{
    template<typename L>
    auto
    null_address_of(L &&)
    ->std::decay_t<L>*
    {
        return nullptr;
    }

    template<typename L>
    struct call_forwarder
    {
        template<typename ... T>
        constexpr auto
        operator() (T && ... t)
        ->decltype(auto)
        {
            using X = std::decay_t<decltype( *std::declval<L&>()(std::forward<T>(t)...) )>;
            // X is the nested 'CONSTEXPR_LAMBDA_arbitrary_hidden_struct_namelkdsjflkafdlksafdja' type
            return X{} (std::forward<T>(t)...);
        }

    };

    template<typename L>
    auto constexpr
    make_CONSTEXPR_LAMBDA(L*)
    -> call_forwarder<L>
    { return {}; }
}


#define CONSTEXPR_LAMBDA(...) CONSTEXPR_LAMBDA_select_based_on_arg_count(COUNT_MACRO_ARGS(__VA_ARGS__))(__VA_ARGS__)
#define COUNT_MACRO_ARGS(...) GET_ARG_11( __VA_ARGS__, 10,9,8,7,6,5,4,3,2,1,I_CANNOT_SEE_ZERO_ARGS)
#define GET_ARG_11(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,N,...) N
#define CONSTEXPR_LAMBDA_select_based_on_arg_count(n) CONSTEXPR_LAMBDA_select_based_on_arg_count_impl(n)
#define CONSTEXPR_LAMBDA_select_based_on_arg_count_impl(n) CONSTEXPR_LAMBDA_with_this_many_args_ ## n

#define CONSTEXPR_LAMBDA_with_this_many_args_2(REF_OF_ARG0, NAME_OF_ARG0)                   \
            CONSTEXPR_LAMBDA_start_of_the_macro_stuff                                       \
                        (   auto                    REF_OF_ARG0 NAME_OF_ARG0                \
                        )   {                                                               \
            CONSTEXPR_LAMBDA_start_the_nested_class                                         \
            operator()  (   decltype(NAME_OF_ARG0)  REF_OF_ARG0 NAME_OF_ARG0                \
                        )                                                                   \
            ->decltype(auto)                                                                \
            {   CONSTEXPR_LAMBDA_BODY_OF_THE_FUNCTION

#define CONSTEXPR_LAMBDA_with_this_many_args_4(REF_OF_ARG0, NAME_OF_ARG0, REF_OF_ARG1, NAME_OF_ARG1)  \
            CONSTEXPR_LAMBDA_start_of_the_macro_stuff                                       \
                    (   auto                            REF_OF_ARG0 NAME_OF_ARG0            \
                    ,   auto                            REF_OF_ARG1 NAME_OF_ARG1            \
                    ) {     \
            CONSTEXPR_LAMBDA_start_the_nested_class                                         \
            operator()  (   decltype(NAME_OF_ARG0)      REF_OF_ARG0 NAME_OF_ARG0            \
                        ,   decltype(NAME_OF_ARG1)      REF_OF_ARG1 NAME_OF_ARG1            \
                        )                                                                   \
            ->decltype(auto)                                                                \
            {   CONSTEXPR_LAMBDA_BODY_OF_THE_FUNCTION


#define CONSTEXPR_LAMBDA_start_of_the_macro_stuff                                           \
            CONSTEXPR_LAMBDA_namespace::make_CONSTEXPR_LAMBDA(true ? nullptr :              \
            CONSTEXPR_LAMBDA_namespace::null_address_of([]

#define CONSTEXPR_LAMBDA_start_the_nested_class                                             \
        struct CONSTEXPR_LAMBDA_arbitrary_hidden_struct_namelkdsjflkafdlksafdja             \
        {   constexpr auto/*start the declaration of the call operator*/

#define CONSTEXPR_LAMBDA_BODY_OF_THE_FUNCTION(...)                                                                          \
                __VA_ARGS__                                                                 \
            }/*end of the call operator in the nested struct*/                              \
        };                                                                                  \
        return (CONSTEXPR_LAMBDA_arbitrary_hidden_struct_namelkdsjflkafdlksafdja*) nullptr; \
    })) /* { closes the lambda, then one parenthesis for 'null_address_of', then one parenthesis for 'make_CONSTEXPR_LAMBDA' */

