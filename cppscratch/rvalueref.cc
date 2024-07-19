#include <iostream>
#include <utility>
#include <vector>
#include <functional>
#include <string>
#include <numeric>

static constexpr std::string_view kLevelPrefix[] = {
    "", "  ", "    ", "      ", "        ", "          ", "            "};

static int s_level = 0;

struct SomeKindOfBig
{
    SomeKindOfBig() : data(1024, 1) {
        std::cout << kLevelPrefix[s_level] << "SomeKindOfBig in DEFAULT constructor!" << std::endl;
    }

    SomeKindOfBig(const SomeKindOfBig &a) : data(a.data)
    {
        std::cout << kLevelPrefix[s_level] << "SomeKindOfBig in COPY constructor!" << std::endl;
    }
    SomeKindOfBig(SomeKindOfBig &&a) : data(std::move(a.data))
    {
        std::cout << kLevelPrefix[s_level] << "SomeKindOfBig data in MOVED constructor!" << std::endl;
    }

    // what if no const
    int operator()() const { return std::accumulate(data.begin(), data.end(), 0); }

    std::vector<int> data;
};

template <typename Func>
int sum_elements_by_value_func(Func f)
{
    std::cout << kLevelPrefix[s_level] << "Calling " << __FUNCTION__  << "(Func f) .... ";
    ++s_level;
    int result = f();
    std::cout << kLevelPrefix[s_level] << "return value=" << result << std::endl;
    --s_level;
    return result;
}

template <typename Func>
int sum_elements_by_const_ref(const Func &f)
{
    std::cout << kLevelPrefix[s_level] << "Calling " << __FUNCTION__ << "(Func& f) .... ";
    ++s_level;
    int result = f();
    std::cout << kLevelPrefix[s_level] << "return value=" << result << std::endl;;
    --s_level;
    return result;
}

template <typename Func>
int sum_elements_by_move_ref(Func &&f)
{
    std::cout << kLevelPrefix[s_level] << "Calling " << __FUNCTION__ << "(Func&& f) .... ";
    ++s_level;
    int result = f();
    std::cout << kLevelPrefix[s_level] << "return value=" << result << std::endl;
    --s_level;
    return result;
}

#define MarkFollowingRun(a) std::cout << std::endl << kLevelPrefix[s_level] << "Runing " << a << std::endl;

void FunctionUsage()
{
    std::cout << kLevelPrefix[s_level] << "Sizeof std::function<int()> = " << sizeof(std::function<int()>) << std::endl;
    MarkFollowingRun("std::function<int()> f = SomeKindOfBig()");
    std::function<int()> f = SomeKindOfBig();

    MarkFollowingRun("std::function<int()> v1(f), v2(f), v3(f) ...");
    std::function<int()> v1(f);
    std::function<int()> v2(f);
    std::function<int()> v3(f);

    MarkFollowingRun("std::function<int()> r(std::ref(f))");
    std::function<int()> r(std::ref(f));

    MarkFollowingRun("std::function<int()> m(std::move(f))");
    std::function<int()> m(std::move(f));

    ++s_level;

    MarkFollowingRun("Using sum_elements_by_value_func...");
    ++s_level;
    std::cout << std::endl << kLevelPrefix[s_level] << "Runing " << "sum_elements_by_value_func(v1)" << std::endl;
    sum_elements_by_value_func(v1);

    std::cout << std::endl << kLevelPrefix[s_level] << "Runing " << "sum_elements_by_value_func(std::ref(v1))" << std::endl;
    sum_elements_by_value_func(std::ref(v1));

    std::cout << std::endl << kLevelPrefix[s_level] << "Runing " << "sum_elements_by_value_func(std::move(v1))" << std::endl;
    sum_elements_by_value_func(std::move(v1));

    --s_level;
    MarkFollowingRun("Using sum_elements_by_const_ref...");
    ++s_level;
    std::cout << std::endl << kLevelPrefix[s_level] << "Runing " << "sum_elements_by_const_ref(v2)" << std::endl;
    sum_elements_by_const_ref(v2);

    std::cout << std::endl << kLevelPrefix[s_level] << "Runing " << "sum_elements_by_const_ref(std::ref(v2))" << std::endl;
    sum_elements_by_const_ref(std::ref(v2));

    std::cout << std::endl << kLevelPrefix[s_level] << "Runing " << "sum_elements_by_const_ref(std::move(v2))" << std::endl;
    sum_elements_by_const_ref(std::move(v2));

    --s_level;
    MarkFollowingRun("Using sum_elements_by_move_ref...");
    ++s_level;
    std::cout << std::endl << kLevelPrefix[s_level] << "Runing " << "sum_elements_by_move_ref(v3)" << std::endl;
    sum_elements_by_move_ref(v3);

    std::cout << std::endl << kLevelPrefix[s_level] << "Runing " << "sum_elements_by_move_ref(std::ref(v3))" << std::endl;
    sum_elements_by_move_ref(std::ref(v3));

    std::cout << std::endl << kLevelPrefix[s_level] << "Runing " << "sum_elements_by_move_ref(std::move(v3))" << std::endl;
    sum_elements_by_move_ref(std::move(v3));
    --s_level;

    --s_level;
}

int main()
{
    FunctionUsage();
    return 0;
}
