#ifndef SMALLISP_EVAL_HPP
#define SMALLISP_EVAL_HPP
#include "object.hpp"
#include <variant>

namespace sml
{

object_t eval(const object_t& obj, env_t& env);

struct evaluator
{
    evaluator(env_t& e): env(std::ref(e)) {}
    ~evaluator() = default;

    evaluator(evaluator const&) = default;
    evaluator(evaluator&)       = default;
    evaluator& operator=(evaluator const&) = default;
    evaluator& operator=(evaluator&)       = default;

    object_t operator()(const nil_t&)          {return object_t(nil);}
    object_t operator()(const bool& v)         {return object_t(v);}
    object_t operator()(const std::int64_t& v) {return object_t(v);}
    object_t operator()(const func_t& v)       {return object_t(v);}
    object_t operator()(const symbol_t& symbol)
    {
        if(env.get().objs.count(symbol) == 0)
        {
            return object_t(nil);
        }
        return env.get().objs.at(symbol);
    }
    object_t operator()(const cell_t& c)
    {
        auto front = eval(car(c), env.get()); // if it is a symbol, search that
        if(not std::holds_alternative<func_t>(front.data))
        {
            throw std::runtime_error(
                "[error] first value of list must be a func to be evaluated");
        }
        return std::get<func_t>(front.data).fn(cdr(c), env);
    }

    std::reference_wrapper<env_t> env;
};

object_t eval(const object_t& obj, env_t& env)
{
    return std::visit(evaluator(env), obj.data);
}

} // sml
#endif // SMALLISP_EVAL_HPP
