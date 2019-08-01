#pragma once

#include <type_traits>
#include <utility>
#include <array>
#include <tuple>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <initializer_list>

///////////////////////////////////////////////////////////////////////////////
// Tools
///////////////////////////////////////////////////////////////////////////////

namespace {
    template <typename Visitor, typename Args, std::size_t... I>
    auto dispatch_visitor_impl(Visitor && visitor, Args && args, std::index_sequence<I...>)
    {
        return std::tuple<decltype(std::get<I>(args)(visitor))...>{ std::get<I>(args)(visitor) ... };
    }
}

/// Dispatch visitor on operands
template <typename Visitor, typename Args>
auto dispatch_visitor(Visitor && visitor, Args && args)
{
    return dispatch_visitor_impl(
        std::forward<Visitor>(visitor),
        std::forward<Args>(args),
        std::make_index_sequence<std::tuple_size<typename std::decay<Args>::type>::value>{}
    );
}

/// Hold operands in a appropriate way (copy or ref) in a tuple
template <typename... Args>
auto hold_args(Args &&... args)
{
   return std::tuple<Args...>{ std::forward<Args>(args)... };
}

/// Calculate array size from its shape
template <typename... Sizes>
std::size_t shape_to_size(Sizes const&... sizes)
{
    return (sizes * ... * 1);
}

namespace {
    template <typename T, std::size_t N, std::size_t... I>
    std::size_t shape_to_size_impl(std::array<T, N> const& shape, std::index_sequence<I...>)
    {
        return shape_to_size(std::get<I>(shape)...);
    }
}

/// Calculate array size from its shape
template <typename T, std::size_t N>
std::size_t shape_to_size(std::array<T, N> const& shape)
{
    return shape_to_size_impl(shape, std::make_index_sequence<N>{});
}

/// Calculate the dimension of the result after applying broadcasting rules
template <std::size_t... N>
auto broadcast_dim(std::size_t i, std::array<std::size_t, N> const&... shapes)
{
    const std::size_t dim = std::max({ i < shapes.size() ? shapes[i] : 0 ... });
    assert( ((i >= shapes.size() || shapes[i] == dim || shapes[i] == 1) && ... && true) && "Invalid broadcast" );
    return dim;
}

namespace {
    template <std::size_t... N, std::size_t... I>
    auto broadcast_shape_impl(std::index_sequence<I...>, std::array<std::size_t, N> const&... shapes)
    {
        return std::array<std::size_t, sizeof...(I)>{ broadcast_dim(I, shapes...)... };
    }
}

/// Calculate the shape of the result after applying broadcasting rules
template <std::size_t... N>
auto broadcast_shape(std::array<std::size_t, N> const&... shapes)
{
    return broadcast_shape_impl(std::make_index_sequence<std::max({std::size_t(0), N...})>{}, shapes...);
}

namespace {
    template <typename Shapes, std::size_t... I>
    auto broadcast_shape_impl(Shapes const& shapes, std::index_sequence<I...>)
    {
        return broadcast_shape(std::get<I>(shapes)...);
    }
}

/// Calculate the shape of the result after applying broadcasting rules
template <typename... Shapes>
auto broadcast_shape(std::tuple<Shapes...> const& shapes)
{
    return broadcast_shape_impl(shapes, std::make_index_sequence<sizeof...(Shapes)>{});
}

/// Check if given shapes are equals
template <std::size_t N, std::size_t... NOther>
bool shape_equal(std::array<std::size_t, N> const& lhs, std::array<std::size_t, NOther> const&... rhs)
{
    if constexpr ((... && (N == NOther)))
        return (... && (lhs == rhs));
    else
        return false;
}

/// Calculate position in container from nD index with respect to the broadcasting rules
template <std::size_t NS, std::size_t NI>
std::size_t idx_to_pos(std::array<std::size_t, NS> const& shape, std::array<std::size_t, NI> const& idx)
{
    std::size_t pos = 0;
    // Should be unrolled
    for (std::size_t i = 0; i < std::min(NS, NI); ++i)
    {
        assert((shape[i] == 1 || idx[i] < shape[i]) && "Invalid index");
        pos = pos * shape[i] + std::min(shape[i]-1, idx[i]);
    }
    return pos;
}

