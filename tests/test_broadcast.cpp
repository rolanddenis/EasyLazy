#include <ostream>
#include <tuple>

template <typename... T>
std::ostream & operator<< (std::ostream & os, std::tuple<T...> const& v);

#include "EasyLazy/core.hpp"
#include "EasyLazy/broadcast.hpp"
#include "EasyLazy/map.hpp"
#include "EasyLazy/fancy_op.hpp"

#include "tools.hpp"

template <std::size_t N = 0, typename Tuple>
void display_tuple(std::ostream & os, Tuple const& v)
{
    if constexpr (N == std::tuple_size<Tuple>{})
        return;
    else
    {
        os << (N > 0 ? "," : "") << std::get<N>(v);
        display_tuple<N + 1>(os, v);
    }
}

template <typename... T>
std::ostream & operator<< (std::ostream & os, std::tuple<T...> const& v)
{
    os << "(";
    display_tuple(os, v);
    os << ")";
    return os;
}

int main()
{
    {
        auto a = 2;
        auto r = broadcast_to(a, 2, 3);
        auto r_ref = array({a, a, a, a, a, a}, 2, 3);
        std::cout << "r = " << r << std::endl;
        CHECK( array_equal(r, r_ref) );
    }

    {
        auto a = array({1, 2});
        auto r = broadcast_to(a, 2, 3);
        auto r_ref = array({1, 1, 1, 2, 2, 2}, 2, 3);
        std::cout << "r = " << r << std::endl;
        CHECK( array_equal(r, r_ref) );
    }

    {
        auto r = broadcast_to(array({1, 2}), 2, 3);
        auto r_ref = array({1, 1, 1, 2, 2, 2}, 2, 3);
        std::cout << "r = " << r << std::endl;
        CHECK( array_equal(r, r_ref) );
    }

    {
        auto r = broadcast_to(1., 2) + array({1, 2, 3}, 1, 3);
        auto r_ref = array({2, 3, 4, 2, 3, 4}, 2, 3);
        std::cout << "r = " << r << std::endl;
        CHECK( array_equal(r, r_ref) );
    }

    {
        auto r = broadcast(array({1, 2, 3}, 1, 3), array({2, 3}));
        using std::get;
        auto r_add = map([] (auto v) { return get<0>(v) + get<1>(v); }, r);
        auto r_ref = array({3, 4, 5, 4, 5, 6}, 2, 3);
        std::cout << "r = " << r << std::endl;
        std::cout << "r_add = " << r_add << std::endl;
        CHECK( array_equal(r_add, r_ref) );
    }

    {
        auto r_add = array({1, 2, 3}, 1, 3) + array({2, 3});
        auto r_ref = array({3, 4, 5, 4, 5, 6}, 2, 3);
        std::cout << "r_add = " << r_add << std::endl;
        CHECK( array_equal(r_add, r_ref) );
    }

    return return_code();
}
