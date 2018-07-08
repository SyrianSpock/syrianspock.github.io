#include <iostream>
#include <optional>

struct DivisionResult {
    int quotient;
    int remainder;
};

struct UserInput {
    int a;
    int b;
};

struct Output {
    int mul;
    std::optional<DivisionResult> div;
};

std::optional<UserInput> parse_inputs(int argc, char** argv)
{
    if (argc != 3) {
        return {};
    }

    UserInput in;
    in.a = atoi(argv[1]);
    in.b = atoi(argv[2]);
    return in;
}

int multiply(const int a, const int b)
{
    return a * b;
}

std::optional<DivisionResult> divide(const int a, const int b)
{
    if (b == 0) return {};
    else        return DivisionResult{a / b, a % b};
}

Output run_computation(const UserInput& input)
{
    Output out;
    out.mul = multiply(input.a, input.b);
    out.div = divide(input.a, input.b);
    return out;
}

bool all_successful(const Output& res)
{
    return bool(res.div);
}

void show_inputs(const UserInput& in)
{
    std::cout << "Inputs:" << std::endl;
    std::cout << "  a: " << in.a << std::endl;
    std::cout << "  b: " << in.b << std::endl;
}

void show_outputs(const Output& out)
{
    std::cout << "Outputs:" << std::endl;
    std::cout << "  mul: " << out.mul << std::endl;
    std::cout << "  div: " << std::endl;
    std::cout << "    quotient: " << out.div.value().quotient << std::endl;
    std::cout << "    remainder: " << out.div.value().remainder << std::endl;
}

int main(int argc, char** argv)
{
    if (auto input = parse_inputs(argc, argv)) {
        auto result = run_computation(input.value());
        show_inputs(input.value());
        show_outputs(result);
        return all_successful(result) ? 0 : 1;
    } else {
        std::cout << "Usage: add <int:a> <int:b>" << std::endl;
        return 1;
    }
}