namespace {
    /// Check that given container has a minimal size (when std::size is available)
    template <typename Container>
    auto check_array_size_impl(Container const& container, std::size_t expected_size, int)
        -> decltype(std::size(container), true)
    {
        return std::size(container) >= expected_size;
    }

    /// Check that given container has a minimal size (fallback)
    template <typename Container>
    bool check_array_size_impl(Container const&, std::size_t, double)
    {
        return true;
    }
}

/// Check that given container has a minimal size (if possible, returns true otherwise)
template <typename Container>
bool check_array_size(Container const& container, std::size_t expected_size)
{
    return check_array_size_impl(container, expected_size, int(0));
}

namespace {
    /// Try to resize a container to a given size (when std::size and .resize are available)
    template <typename Container>
    auto resize_array_impl(Container & container, std::size_t size, int)
        -> decltype(std::size(container), container.resize(size), void())
    {
        if (std::size(container) < size)
            container.resize(size);
    }

    /// Try to resize a container to a given size (fallback)
    template <typename Container>
    void resize_array_impl(Container &, std::size_t, double)
    {
    }
} // namespace

/// Try to resize a container to a given size (do nothing if not possible)
template <typename Container>
void resize_array(Container & container, std::size_t size)
{
    resize_array_impl(container, size, int(0));
}

///////////////////////////////////////////////////////////////////////////////
// Expression nodes
///////////////////////////////////////////////////////////////////////////////


/// Expression tag
struct expr_tag {};

/// Constant value tag
struct cst_tag : expr_tag {};

/// Constant value expression
template <typename T>
auto cst(T value)
{
    return [=] (auto && visitor) -> decltype(auto) {
        return std::forward<decltype(visitor)>(visitor)(cst_tag{}, value);
    };
}

/// Array tag
struct array_tag : expr_tag {};

/// Array expression from a given container (shape as array)
template <typename Container, std::size_t N>
auto array(Container && container, std::array<std::size_t, N> const& shape)
{
    resize_array(container, shape_to_size(shape)); // Try to resize the container if it is too small
    assert(check_array_size(container, shape_to_size(shape)) && "Container is too small for given shape");

    return [op = hold_args(std::forward<Container>(container)), shape]
           (auto && visitor) mutable -> decltype(auto) {
               return std::forward<decltype(visitor)>(visitor)(array_tag{}, std::get<0>(op), shape);
           };
}

/// Array expression from a given container (shape as arguments)
template <typename Container, typename... Sizes>
auto array(Container && container, Sizes const& ... sizes)
{
    if constexpr (sizeof...(Sizes) == 0)
        return array(std::forward<Container>(container), std::size(container));
    else
        return array(
            std::forward<Container>(container),
            std::array<std::size_t, sizeof...(Sizes)>{static_cast<std::size_t>(sizes)...}
        );
}

/// Array expression from a given initializer list (stored in a std::vector)
template <typename T, typename... Args>
auto array(std::initializer_list<T> data, Args &&... args)
{
    return array(std::vector<T>(data), std::forward<Args>(args)...);
}

/// Operations tag
struct op_tag : expr_tag {};

/// Binary operations expressions
#define BINARY_OP(name, tag)            \
struct tag : op_tag {};                 \
template <typename LHS, typename RHS>   \
auto name(LHS && lhs, RHS && rhs)       \
{                                       \
    return                              \
        [operands = hold_args(std::forward<LHS>(lhs), std::forward<RHS>(rhs))] \
        (auto && visitor) mutable -> decltype(auto) { \
            return std::forward<decltype(visitor)>(visitor)(tag{}, dispatch_visitor(visitor, operands));  \
        };                              \
}                                       \

BINARY_OP(plus,         plus_tag)
BINARY_OP(minus,        minus_tag)
BINARY_OP(multiplies,   multiplies_tag)
BINARY_OP(divides,      divides_tag)

///////////////////////////////////////////////////////////////////////////////
// Visitors
///////////////////////////////////////////////////////////////////////////////
namespace visitor
{

/// Get shape of the expression
struct shape
{
    /// Shape of a constant value
    template <typename T>
    auto operator() (cst_tag, T) const
    {
        return std::array<std::size_t, 0>{};
    }

    /// Shape of an array
    template <typename D, typename S>
    auto operator() (array_tag, D, S const& s) const
    {
        return s;
    }

    /// Shape of the result of a component-wise operation
    template <typename Args>
    auto operator() (op_tag, Args const& shapes) const
    {
        return broadcast_shape(shapes);
    }

