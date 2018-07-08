#include <iostream>
#include <optional>

struct DivisionResult {
    int quotient;
    int remainder;
};

struct Data {
    struct {
        int a;
        int b;
    } in;
    struct {
        int mul;
        std::optional<DivisionResult> div;
    } out;
};

std::optional<Data> parse_inputs(int argc, char** argv)
{
    if (argc != 3) {
        return {};
    }

    Data p;
    p.in.a = atoi(argv[1]);
    p.in.b = atoi(argv[2]);
    return p;
}

int multiply(const Data& p)
{
    return p.in.a * p.in.b;
}

bool is_zero(const Data& p)
{
    return p.in.b == 0;
}

std::optional<DivisionResult> divide(const Data& p)
{
    if (is_zero(p)) return {};
    else            return DivisionResult{p.in.a / p.in.b, p.in.a % p.in.b};
}

Data run_computation(Data p)
{
    p.out.mul = multiply(p);
    p.out.div = divide(p);
    return p;
}

bool all_successful(const Data& p)
{
    return bool(p.out.div);
}

int show_results(const Data& p)
{
    std::cout << "Inputs:" << std::endl;
    std::cout << "  a: " << p.in.a << std::endl;
    std::cout << "  b: " << p.in.b << std::endl;
    std::cout << "Outputs:" << std::endl;
    std::cout << "  mul: " << p.out.mul << std::endl;
    std::cout << "  div: " << std::endl;
    std::cout << "    quotient: " << p.out.div.value().quotient << std::endl;
    std::cout << "    remainder: " << p.out.div.value().remainder << std::endl;
    return all_successful(p) ? 0 : 1;
}

int main(int argc, char** argv)
{
    if (auto input = parse_inputs(argc, argv)) {
        auto output = run_computation(input.value());
        return show_results(output);
    } else {
        std::cout << "Usage: add <int:a> <int:b>" << std::endl;
        return 1;
    }
}
