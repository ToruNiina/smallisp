#ifndef SMALLISP_BUILTIN_HPP
#define SMALLISP_BUILTIN_HPP
#include "eval.hpp"
#include <iostream>

namespace sml
{

inline object_t builtin_car(const object_t& cons, env_t& env)
{
    if(not cons.is_cell())
    {
        return object_t(nil);
    }
    return eval(car(std::get<cell_t>(cons.data)), env);
}

inline object_t builtin_cdr(const object_t& cons, env_t& env)
{
    if(not cons.is_cell())
    {
        return object_t(nil);
    }
    return eval(cdr(std::get<cell_t>(cons.data)), env);
}

inline object_t builtin_eq(const object_t& cons, env_t& env)
{
    const auto first  = eval(car(cons), env);
    const auto second = eval(car(cdr(cons)), env);

    if(first.data == second.data)
    {
        return object_t(true_t{});
    }
    else
    {
        return object_t(nil);
    }
}

inline object_t builtin_lt(const object_t& cons, env_t& env)
{
    const auto first  = eval(car(cons), env);
    const auto second = eval(car(cdr(cons)), env);

    if(first.data < second.data)
    {
        return object_t(true_t{});
    }
    else
    {
        return object_t(nil);
    }
}

struct builtin_plus_impl
{
    builtin_plus_impl()  = default;
    ~builtin_plus_impl() = default;
    builtin_plus_impl(builtin_plus_impl const&) = default;
    builtin_plus_impl(builtin_plus_impl &&)     = default;
    builtin_plus_impl& operator=(builtin_plus_impl const&) = default;
    builtin_plus_impl& operator=(builtin_plus_impl &&)     = default;

    object_t operator()(const std::int64_t& lhs, const std::int64_t& rhs) const
    {
        return object_t(lhs + rhs);
    }
    object_t operator()(const std::string& lhs, const std::string& rhs) const
    {
        return object_t(lhs + rhs);
    }
    object_t operator()(const std::int64_t& lhs, const std::string& rhs) const
    {
        return object_t(std::to_string(lhs) + rhs);
    }
    object_t operator()(const std::string& lhs, const std::int64_t& rhs) const
    {
        return object_t(lhs + std::to_string(rhs));
    }
    template<typename T, typename U>
    object_t operator()(const T&, const U&) const
    {
        throw std::runtime_error("[error] type error in builtin_plus");
    }
};

inline object_t builtin_plus(const object_t& cons, env_t& env)
{
    auto list = make_list(std::get<cell_t>(cons.data));
    object_t retval(list.front());
    list.erase(list.begin());
    for(const auto& obj : list)
    {
        const auto evaled = eval(obj, env);
        retval = std::visit(builtin_plus_impl(),
                            retval.data, evaled.data);
    }
    return retval;
}

inline object_t builtin_minus(const object_t& cons, env_t& env)
{
    std::int64_t val = std::get<std::int64_t>(eval(car(cons), env).data);
    if(cdr(cons).is_nil())
    {
        return object_t(-val);
    }

    for(auto&& obj : make_list(std::get<cell_t>(cdr(cons).data)))
    {
        val -= std::get<std::int64_t>(eval(obj, env).data);
    }
    return object_t(val);
}

inline object_t builtin_mod(const object_t& cons, env_t& env)
{
    const auto& lhs = eval(car(cons), env);
    const auto& rhs = eval(car(cdr(cons)), env);

    if(lhs.is_int() && rhs.is_int())
    {
        return object_t(std::get<std::int64_t>(lhs.data) %
                        std::get<std::int64_t>(rhs.data));
    }
    throw std::runtime_error("[error] arguments of % must be integers");
}

inline object_t builtin_println(const object_t& cons, env_t& env)
{
    for(auto&& obj : make_list(std::get<cell_t>(cons.data)))
    {
        std::cout << eval(obj, env) << std::endl;
    }
    return object_t(nil);
}

inline object_t builtin_if(const object_t& cons, env_t& env)
{
    // (if (cond) (then) (else))
    const object_t& v_cond = eval(car(cons), env);
    const object_t& v_then = car(cdr(cons));
    const object_t& v_else = car(cdr(cdr(cons)));

    if(v_cond.is_nil())
    {
        return eval(v_else, env);
    }
    else // not nil
    {
        return eval(v_then, env);
    }
}

inline object_t builtin_while(const object_t& cons, env_t& env)
{
    // (while (cond) (body))
    const object_t& v_cond = car(cons);
    const object_t& v_body = car(cdr(cons));
    if(eval(v_cond, env).is_nil())
    {
        return object_t(nil);
    }
    while(true)
    {
        auto retval = eval(v_body, env);
        if(eval(v_cond, env).is_nil())
        {
            return retval;
        }
    }
}

inline object_t builtin_let(const object_t& cons, env_t& env)
{
    // (let <symbol> <expr>)

    const symbol_t& name = std::get<symbol_t>(car(cons).data);
    const object_t& expr = car(cdr(cons));

    env[name] = eval(expr, env);
    return eval(car(cons), env);
}

inline object_t builtin_define(const object_t& cons, env_t& env)
{
    const object_t& decl = car(cons);
    const object_t& body = car(cdr(cons));

    if(not cdr(cdr(cons)).is_nil())
    {
        throw std::runtime_error(
            "[error] (define (<name-symbol> <arg-symbol...>) (<expr>))");
    }

    func_t fn;
    fn.name = std::get<symbol_t>(car(decl).data).name;

    std::vector<symbol_t> args;
    for(auto&& syms : make_list(std::get<cell_t>(cdr(decl).data)))
    {
        args.push_back(std::get<symbol_t>(syms.data));
    }
    fn.args = std::move(args);
    fn.body = std::get<cell_t>(body.data);

    env[fn.name] = object_t(fn);

    return env.at(fn.name);
}

} // sml
#endif// SMALLISP_BUILTIN_HPP