    /// Calling extension of shape visitor
    template <typename Tag, typename... Args>
    auto operator() (Tag tag, Args &&... args) const
        -> decltype(visit(*this, tag, std::forward<Args>(args)...)) // So that SFINAE works here
    {
        return visit(*this, tag, std::forward<Args>(args)...);
    }
};

/// Evaluate expression at given index
template <std::size_t N>
struct evaluator
{
    std::array<std::size_t, N> idx; ///< Indexes

    /// Evaluation of a constant value
    template <typename T>
    auto operator() (cst_tag, T value ) const
    {
        return value;
    }

    /// Evaluation of an array
    template <typename D, typename S>
    auto operator() (array_tag, D & d, S const& s) const -> decltype(auto)
    {
        return d[idx_to_pos(s, idx)];
    }

    /// Evaluation of binary operations
#define EVALUATOR_BINARY_OP(tag, op) \
    template <typename Args> auto operator() (tag, Args const& args) { return std::get<0>(args) op std::get<1>(args); }

    EVALUATOR_BINARY_OP(plus_tag, +)
    EVALUATOR_BINARY_OP(minus_tag, -)
    EVALUATOR_BINARY_OP(multiplies_tag, *)
    EVALUATOR_BINARY_OP(divides_tag, /)

    // Calling extension of evaluator visitor
    template <typename Tag, typename... Args>
    auto operator() (Tag tag, Args &&... args) const
        -> decltype(visit(*this, tag, std::forward<Args>(args)...)) // So that SFINAE works here
    {
        return visit(*this, tag, std::forward<Args>(args)...);
    }
};

/// Deduction guide for evaluator.
template <typename... I>
evaluator(I const&... i) -> evaluator<sizeof...(I)>;

} // namespace visitor

///////////////////////////////////////////////////////////////////////////////
// Type traits
///////////////////////////////////////////////////////////////////////////////
namespace type_traits
{

/// Fake visitor used to check if something is an expression
struct expr_probe
{
    struct success {};

    template <typename Tag, typename... T, typename = std::enable_if_t<std::is_base_of<expr_tag, typename std::decay<Tag>::type>::value> >
    auto operator() (Tag &&, T...) { return success{}; }

    template <typename... T>    auto operator() (T...) { return false; }
};

/// true if the given argument is an expression
template <typename Expr, typename = void>
constexpr bool is_expr_v = false;

template <typename Expr>
constexpr bool is_expr_v<Expr, std::void_t<std::invoke_result_t<Expr, expr_probe>>> = std::is_same_v<expr_probe::success, std::decay_t<std::invoke_result_t<Expr, expr_probe>>>;

template <typename Expr>
constexpr auto is_expr(Expr &&)
{
    return is_expr_v<Expr>;
}

/// Dimension of an expression
template <typename Expr, typename = void>
constexpr std::size_t dim_v = 0;

template <typename Expr>
constexpr std::size_t dim_v<Expr, std::void_t<std::invoke_result_t<Expr, visitor::shape>>> = std::tuple_size<std::decay_t<std::invoke_result_t<Expr, visitor::shape>>>::value;

template <typename Expr>
constexpr std::size_t dim(Expr &&)
{
    return dim_v<Expr>;
}

/// Value type of an expression
template <typename Expr>
using value_t = std::decay_t<decltype(std::declval<Expr>()(visitor::evaluator<dim_v<Expr>>{}))>;

} // type_traits

///////////////////////////////////////////////////////////////////////////////
// Additional tools
///////////////////////////////////////////////////////////////////////////////

/// Transform constants using cst
template <typename Expr>
auto make_it_expr(Expr && expr) -> decltype(auto)
{
    if constexpr (type_traits::is_expr_v<Expr>)
        return std::forward<Expr>(expr);
    else
        return cst(std::forward<Expr>(expr));
}

namespace
{
    /// Apply a function component-wise to the given expressions using broadcasting rules (implementation)
    template <typename Function, typename Shape, typename Args, std::size_t... I, typename... Coords>
    void apply_impl(Function && function, Shape const& shape, Args && args, std::index_sequence<I...>, Coords... coords)
    {
        constexpr std::size_t dim = std::tuple_size_v<Shape> - sizeof...(Coords);
        if constexpr (dim == 0)
            std::forward<Function>(function)(std::get<I>(args)(visitor::evaluator{coords...})...);
        else
        {
            for (std::size_t i = 0; i < shape[sizeof...(Coords)]; ++i)
              apply_impl(function, shape, args, std::index_sequence<I...>{}, coords..., i);
        }
    }
}

