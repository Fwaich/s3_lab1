#include <gtest/gtest.h>
#include "LazySequence.hpp"
#include "ArraySequence.hpp"

TEST(LazySequence, CreateFromSequence) {
    Array_Sequence<int> seq;
    seq.append(1);
    seq.append(2);
    seq.append(3);

    auto lazy = Lazy_Sequence<int>::create(seq);

    EXPECT_EQ(lazy->get(0), 1);
    EXPECT_EQ(lazy->get(1), 2);
    EXPECT_EQ(lazy->get(2), 3);
    EXPECT_EQ(lazy->get_materialized_count(), 3);
}

TEST(LazySequence, LazyMaterialization) {
    Array_Sequence<int> start;
    start.append(1);
    start.append(1);

    auto fib = [](const Array_Sequence<int>& s) {
        size_t n = s.get_size();
        return s.get(n - 1) + s.get(n - 2);
    };

    auto lazy = Lazy_Sequence<int>::create(start, 2, fib);

    EXPECT_EQ(lazy->get_materialized_count(), 2);

    EXPECT_EQ(lazy->get(2), 2);
    EXPECT_EQ(lazy->get_materialized_count(), 3);

    EXPECT_EQ(lazy->get(5), 8);
    EXPECT_EQ(lazy->get_materialized_count(), 6);
}

TEST(LazySequence, GetNextWorks) {
    Array_Sequence<int> start;
    start.append(1);
    start.append(2);

    auto gen = Lazy_Sequence<int>::create(start);

    EXPECT_EQ(gen->get_next(), 1);
    EXPECT_EQ(gen->get_next(), 2);
}

TEST(LazySequence, Append) {
    Array_Sequence<int> a;
    a.append(1);
    a.append(2);

    Array_Sequence<int> b;
    b.append(3);
    b.append(4);

    auto la = Lazy_Sequence<int>::create(a);
    auto lb = Lazy_Sequence<int>::create(b);

    auto combined = la->append(lb);

    EXPECT_EQ(combined->get(0), 1);
    EXPECT_EQ(combined->get(1), 2);
    EXPECT_EQ(combined->get(2), 3);
    EXPECT_EQ(combined->get(3), 4);
}

TEST(LazySequence, Prepend) {
    Array_Sequence<int> a;
    a.append(3);
    a.append(4);

    Array_Sequence<int> b;
    b.append(1);
    b.append(2);

    auto la = Lazy_Sequence<int>::create(a);
    auto lb = Lazy_Sequence<int>::create(b);

    auto combined = la->prepend(lb);

    EXPECT_EQ(combined->get(0), 1);
    EXPECT_EQ(combined->get(1), 2);
    EXPECT_EQ(combined->get(2), 3);
    EXPECT_EQ(combined->get(3), 4);
}

TEST(LazySequence, InsertAt) {
    Array_Sequence<int> base;
    base.append(0);
    base.append(1);

    auto fib = [](const Array_Sequence<int>& s) {
        size_t n = s.get_size();
        return s.get(n - 1) + s.get(n - 2);
    };

    Array_Sequence<int> mid;
    mid.append(2);
    mid.append(3);

    auto lb = Lazy_Sequence<int>::create(base);
    auto lm = Lazy_Sequence<int>::create(mid);

    auto result = lb->insert_at(2, lm);

    EXPECT_EQ(result->get(0), 0);
    EXPECT_EQ(result->get(1), 2);
    EXPECT_EQ(result->get(2), 3);
    EXPECT_EQ(result->get(3), 1);
}

TEST(LazySequence, Map) {
    Array_Sequence<int> seq;
    seq.append(1);
    seq.append(2);
    seq.append(3);

    auto lazy = Lazy_Sequence<int>::create(seq);

    auto mapped = lazy->map<int>([](int x) {
        return x * 2;
    });

    EXPECT_EQ(mapped->get(0), 2);
    EXPECT_EQ(mapped->get(1), 4);
    EXPECT_EQ(mapped->get(2), 6);
}

TEST(LazySequence, WhereFilter) {
    Array_Sequence<int> seq;
    for (int i = 0; i <= 10; ++i)
        seq.append(i);

    auto lazy = Lazy_Sequence<int>::create(seq);

    auto evens = lazy->where([](int x) {
        return x % 2 == 0;
    });

    EXPECT_EQ(evens->get(0), 0);
    EXPECT_EQ(evens->get(1), 2);
    EXPECT_EQ(evens->get(2), 4);
    EXPECT_EQ(evens->get(3), 6);
    EXPECT_EQ(evens->get(4), 8);
}

TEST(LazySequence, MapThenWhere) {
    Array_Sequence<int> seq;
    for (int i = 1; i <= 10; ++i)
        seq.append(i);

    auto lazy = Lazy_Sequence<int>::create(seq);

    auto result = lazy
        ->map<int>([](int x) { return x * x; })
        ->where([](int x) { return x % 2 == 0; });

    EXPECT_EQ(result->get(0), 4);
    EXPECT_EQ(result->get(1), 16);
    EXPECT_EQ(result->get(2), 36);
    EXPECT_EQ(result->get(3), 64);
    EXPECT_EQ(result->get(4), 100);
}

TEST(LazySequence, DoesNotOverGenerate)
{
    int generated = 0;

    Array_Sequence<int> start;
    start.append(0);
    start.append(1);

    auto rule = [&generated](const Array_Sequence<int>& seq) -> int {
        ++generated;
        size_t n = seq.get_size();
        return seq.get(n - 1) + seq.get(n - 2);
    };

    auto seq = Lazy_Sequence<int>::create(start, 2, rule);

    EXPECT_TRUE(seq->has_next());
    EXPECT_TRUE(seq->has_next());
    EXPECT_EQ(generated, 0);

    EXPECT_EQ(seq->get_next(), 1);
    EXPECT_EQ(generated, 1);

    EXPECT_EQ(seq->get(2), 1);
    EXPECT_EQ(generated, 1);

    EXPECT_TRUE(seq->has_next());
    EXPECT_EQ(generated, 1);

    EXPECT_EQ(seq->get_next(), 2);
    EXPECT_EQ(generated, 2);

    EXPECT_EQ(seq->get(3), 2);
    EXPECT_EQ(generated, 2);
}