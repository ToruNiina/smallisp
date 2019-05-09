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

inline bool operator==(const nil_t&, const nil_t&) noexcept {return true;}
inline bool operator!=(const nil_t&, const nil_t&) noexcept {return false;}
inline bool operator< (const nil_t&, const nil_t&) noexcept {return false;}
inline bool operator<=(const nil_t&, const nil_t&) noexcept {return true;}
inline bool operator> (const nil_t&, const nil_t&) noexcept {return false;}
inline bool operator>=(const nil_t&, const nil_t&) noexcept {return true;}

struct true_t {};

template<typename charT, typename traits>
std::basic_ostream<charT, traits>&
operator<<(std::basic_ostream<charT, traits>& os, const true_t&)
{
    os << 'T';
    return os;
}

inline bool operator==(const true_t&, const true_t&) noexcept {return true;}
inline bool operator!=(const true_t&, const true_t&) noexcept {return false;}
inline bool operator< (const true_t&, const true_t&) noexcept {return false;}
inline bool operator<=(const true_t&, const true_t&) noexcept {return true;}
inline bool operator> (const true_t&, const true_t&) noexcept {return false;}
inline bool operator>=(const true_t&, const true_t&) noexcept {return true;}

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

inline bool operator==(const cell_t& lhs, const cell_t& rhs) noexcept {return lhs.objs == rhs.objs;}
inline bool operator!=(const cell_t& lhs, const cell_t& rhs) noexcept {return lhs.objs != rhs.objs;}
inline bool operator< (const cell_t& lhs, const cell_t& rhs) noexcept {return lhs.objs <  rhs.objs;}
inline bool operator<=(const cell_t& lhs, const cell_t& rhs) noexcept {return lhs.objs <= rhs.objs;}
inline bool operator> (const cell_t& lhs, const cell_t& rhs) noexcept {return lhs.objs >  rhs.objs;}
inline bool operator>=(const cell_t& lhs, const cell_t& rhs) noexcept {return lhs.objs >= rhs.objs;}

struct array_t
{
    array_t() = default;
    ~array_t() = default;
    array_t(const array_t&) = default;
    array_t(array_t&&)      = default;
    array_t& operator=(const array_t&) = default;
    array_t& operator=(array_t&&)      = default;

    void resize(std::size_t n) {objs.resize(n);}
    std::size_t size() const noexcept {return objs.size();}

    void push_back(const object_t& obj) {objs.push_back(obj);}
    void push_back(object_t&&      obj) {objs.push_back(std::move(obj));}

    object_t&       operator[](std::size_t i)       noexcept {return objs[i];}
    object_t const& operator[](std::size_t i) const noexcept {return objs[i];}
    object_t&       at(std::size_t i)       noexcept {return objs[i];}
    object_t const& at(std::size_t i) const noexcept {return objs[i];}

    object_t&       front()       noexcept {return objs.front();}
    object_t const& front() const noexcept {return objs.front();}
    object_t&       back()        noexcept {return objs.back();}
    object_t const& back()  const noexcept {return objs.back();}

    auto begin()        noexcept {return objs.begin();}
    auto end()          noexcept {return objs.end();}
    auto begin()  const noexcept {return objs.begin();}
    auto end()    const noexcept {return objs.end();}
    auto cbegin() const noexcept {return objs.cbegin();}
    auto cend()   const noexcept {return objs.cend();}

    auto rbegin()        noexcept {return objs.rbegin();}
    auto rend()          noexcept {return objs.rend();}
    auto rbegin()  const noexcept {return objs.rbegin();}
    auto rend()    const noexcept {return objs.rend();}
    auto crbegin() const noexcept {return objs.crbegin();}
    auto crend()   const noexcept {return objs.crend();}

    std::vector<object_t> objs;
};

template<typename charT, typename traits>
std::basic_ostream<charT, traits>&
operator<<(std::basic_ostream<charT, traits>& os, const array_t& array)
{
    os << "( ";
    for(const auto& item : array)
    {
        os << item << ' ';
    }
    os << ')';
    return os;
}

inline bool operator==(const array_t& lhs, const array_t& rhs) noexcept {return lhs.objs == rhs.objs;}
inline bool operator!=(const array_t& lhs, const array_t& rhs) noexcept {return lhs.objs != rhs.objs;}
inline bool operator< (const array_t& lhs, const array_t& rhs) noexcept {return lhs.objs <  rhs.objs;}
inline bool operator<=(const array_t& lhs, const array_t& rhs) noexcept {return lhs.objs <= rhs.objs;}
inline bool operator> (const array_t& lhs, const array_t& rhs) noexcept {return lhs.objs >  rhs.objs;}
inline bool operator>=(const array_t& lhs, const array_t& rhs) noexcept {return lhs.objs >= rhs.objs;}

struct builtin_t
{
    template<typename Fn>
    builtin_t(std::string nm, Fn&& f)
        : name(std::move(nm)), fn(std::forward<Fn>(f))
    {}
    builtin_t()  = default;
    ~builtin_t() = default;
    builtin_t(builtin_t const&) = default;
    builtin_t(builtin_t&&)      = default;
    builtin_t& operator=(builtin_t const&) = default;
    builtin_t& operator=(builtin_t&&)      = default;

