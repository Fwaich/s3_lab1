#include <gtest/gtest.h>
#include "LazySequence.hpp"
#include "ArraySequence.hpp"
#include "ReadOnlyStream.hpp"

TEST(LazyReadOnlyStream, ReadAdvancesPosition)
{
    Array_Sequence<int> seq;
    seq.append(10);
    seq.append(20);
    seq.append(30);

    auto lazy = Lazy_Sequence<int>::create(seq);
    Lazy_Read_Only_Stream<int> stream(lazy);

    EXPECT_EQ(stream.get_position(), 0);

    EXPECT_EQ(stream.read(), 10);
    EXPECT_EQ(stream.get_position(), 1);

    EXPECT_EQ(stream.read(), 20);
    EXPECT_EQ(stream.get_position(), 2);
}

TEST(LazyReadOnlyStream, ResetResetsPosition)
{
    Array_Sequence<int> seq;
    seq.append(1);
    seq.append(2);

    auto lazy = Lazy_Sequence<int>::create(seq);
    Lazy_Read_Only_Stream<int> stream(lazy);

    stream.read();
    stream.read();

    EXPECT_EQ(stream.get_position(), 2);

    stream.reset();
    EXPECT_EQ(stream.get_position(), 0);

    EXPECT_EQ(stream.read(), 1);
}

TEST(LazyReadOnlyStream, SeekWithinMaterialized)
{
    Array_Sequence<int> seq;
    seq.append(5);
    seq.append(6);
    seq.append(7);

    auto lazy = Lazy_Sequence<int>::create(seq);
    Lazy_Read_Only_Stream<int> stream(lazy);

    stream.read();
    stream.read();

    EXPECT_TRUE(stream.seek(0));
    EXPECT_EQ(stream.get_position(), 0);
    EXPECT_EQ(stream.read(), 5);

    EXPECT_TRUE(stream.seek(1));
    EXPECT_EQ(stream.read(), 6);
}

TEST(LazyReadOnlyStream, SeekBeyondMaterializedThrows)
{
    Array_Sequence<int> seq;
    seq.append(1);
    seq.append(2);

    auto lazy = Lazy_Sequence<int>::create(seq);
    Lazy_Read_Only_Stream<int> stream(lazy);

    EXPECT_THROW(stream.seek(1), std::runtime_error);

    stream.read();

    EXPECT_THROW(stream.seek(2), std::runtime_error);
}

TEST(LazyReadOnlyStream, GetSizeReturnsMaterializedCount)
{
    Array_Sequence<int> seq;
    seq.append(3);
    seq.append(4);
    seq.append(5);

    auto lazy = Lazy_Sequence<int>::create(seq);
    Lazy_Read_Only_Stream<int> stream(lazy);

    EXPECT_EQ(stream.get_size(), 0);

    stream.read();
    EXPECT_EQ(stream.get_size(), 1);

    stream.read();
    EXPECT_EQ(stream.get_size(), 2);
}

TEST(LazyReadOnlyStream, DoesNotOverGenerate)
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

    auto lazy = Lazy_Sequence<int>::create(start, 2, rule);
    Lazy_Read_Only_Stream<int> stream(lazy);

    EXPECT_EQ(generated, 0);

    EXPECT_EQ(stream.read(), 0);
    EXPECT_EQ(generated, 0);

    EXPECT_EQ(stream.read(), 1);
    EXPECT_EQ(generated, 0);

    EXPECT_EQ(stream.read(), 1);
    EXPECT_EQ(generated, 1);

    EXPECT_EQ(stream.get_size(), 3);
}






