#ifndef SMALLISP_BUILTIN_HPP
#define SMALLISP_BUILTIN_HPP
#include "eval.hpp"
#include <iostream>

namespace sml
{

inline std::vector<object_t> eval_list(const cell_t& cell, env_t& env)
{
    std::vector<object_t> vec;
    vec.push_back(eval(car(cell), env));

    object_t const* v_cdr = std::addressof(cdr(cell));
    while(not std::holds_alternative<nil_t>(v_cdr->data))
    {
        const auto& cons = std::get<cell_t>(v_cdr->data);
        vec.push_back(eval(car(cons), env));
        v_cdr = std::addressof(cdr(cons));
    }
    return vec;
}

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
    for(auto&& obj : eval_list(std::get<cell_t>(cons.data), env))
    {
        val += std::get<std::int64_t>(obj.data);
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

    for(auto&& obj : eval_list(std::get<cell_t>(cdr(cons).data), env))
    {
        val -= std::get<std::int64_t>(obj.data);
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