    std::string name;
    std::function<object_t(const object_t&, env_t&)> fn;
};

template<typename charT, typename traits>
std::basic_ostream<charT, traits>&
operator<<(std::basic_ostream<charT, traits>& os, const builtin_t& func)
{
    os << func.name;
    return os;
}

inline bool operator==(const builtin_t& lhs, const builtin_t& rhs) noexcept {return lhs.name == rhs.name;}
inline bool operator!=(const builtin_t& lhs, const builtin_t& rhs) noexcept {return lhs.name != rhs.name;}
inline bool operator< (const builtin_t& lhs, const builtin_t& rhs) noexcept {return lhs.name <  rhs.name;}
inline bool operator<=(const builtin_t& lhs, const builtin_t& rhs) noexcept {return lhs.name <= rhs.name;}
inline bool operator> (const builtin_t& lhs, const builtin_t& rhs) noexcept {return lhs.name >  rhs.name;}
inline bool operator>=(const builtin_t& lhs, const builtin_t& rhs) noexcept {return lhs.name >= rhs.name;}

struct func_t
{
    func_t()  = default;
    ~func_t() = default;
    func_t(func_t const&) = default;
    func_t(func_t&&)      = default;
    func_t& operator=(func_t const&) = default;
    func_t& operator=(func_t&&)      = default;

    std::string           name;
    std::vector<symbol_t> args;
    cell_t                body;
};

template<typename charT, typename traits>
std::basic_ostream<charT, traits>&
operator<<(std::basic_ostream<charT, traits>& os, const func_t& func)
{
    os << '(' << func.name;
    for(const auto& arg : func.args)
    {
        os << ' ' << arg.name;
    }
    os << ')';
    return os;
}

inline bool operator==(const func_t& lhs, const func_t& rhs) noexcept {return lhs.name == rhs.name;}
inline bool operator!=(const func_t& lhs, const func_t& rhs) noexcept {return lhs.name != rhs.name;}
inline bool operator< (const func_t& lhs, const func_t& rhs) noexcept {return lhs.name <  rhs.name;}
inline bool operator<=(const func_t& lhs, const func_t& rhs) noexcept {return lhs.name <= rhs.name;}
inline bool operator> (const func_t& lhs, const func_t& rhs) noexcept {return lhs.name >  rhs.name;}
inline bool operator>=(const func_t& lhs, const func_t& rhs) noexcept {return lhs.name >= rhs.name;}

struct object_t
{
    object_t() noexcept: data(nil) {}
    ~object_t() = default;
    object_t(object_t const&) = default;
    object_t(object_t&&)      = default;
    object_t& operator=(object_t const&) = default;
    object_t& operator=(object_t&&)      = default;

    object_t(nil_t        v): data(v) {}
    object_t(true_t       v): data(v) {}
    object_t(std::int64_t v): data(v) {}
    object_t(std::string  v): data(std::move(v)) {}
    object_t(symbol_t     v): data(std::move(v)) {}
    object_t(cell_t       v): data(std::move(v)) {}
    object_t(func_t       v): data(std::move(v)) {}
    object_t(builtin_t    v): data(std::move(v)) {}

    bool is_nil()     const noexcept {return std::holds_alternative<nil_t       >(data);}
    bool is_T()       const noexcept {return std::holds_alternative<true_t      >(data);}
    bool is_int()     const noexcept {return std::holds_alternative<std::int64_t>(data);}
    bool is_string()  const noexcept {return std::holds_alternative<std::string >(data);}
    bool is_symbol()  const noexcept {return std::holds_alternative<symbol_t    >(data);}
    bool is_cell()    const noexcept {return std::holds_alternative<cell_t      >(data);}
    bool is_func()    const noexcept {return std::holds_alternative<func_t      >(data);}
    bool is_builtin() const noexcept {return std::holds_alternative<builtin_t   >(data);}

    std::variant<nil_t, true_t, std::int64_t, std::string, symbol_t, cell_t, array_t, func_t, builtin_t>
        data;
};

inline object_t const& car(object_t const& cell) noexcept {return car(std::get<cell_t>(cell.data));}
inline object_t&       car(object_t&       cell) noexcept {return car(std::get<cell_t>(cell.data));}
inline object_t const& cdr(object_t const& cell) noexcept {return cdr(std::get<cell_t>(cell.data));}
inline object_t&       cdr(object_t&       cell) noexcept {return cdr(std::get<cell_t>(cell.data));}

inline bool operator==(const object_t& lhs, const object_t& rhs) noexcept {return lhs.data == rhs.data;}
inline bool operator!=(const object_t& lhs, const object_t& rhs) noexcept {return lhs.data != rhs.data;}
inline bool operator< (const object_t& lhs, const object_t& rhs) noexcept {return lhs.data <  rhs.data;}
inline bool operator<=(const object_t& lhs, const object_t& rhs) noexcept {return lhs.data <= rhs.data;}
inline bool operator> (const object_t& lhs, const object_t& rhs) noexcept {return lhs.data >  rhs.data;}
inline bool operator>=(const object_t& lhs, const object_t& rhs) noexcept {return lhs.data >= rhs.data;}

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
