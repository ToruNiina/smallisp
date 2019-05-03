#ifndef SMALLISP_BUILTIN_HPP
#define SMALLISP_BUILTIN_HPP
#include "eval.hpp"
#include <iostream>

namespace sml
{

inline object_t builtin_car(const object_t& cons, env_t& env)
{
    if(not std::holds_alternative<cell_t>(cons.data))
    {
        return object_t(nil);
    }
    return eval(car(std::get<cell_t>(cons.data)), env);
}

inline object_t builtin_cdr(const object_t& cons, env_t& env)
{
    if(not std::holds_alternative<cell_t>(cons.data))
    {
        return object_t(nil);
    }
    return eval(cdr(std::get<cell_t>(cons.data)), env);
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
    if(std::holds_alternative<nil_t>(cdr(cons).data))
    {
        return object_t(-val);
    }

    for(auto&& obj : make_list(std::get<cell_t>(cdr(cons).data)))
    {
        val -= std::get<std::int64_t>(eval(obj, env).data);
    }
    return object_t(val);
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
    // (define (<symbol> <args...>) (<expr>))
    return object_t(nil);
}

} // sml
#endif// SMALLISP_BUILTIN_HPP
