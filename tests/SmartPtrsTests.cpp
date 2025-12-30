#include "gtest/gtest.h"
#include "UniquePtr.hpp"
#include "SharedPtr.hpp"
#include "WeakPtr.hpp"
#include <string>

struct TestStruct {
    int x;
    TestStruct(int val) : x(val) {}
    int get() const { return x; }
};


TEST(UniquePtrTest, ConstructorAndGet) {
    Unique_Ptr<TestStruct> p(new TestStruct(42));
    ASSERT_TRUE(p);
    EXPECT_EQ(p->get(), 42);
    EXPECT_EQ((*p).get(), 42);
}

TEST(UniquePtrTest, MoveConstructor) {
    Unique_Ptr<TestStruct> p1(new TestStruct(10));
    Unique_Ptr<TestStruct> p2(std::move(p1));

    EXPECT_TRUE(p2);
    EXPECT_EQ(p2->get(), 10);
    EXPECT_FALSE(p1);
}

TEST(UniquePtrTest, MoveAssignment) {
    Unique_Ptr<TestStruct> p1(new TestStruct(5));
    Unique_Ptr<TestStruct> p2;
    p2 = std::move(p1);

    EXPECT_TRUE(p2);
    EXPECT_EQ(p2->get(), 5);
    EXPECT_FALSE(p1);
}

TEST(UniquePtrTest, ReleaseAndReset) {
    Unique_Ptr<TestStruct> p(new TestStruct(7));
    TestStruct* raw = p.release();
    EXPECT_FALSE(p);
    EXPECT_EQ(raw->get(), 7);
    delete raw;

    p.reset(new TestStruct(20));
    EXPECT_TRUE(p);
    EXPECT_EQ(p->get(), 20);

    p.reset();
    EXPECT_FALSE(p);
}

TEST(UniquePtrTest, Swap) {
    Unique_Ptr<TestStruct> p1(new TestStruct(1));
    Unique_Ptr<TestStruct> p2(new TestStruct(2));

    p1.swap(p2);
    EXPECT_EQ(p1->get(), 2);
    EXPECT_EQ(p2->get(), 1);
}

TEST(UniquePtrTest, MakeUnique) {
    auto p = my::make_unique<TestStruct>(123);
    EXPECT_TRUE(p);
    EXPECT_EQ(p->get(), 123);
}



TEST(SharedPtrTest, ConstructorAndGet) {
    Shared_Ptr<TestStruct> p(new TestStruct(42));
    EXPECT_TRUE(p);
    EXPECT_EQ(p->get(), 42);
    EXPECT_EQ((*p).get(), 42);
    EXPECT_EQ(p.use_count(), 1);
    EXPECT_TRUE(p.unique());
}

TEST(SharedPtrTest, CopyConstructor) {
    Shared_Ptr<TestStruct> p1(new TestStruct(10));
    Shared_Ptr<TestStruct> p2(p1);

    EXPECT_EQ(p1.use_count(), 2);
    EXPECT_EQ(p2.use_count(), 2);
    EXPECT_FALSE(p1.unique());
    EXPECT_EQ(p2->get(), 10);
}

TEST(SharedPtrTest, MoveConstructor) {
    Shared_Ptr<TestStruct> p1(new TestStruct(5));
    Shared_Ptr<TestStruct> p2(std::move(p1));

    EXPECT_TRUE(p2);
    EXPECT_EQ(p2->get(), 5);
    EXPECT_FALSE(p1);
    EXPECT_EQ(p2.use_count(), 1);
}

TEST(SharedPtrTest, CopyAssignment) {
    Shared_Ptr<TestStruct> p1(new TestStruct(7));
    Shared_Ptr<TestStruct> p2;
    p2 = p1;

    EXPECT_EQ(p1.use_count(), 2);
    EXPECT_EQ(p2.use_count(), 2);
    EXPECT_EQ(p2->get(), 7);
}

TEST(SharedPtrTest, MoveAssignment) {
    Shared_Ptr<TestStruct> p1(new TestStruct(8));
    Shared_Ptr<TestStruct> p2;
    p2 = std::move(p1);

    EXPECT_TRUE(p2);
    EXPECT_EQ(p2->get(), 8);
    EXPECT_FALSE(p1);
    EXPECT_EQ(p2.use_count(), 1);
}

TEST(SharedPtrTest, ResetAndSwap) {
    Shared_Ptr<TestStruct> p1(new TestStruct(1));
    Shared_Ptr<TestStruct> p2(new TestStruct(2));

    p1.reset(new TestStruct(42));
    EXPECT_EQ(p1->get(), 42);

    p1.swap(p2);
    EXPECT_EQ(p1->get(), 2);
    EXPECT_EQ(p2->get(), 42);
}

TEST(WeakPtrTest, ConstructFromShared) {
    Shared_Ptr<TestStruct> sp(new TestStruct(42));
    Weak_Ptr<TestStruct> wp(sp);

    EXPECT_FALSE(wp.expired());
    EXPECT_EQ(wp.use_count(), 1);
}

TEST(WeakPtrTest, CopyConstructor) {
    Shared_Ptr<TestStruct> sp(new TestStruct(10));
    Weak_Ptr<TestStruct> wp1(sp);
    Weak_Ptr<TestStruct> wp2(wp1);

    EXPECT_FALSE(wp2.expired());
    EXPECT_EQ(wp1.use_count(), 1);
    EXPECT_EQ(wp2.use_count(), 1);
}

TEST(WeakPtrTest, MoveConstructor) {
    Shared_Ptr<TestStruct> sp(new TestStruct(5));
    Weak_Ptr<TestStruct> wp1(sp);
    Weak_Ptr<TestStruct> wp2(std::move(wp1));

    EXPECT_FALSE(wp2.expired());
    EXPECT_TRUE(wp1.expired() || !wp1.lock());
}

TEST(WeakPtrTest, LockShared) {
    auto sp = my::make_shared<TestStruct>(123);
    Weak_Ptr<TestStruct> wp(sp);

    auto sp2 = wp.lock();
    EXPECT_TRUE(sp2);           
    EXPECT_EQ(sp2->get(), 123);
    EXPECT_EQ(sp.use_count(), 2);
}

TEST(WeakPtrTest, ExpiredAfterSharedReset) {
    auto sp = my::make_shared<TestStruct>(99);
    Weak_Ptr<TestStruct> wp(sp);

    EXPECT_FALSE(wp.expired());
    sp.reset();

    EXPECT_TRUE(wp.expired());
    auto sp2 = wp.lock();
    EXPECT_FALSE(sp2);
}

TEST(WeakPtrTest, CopyAndMoveAssignment) {
    auto sp = my::make_shared<TestStruct>(77);
    Weak_Ptr<TestStruct> wp1(sp);
    Weak_Ptr<TestStruct> wp2;
    wp2 = wp1;

    EXPECT_FALSE(wp2.expired());
    EXPECT_EQ(wp2.use_count(), 1);

    Weak_Ptr<TestStruct> wp3;
    wp3 = std::move(wp2);
    EXPECT_FALSE(wp3.expired());
    EXPECT_TRUE(wp2.expired() || !wp2.lock());
}

TEST(WeakPtrTest, Reset) {
    auto sp = my::make_shared<TestStruct>(88);
    Weak_Ptr<TestStruct> wp(sp);

    wp.reset();
    EXPECT_TRUE(wp.expired());
    EXPECT_FALSE(wp.lock());
}
