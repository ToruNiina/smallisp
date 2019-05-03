#include "eval.hpp"
#include "parser.hpp"
#include <iostream>

int main(int argc, char **argv)
{
    if(argc != 2)
    {
        std::cerr << "[error]: usage ./smallisp [script]" << std::endl;
        return 1;
    }

    sml::env_t env = sml::init_env();

    std::ifstream ifs(argv[1]);
    while(true)
    {
        sml::object_t expr = sml::read_expr(ifs);
        if(std::holds_alternative<sml::nil_t>(expr.data))
        {
            break;
        }
        std::cout << sml::eval(expr, env) << std::endl;
    }
    return 0;
}
