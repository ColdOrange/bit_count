#include <utility>

#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <time.h>
#include <string>
#include <vector>

class Timer
{
public:
    Timer() : start_(0) { }

    Timer(const Timer&) = delete;
    Timer& operator=(const Timer&) = delete;

    void Start()
    {
        start_ = clock();
    }

    double ElapsedMilliseconds() const
    {
        return static_cast<double>(clock() - start_) / CLOCKS_PER_SEC * 1000;
    }

private:
    clock_t start_;
};

class BenchmarkBase
{
public:
    BenchmarkBase(std::string name, std::string description)
        : name_(std::move(name)), description_(std::move(description)) { }

    BenchmarkBase(const BenchmarkBase&) = delete;
    BenchmarkBase& operator=(const BenchmarkBase&) = delete;

    const std::string& GetName() const
    {
        return name_;
    }

    const std::string& GetDescription() const
    {
        return description_;
    }

    virtual void Run() = 0;

private:
    std::string name_;
    std::string description_;
};

class Benchmarks
{
public:
    Benchmarks() = default;

    Benchmarks(const Benchmarks&) = delete;
    Benchmarks& operator=(const Benchmarks&) = delete;

    void AddBenchmark(BenchmarkBase* benchmark)
    {
        benchmarks_.push_back(benchmark);
    }

    void RunBenchmarks()
    {
        for (auto benchmark : benchmarks_)
        {
            Timer timer;
            timer.Start();
            benchmark->Run();

            auto elapsed = timer.ElapsedMilliseconds();
            auto& name = benchmark->GetName();
            auto& description = benchmark->GetDescription();

            if (description.empty()) {
                printf("[%s]: %.2f milliseconds.\n", name.c_str(), elapsed);               
            }
            else {
                printf("[%s](%s): %.2f milliseconds.\n",
                        name.c_str(), description.c_str(), elapsed);
            }
        }
    }

private:
    std::vector<BenchmarkBase*> benchmarks_;
};

extern Benchmarks benchmarks;

#define BENCHMARK(name, description)                               \
    struct Benchmark_##name : public BenchmarkBase                 \
    {                                                              \
        Benchmark_##name() : BenchmarkBase(#name, #description)    \
        {                                                          \
            benchmarks.AddBenchmark(this);                         \
        }                                                          \
                                                                   \
        virtual void Run();                                        \
    } benchmark_##name##_obj;                                      \
                                                                   \
    void Benchmark_##name::Run()

#endif // BENCHMARK_H
