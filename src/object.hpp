#ifndef SMALLISP_OBJECT_HPP
#define SMALLISP_OBJECT_HPP
#include <utility>
#include <variant>
#include <vector>
#include <string>
#include <string_view>
#include <functional>
#include <map>

namespace sml
{

// forward decl
struct object_t;
struct env_t;

template<typename charT, typename traits>
std::basic_ostream<charT, traits>&
operator<<(std::basic_ostream<charT, traits>&, const object_t&);

struct nil_t {};
inline nil_t nil;

template<typename charT, typename traits>
std::basic_ostream<charT, traits>&
operator<<(std::basic_ostream<charT, traits>& os, const nil_t&)
{
    os << "nil";
    return os;
}

struct symbol_t
{
    explicit symbol_t(std::string      s ): name(std::move(s)) {}
    explicit symbol_t(std::string_view sv): name(sv) {}
    explicit symbol_t(const char*      sl): name(sl) {}
    symbol_t& operator=(std::string      s ){name = std::move(s); return *this;}
    symbol_t& operator=(std::string_view sv){name = sv;           return *this;}

    symbol_t()  = default;
    ~symbol_t() = default;
    symbol_t(const symbol_t&) = default;
    symbol_t(symbol_t&&)      = default;
    symbol_t& operator=(const symbol_t&) = default;
    symbol_t& operator=(symbol_t&&)      = default;

    std::string name;
};

inline bool operator==(const symbol_t& lhs, const symbol_t& rhs) noexcept {return lhs.name == rhs.name;}
inline bool operator!=(const symbol_t& lhs, const symbol_t& rhs) noexcept {return lhs.name != rhs.name;}
inline bool operator< (const symbol_t& lhs, const symbol_t& rhs) noexcept {return lhs.name <  rhs.name;}
inline bool operator<=(const symbol_t& lhs, const symbol_t& rhs) noexcept {return lhs.name <= rhs.name;}
inline bool operator> (const symbol_t& lhs, const symbol_t& rhs) noexcept {return lhs.name >  rhs.name;}
inline bool operator>=(const symbol_t& lhs, const symbol_t& rhs) noexcept {return lhs.name >= rhs.name;}

template<typename charT, typename traits>
std::basic_ostream<charT, traits>&
operator<<(std::basic_ostream<charT, traits>& os, const symbol_t& sym)
{
    os << sym.name;
    return os;
}

struct cell_t
{
    cell_t(): objs(2) {}
    ~cell_t() = default;
    cell_t(const cell_t&) = default;
    cell_t(cell_t&&)      = default;
    cell_t& operator=(const cell_t&) = default;
    cell_t& operator=(cell_t&&)      = default;

    std::vector<object_t> objs;
};

inline object_t const& car(cell_t const& cell) noexcept {return cell.objs.front();}
inline object_t&       car(cell_t&       cell) noexcept {return cell.objs.front();}
inline object_t const& cdr(cell_t const& cell) noexcept {return cell.objs.back();}
inline object_t&       cdr(cell_t&       cell) noexcept {return cell.objs.back();}

template<typename charT, typename traits>
std::basic_ostream<charT, traits>&
operator<<(std::basic_ostream<charT, traits>& os, const cell_t& cell)
{
    os << '(' << car(cell) << '.' << cdr(cell) << ')';
    return os;
}

struct func_t
{
    template<typename Fn>
    func_t(std::string nm, Fn&& f)
        : name(std::move(nm)), fn(std::forward<Fn>(f))
    {}
    func_t()  = default;
    ~func_t() = default;
    func_t(func_t const&) = default;
    func_t(func_t&&)      = default;
    func_t& operator=(func_t const&) = default;
    func_t& operator=(func_t&&)      = default;

    std::string name;
    std::function<object_t(const object_t&, env_t&)> fn;
};

template<typename charT, typename traits>
std::basic_ostream<charT, traits>&
operator<<(std::basic_ostream<charT, traits>& os, const func_t& func)
{
    os << func.name;
    return os;
}

struct object_t
{
    object_t() noexcept: data(nil) {}
    ~object_t() = default;
    object_t(object_t const&) = default;
    object_t(object_t&&)      = default;
    object_t& operator=(object_t const&) = default;
    object_t& operator=(object_t&&)      = default;

    object_t(nil_t        v): data(v) {}
    object_t(bool         v): data(v) {}
    object_t(std::int64_t v): data(v) {}
    object_t(symbol_t     v): data(v) {}
    object_t(cell_t       v): data(v) {}
    object_t(func_t       v): data(v) {}

    std::variant<nil_t, bool, std::int64_t, symbol_t, cell_t, func_t> data;
};

inline object_t const& car(object_t const& cell) noexcept {return car(std::get<cell_t>(cell.data));}
inline object_t&       car(object_t&       cell) noexcept {return car(std::get<cell_t>(cell.data));}
inline object_t const& cdr(object_t const& cell) noexcept {return cdr(std::get<cell_t>(cell.data));}
inline object_t&       cdr(object_t&       cell) noexcept {return cdr(std::get<cell_t>(cell.data));}

template<typename charT, typename traits>
std::basic_ostream<charT, traits>&
operator<<(std::basic_ostream<charT, traits>& os, const object_t& obj)
{
    std::visit([&os](const auto& val) -> void {os << val;}, obj.data);
    return os;
}

struct env_t
{
    env_t()  = default;
    ~env_t() = default;
    env_t(const env_t&) = default;
    env_t(env_t&&)      = default;
    env_t& operator=(const env_t&) = default;
    env_t& operator=(env_t&&)      = default;

    object_t&       operator[](std::string_view sv)       {return objs[symbol_t(sv)];}
    object_t const& operator[](std::string_view sv) const {return objs.at(symbol_t(sv));}
    object_t&       at(std::string_view sv)       {return objs[symbol_t(sv)];}
    object_t const& at(std::string_view sv) const {return objs.at(symbol_t(sv));}

    object_t&       operator[](const symbol_t& sym)       {return objs[sym];}
    object_t const& operator[](const symbol_t& sym) const {return objs.at(sym);}
    object_t&       at(const symbol_t& sym)       {return objs[sym];}
    object_t const& at(const symbol_t& sym) const {return objs.at(sym);}

    std::map<symbol_t, object_t> objs;
};

} // sml
#endif // SMALLISP_OBJECT_HPP