/// Apply a function component-wise to the given expressions using broadcasting rules.
template <typename Function, typename... Args>
void apply(Function && function, Args &&... args)
{
    const auto shape = broadcast_shape(make_it_expr(args)(visitor::shape{})...);
    apply_impl(std::forward<Function>(function), shape, hold_args(make_it_expr(std::forward<Args>(args))...), std::make_index_sequence<sizeof...(Args)>{});
}

/// Assign an expression to an array
template <typename LHS, typename RHS>
void assign(LHS && lhs, RHS && rhs)
{
   assert(broadcast_shape(lhs(visitor::shape{}), make_it_expr(rhs)(visitor::shape{})) == lhs(visitor::shape{})
          && "Assigned expression must have same shape as the broadcasted shape of the two terms");
   apply([] (auto && l, auto && r) { std::forward<decltype(l)>(l) = std::forward<decltype(r)>(r); }, std::forward<LHS>(lhs), std::forward<RHS>(rhs));
}

/** Check array equality between expressions using broadcasting.
 *
 * @note Could also be implemented using reduction operations like all(is_equal(lhs, rhs...))
 */
template <typename LHS, typename... RHS>
bool array_equal_equiv(LHS && lhs, RHS &&... rhs)
{
    bool result = true;
    apply([&result] (auto && l, auto &&... r) { result &= (... && (std::forward<decltype(l)>(l) == std::forward<decltype(r)>(r))); }, std::forward<LHS>(lhs), std::forward<RHS>(rhs)...);
    return result;
}

/// Check that expressions have same shape and same values
template <typename LHS, typename... RHS>
bool array_equal(LHS && lhs, RHS &&... rhs)
{
    using visitor::shape;
    bool result = shape_equal(make_it_expr(lhs)(shape{}), make_it_expr(rhs)(shape{})...);
    return result && array_equal_equiv(std::forward<LHS>(lhs), std::forward<RHS>(rhs)...);
}

/// Array expression from a container initialized with another expression
template <typename Container, typename Expr, typename = std::enable_if_t<type_traits::is_expr_v<Expr>>>
auto array(Container && container, Expr && expr)
{
    auto A = array(std::forward<Container>(container), expr(visitor::shape{}));
    assign(A, std::forward<Expr>(expr));
    return A;
}

/// Array expression initialized with another expression
template <typename Expr, typename = std::enable_if_t<type_traits::is_expr_v<Expr>>>
auto array(Expr && expr)
{
    return array(
        std::vector<type_traits::value_t<Expr>>{},
        std::forward<Expr>(expr)
    );
}

/// Display of shape
template <std::size_t N>
void display(std::ostream & os, std::array<std::size_t, N> const& shape)
{
    os << "[";
    for (std::size_t i = 0; i < N; ++i)
        os << (i > 0 ? ", " : "") << shape[i];
    os << "]";
}

/// Display of an expression
template <typename Expr, typename... Coords, typename = std::enable_if_t<type_traits::is_expr_v<Expr>>>
void display(std::ostream & os, Expr && expr, Coords... coords)
{
    constexpr std::size_t dim = type_traits::dim_v<Expr> - sizeof...(Coords);
    [[maybe_unused]] auto const shape = expr(visitor::shape{});

    if constexpr (dim == 0)
        os << std::forward<Expr>(expr)(visitor::evaluator<sizeof...(Coords)>{coords...});
    else if constexpr (dim == 1)
    {
        os << std::string(2*sizeof...(Coords), ' ') << "[";
        for (std::size_t i = 0; i < shape[sizeof...(Coords)]; ++i)
            os << (i == 0 ? "" : " ") << expr(visitor::evaluator<sizeof...(Coords)+1>{coords..., i});
        os << "]";
    }
    else
    {
        os << std::string(2*sizeof...(Coords), ' ') << "[" << std::endl;
        for (std::size_t i = 0; i < shape[sizeof...(Coords)]; ++i)
        {
            display(os, expr, coords..., i);
            os << std::endl;
        }
        os << std::string(2*sizeof...(Coords), ' ') << "]";
    }
}

