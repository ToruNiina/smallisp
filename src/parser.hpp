#ifndef SMALLISP_PARSER_HPP
#define SMALLISP_PARSER_HPP
#include "object.hpp"
#include "eval.hpp"
#include "builtin.hpp"
#include <fstream>
#include <string>
#include <cassert>
#include <cctype>
#include <iostream>

namespace sml
{

inline env_t init_env()
{
    env_t env;
    env["nil"]     = object_t(nil_t{});
    env["T"]       = object_t(true_t{});
    env["+"]       = builtin_t("builtin_plus",    builtin_plus);
    env["-"]       = builtin_t("builtin_minus",   builtin_minus);
    env["car"]     = builtin_t("builtin_car",     builtin_car);
    env["cdr"]     = builtin_t("builtin_cdr",     builtin_cdr);
    env["let"]     = builtin_t("builtin_let",     builtin_let);
    env["define"]  = builtin_t("builtin_define",  builtin_define);
    env["println"] = builtin_t("builtin_println", builtin_println);
    return env;
}

template<typename charT, typename traits>
object_t read_number(std::basic_ifstream<charT, traits>& file, char sign)
{
    std::string token;
    while(not file.eof())
    {
        if(std::isdigit(file.peek()))
        {
            token += file.get();
        }
        else
        {
            break;
        }
    }
    if(sign == '-')
    {
        return object_t(-std::int64_t(std::stoll(token)));
    }
    return object_t(std::int64_t(std::stoll(token)));
}

template<typename charT, typename traits>
object_t read_symbol(std::basic_ifstream<charT, traits>& file)
{
    std::string token;
    while(not file.eof())
    {
        if(std::isalpha(file.peek()) || file.peek() == '-')
        {
            token += file.get();
        }
        else
        {
            break;
        }
    }
    return object_t(symbol_t(token));
}

template<typename charT, typename traits>
object_t read_expr(std::basic_ifstream<charT, traits>& file);

template<typename charT, typename traits>
object_t read_list(std::basic_ifstream<charT, traits>& file)
{
    assert(file.get() == '(');

    object_t list(cell_t{});
    car(std::get<cell_t>(list.data)) = read_expr(file);

    object_t* cons = std::addressof(cdr(std::get<cell_t>(list.data)));

    file.peek();
    while(not file.eof())
    {
        const char c = file.get();
        if(file.eof()) {return list;}

        if(c == ' ' || c == '\n' || c == '\t' || c == '\r')
        {
            continue;
        }
        if(c == ')')
        {
            return list;
        }
        file.unget();

        *cons = cell_t{};
        car(std::get<cell_t>(cons->data)) = read_expr(file);
        cons = std::addressof(cdr(std::get<cell_t>(cons->data)));
    }
    throw std::runtime_error("[error] list did not closed");
}

template<typename charT, typename traits>
object_t read_expr(std::basic_ifstream<charT, traits>& file)
{
    object_t expr(nil);
    std::string token;

    file.peek();
    while(not file.eof())
    {
        const char c = file.get();
        if(file.eof()) {break;}

        if(c == ' ' || c == '\n' || c == '\t' || c == '\r')
        {
            continue;
        }
        if(c == ';') // comment
        {
            while(not file.eof() && file.peek() != '\n')
            {
                file.ignore(1);
            }
            continue;
        }
        if(c == '+')
        {
            if(std::isdigit(file.peek()))
            {
                return read_number(file, '+');
            }
            else
            {
                return object_t(symbol_t("+"));
            }
        }
        if(c == '-')
        {
            if(std::isdigit(file.peek()))
            {
                return read_number(file, '-');
            }
            else
            {
                return object_t(symbol_t("-"));
            }
        }
        if(std::isdigit(c))
        {
            file.unget(); // put back `c` to file
            return read_number(file, '+');
        }
        if(std::isalpha(c))
        {
            file.unget(); // put back `c` to file
            return read_symbol(file);
        }
        if(c == '(')
        {
            file.unget();
            return read_list(file);
        }

        file.unget();
        file >> token;
        throw std::runtime_error(
                "[error] couldn't parse the next token -> " + token);
    }
    // EOF.
    return object_t(nil);
}


} // sml
#endif // SMALLISP_PARSER_HPP
