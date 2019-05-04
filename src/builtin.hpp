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

inline object_t builtin_plus(const object_t& cons, env_t& env)
{
    std::int64_t val = 0;
    for(auto&& obj : make_list(std::get<cell_t>(cons.data)))
    {
        val += std::get<std::int64_t>(eval(obj, env).data);
    }
    return object_t(val);
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

inline object_t builtin_let(const object_t& cons, env_t& env)
{
    // (let <symbol> <expr>)

    const symbol_t& name = std::get<symbol_t>(car(cons).data);
    const object_t& expr = car(cdr(cons));

    env[name] = eval(expr, env);
    return car(cons);
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
