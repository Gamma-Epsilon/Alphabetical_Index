// Include iterators first to also check their standalone dependencies
#include "../headers/Iterators/ListIterator.hpp"
#include "../headers/Iterators/ArrayIterator.hpp"
#include "../headers/Pointers/SharedPtr.hpp"

#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace {
void Check(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}

template <class Exception, class Action>
void ExpectThrow(Action action) {
    try { action(); }
    catch (const Exception&) { return; }
    throw std::runtime_error("Expected exception was not thrown");
}

void CheckValues(const ISequence<int>& sequence, const std::vector<int>& expected) {
    Check(sequence.GetLength() == static_cast<int>(expected.size()), "Length mismatch");
    auto iterator = sequence.GetIterator();
    for (int value : expected) {
        Check(iterator->HasNext(), "Iterator ended early");
        Check(iterator->Current() == value, "Value mismatch");
        iterator->Next();
    }
    Check(!iterator->HasNext(), "Iterator did not end");
    ExpectThrow<IndexOutOfRange>([&] { iterator->Current(); });
    ExpectThrow<IndexOutOfRange>([&] { iterator->Next(); });
    if (!expected.empty()) {
        Check(sequence.GetFirst() == expected.front(), "First mismatch");
        Check(sequence.GetLast() == expected.back(), "Last/tail mismatch");
    }
}

template <class Sequence>
void SequenceOperations() {
    Sequence sequence;
    ISequence<int>& base = sequence;
    CheckValues(base, {});
    ExpectThrow<IndexOutOfRange>([&] { base.GetFirst(); });
    ExpectThrow<IndexOutOfRange>([&] { base.GetLast(); });
    ExpectThrow<IndexOutOfRange>([&] { base[0]; });
    ExpectThrow<IndexOutOfRange>([&] { base.RemoveAt(0); });
    base.InsertAt(2, 0);
    CheckValues(base, {2});
    base.Prepend(1);
    base.Append(4);
    base.InsertAt(3, 2);
    base.InsertAt(5, 4);
    CheckValues(base, {1, 2, 3, 4, 5});
    base[2] = 30;
    const ISequence<int>& readOnly = base;
    Check(readOnly[2] == 30, "Const access mismatch");
    ExpectThrow<IndexOutOfRange>([&] { base[-1]; });
    ExpectThrow<IndexOutOfRange>([&] { readOnly[5]; });
    ExpectThrow<IndexOutOfRange>([&] { readOnly[-1]; });
    ExpectThrow<IndexOutOfRange>([&] { base.InsertAt(0, -1); });
    ExpectThrow<IndexOutOfRange>([&] { base.InsertAt(0, 6); });
    ExpectThrow<IndexOutOfRange>([&] { base.RemoveAt(5); });
    ExpectThrow<IndexOutOfRange>([&] { base.RemoveAt(-1); });
    base.RemoveAt(2);
    base.RemoveAt(0);
    base.RemoveAt(base.GetLength() - 1);
    CheckValues(base, {2, 4});
    base.RemoveAt(0);
    base.RemoveAt(0);
    CheckValues(base, {});
    base.Prepend(8);
    base.RemoveAt(0);
    base.Append(9);
    CheckValues(base, {9});
    for (int i = 0; i < 100; ++i) base.Append(i);
    Check(base.GetLast() == 99, "Many appends failed");
}

template <class Sequence>
void CopyAndMove() {
    const int items[] = {1, 2, 3};
    Sequence original(items, 3);
    Sequence copy(original);
    copy[0] = 9;
    Check(original[0] == 1, "Copy shares storage");
    Sequence assigned;
    assigned.Append(99);
    assigned = original;
    assigned.RemoveAt(0);
    CheckValues(original, {1, 2, 3});
    original = original;
    CheckValues(original, {1, 2, 3});
    const int* first = &original[0];
    Sequence moved(std::move(original));
    Check(&moved[0] == first, "Move constructor copied storage");
    CheckValues(original, {});
    original.Append(7);
    CheckValues(original, {7});
    assigned = std::move(moved);
    Check(&assigned[0] == first, "Move assignment copied storage");
    CheckValues(assigned, {1, 2, 3});
    CheckValues(moved, {});
    moved.Prepend(8);
    CheckValues(moved, {8});
    assigned = std::move(assigned);
    CheckValues(assigned, {1, 2, 3});
    Sequence empty;
    Sequence emptyCopy(empty);
    assigned = empty;
    CheckValues(assigned, {});
    CheckValues(emptyCopy, {});
    Sequence emptyMove(std::move(empty));
    assigned = std::move(emptyMove);
    assigned.Append(5);
    CheckValues(assigned, {5});
    Sequence nullEmpty(nullptr, 0);
    CheckValues(nullEmpty, {});
    ExpectThrow<IndexOutOfRange>([] { Sequence invalid(nullptr, -1); });
    ExpectThrow<std::invalid_argument>([] { Sequence invalid(nullptr, 1); });
}

void ArrayCapacityAndAliasing() {
    DynamicArray<int> array;
    Check(array.GetCapacity() == 0 && array.GetSize() == 0, "Default array not empty");
    array.Reserve(4);
    Check(array.GetLength() == 0 && array.GetCapacity() == 4, "Reserve changed size");
    array.Resize(2);
    CheckValues(array, {0, 0});
    array[0] = 7;
    const int* address = &array[0];
    array.Reserve(1);
    array.Reserve(4);
    Check(&array[0] == address && array.GetCapacity() == 4, "Reserve reallocated unnecessarily");
    array.Resize(6);
    CheckValues(array, {7, 0, 0, 0, 0, 0});
    array.Resize(1);
    Check(array.GetCapacity() == 6, "Shrink changed capacity");
    array.Resize(3);
    CheckValues(array, {7, 0, 0});
    array.Resize(0);
    CheckValues(array, {});
    ExpectThrow<IndexOutOfRange>([&] { array.Resize(-1); });
    ExpectThrow<IndexOutOfRange>([&] { array.Reserve(-1); });
    ExpectThrow<IndexOutOfRange>([] { DynamicArray<int> invalid(-1); });
    DynamicArray<int> initialized(3);
    CheckValues(initialized, {0, 0, 0});
    DynamicArray<std::string> words;
    words.Append("alpha");
    words.Append(words[0]); // Reallocation with an internal reference
    words.InsertAt(words[1], 0); // Reallocation and shifting
    words.Reserve(10);
    words[1] = "beta";
    words.InsertAt(words[1], 0); // Shifting without reallocation
    Check(words[0] == "beta" && words[1] == "alpha" &&
          words[2] == "beta" && words[3] == "alpha", "Aliased insert corrupted data");
    DynamicArray<int> growth;
    int allocations = 0;
    for (int i = 0; i < 4096; ++i) {
        const int capacity = growth.GetCapacity();
        growth.Append(i);
        if (capacity != growth.GetCapacity()) ++allocations;
    }
    Check(allocations <= 14 && growth.GetLast() == 4095, "Growth is not geometric");
}

// A resource-owning value: counts actual resources, not preallocated array slots
struct Resource {
    static int live;
    static int copiesBeforeThrow;
    UniquePtr<int> value;
    Resource() = default;
    explicit Resource(int v) : value(new int(v)) { ++live; }
    Resource(const Resource& other) {
        if (copiesBeforeThrow == 0) throw std::runtime_error("Simulated copy failure");
        if (copiesBeforeThrow > 0) --copiesBeforeThrow;
        if (other.value) { value.reset(new int(*other.value)); ++live; }
    }
    Resource(Resource&& other) noexcept : value(std::move(other.value)) {}
    Resource& operator=(const Resource& other) {
        if (this != &other) { Resource copy(other); value.swap(copy.value); }
        return *this;
    }
    Resource& operator=(Resource&& other) noexcept {
        if (this != &other) {
            if (value) --live;
            value = std::move(other.value);
        }
        return *this;
    }
    ~Resource() { if (value) --live; }
};
int Resource::live = 0;
int Resource::copiesBeforeThrow = -1;

template <class Sequence>
void FailedCopies() {
    {
        Sequence source;
        source.Append(Resource(1));
        source.Append(Resource(2));
        source.Append(Resource(3));
        Sequence destination;
        destination.Append(Resource(9));
        Resource::copiesBeforeThrow = 1;
        ExpectThrow<std::runtime_error>([&] { destination = source; });
        Resource::copiesBeforeThrow = -1;
        Check(destination.GetLength() == 1 && *destination[0].value == 9,
              "Failed assignment changed destination");
        Check(source.GetLength() == 3 && *source[0].value == 1,
              "Failed assignment changed source");
        Check(Resource::live == 4, "Failed assignment leaked resources");
        Resource::copiesBeforeThrow = 1;
        ExpectThrow<std::runtime_error>([&] { Sequence copy(source); });
        Resource::copiesBeforeThrow = -1;
        Check(Resource::live == 4, "Failed construction leaked resources");
        Resource::copiesBeforeThrow = 0;
        ExpectThrow<std::runtime_error>([&] { source.Append(source[0]); });
        Resource::copiesBeforeThrow = -1;
        Check(source.GetLength() == 3 && Resource::live == 4,
              "Failed insertion changed size or leaked resources");
    }
    Check(Resource::live == 0, "Failed copy cleanup leaked resources");
}

void ResourcesAndNestedTypes() {
    {
        DynamicArray<Resource> array;
        array.Append(Resource(1));
        array.Append(Resource(2));
        array.Reserve(20);
        Check(Resource::live == 2, "Reserve leaked resources");
        array.Resize(1);
        Check(Resource::live == 1, "Shrink did not release resources");
        array.RemoveAt(0);
        Check(Resource::live == 0, "Remove did not release resource");
        array.Resize(5);
        Check(Resource::live == 0, "New slots not default initialized");
        LinkedList<Resource> list;
        list.Append(Resource(3));
        list.Append(Resource(4));
        list.RemoveAt(0);
        Check(Resource::live == 1, "List removal leaked");
    }
    Check(Resource::live == 0, "Destruction leaked resources");

    DynamicArray<LinkedList<int>> buckets(2);
    buckets[0].Append(10);
    buckets[1].Append(20);
    const int* node = &buckets[0][0];
    buckets.Reserve(10);
    Check(&buckets[0][0] == node, "Reserve copied nested list instead of moving");
    buckets.Append(buckets[0]);
    DynamicArray<LinkedList<int>> copy = buckets;
    copy[0][0] = 99;
    Check(buckets[0][0] == 10 && copy[2][0] == 10, "Nested copy shares data");
    buckets.Resize(1);
    buckets.Resize(3);
    Check(buckets[1].GetLength() == 0 && buckets[2].GetLength() == 0,
          "Nested shrink retained elements");
    // A test-only record, not the future dictionary's KeyValue implementation
    struct Entry { std::string word; DynamicArray<int> pages; };
    Entry entry;
    entry.word = "algorithm";
    entry.pages.Append(1);
    entry.pages.Append(3);
    DynamicArray<Entry> entries;
    entries.Append(entry);
    entries.Append(entries[0]);
    auto entriesCopy = entries;
    entriesCopy[0].pages[0] = 7;
    Check(entries[0].pages[0] == 1 && entriesCopy[1].pages[1] == 3,
          "Complex type copy failed");
    auto moved = std::move(entriesCopy);
    Check(moved[0].word == "algorithm" && moved[0].pages[0] == 7,
          "Complex type move failed");
}

template <class Sequence>
void RepeatedOperations() {
    Sequence sequence;
    std::vector<int> reference;
    std::mt19937 random(42);
    for (int step = 0; step < 3000; ++step) {
        const int length = static_cast<int>(reference.size());
        if (length == 0 || random() % 2 == 0) {
            const int index = static_cast<int>(random() % (length + 1));
            sequence.InsertAt(step, index);
            reference.insert(reference.begin() + index, step);
        } else {
            const int index = static_cast<int>(random() % length);
            sequence.RemoveAt(index);
            reference.erase(reference.begin() + index);
        }
        CheckValues(sequence, reference);
    }
}

void LongListAndOwnership() {
    LinkedList<int> list;
    for (int i = 0; i < 100000; ++i) list.Append(i);
    auto iterator = list.GetIterator();
    int count = 0;
    while (iterator->HasNext()) {
        Check(iterator->Current() == count++, "Long list traversal failed");
        iterator->Next();
    }
    Check(count == 100000, "Long list lost elements");
    LinkedList<int> copy = list;
    copy = LinkedList<int>(); // Iterative cleanup in move assignment
    Check(copy.GetLength() == 0, "Long list cleanup failed");
    UniquePtr<ISequence<int>> owned(new LinkedList<int>(list));
    Check(owned->GetLast() == 99999, "Polymorphic ownership failed");
    // list and owned also exercise iterative destruction
}

void PointerSmokeTests() {
    UniquePtr<int> pointer(new int(5));
    UniquePtr<int> moved(std::move(pointer));
    Check(!pointer && *moved == 5, "UniquePtr move failed");
    int* raw = moved.release();
    Check(!moved, "UniquePtr release failed");
    pointer.reset(raw);
    pointer.swap(moved);
    Check(!pointer && *moved == 5, "UniquePtr swap failed");
    moved.reset();
    UniquePtr<int[]> array(new int[2]{});
    array[1] = 7;
    UniquePtr<int[]> other;
    other = std::move(array);
    Check(!array && other[1] == 7, "UniquePtr array move failed");
    SharedPtr<int> shared(new int(9));
    { SharedPtr<int> second(shared); Check(second.use_count() == 2, "SharedPtr count failed"); }
    Check(shared.use_count() == 1 && *shared == 9, "SharedPtr smoke test failed");
}

static_assert(std::is_base_of<ISequence<int>, DynamicArray<int>>::value, "Array inheritance");
static_assert(std::is_base_of<ISequence<int>, LinkedList<int>>::value, "List inheritance");
static_assert(!std::is_abstract<DynamicArray<int>>::value, "Array must be concrete");
static_assert(!std::is_abstract<LinkedList<int>>::value, "List must be concrete");
static_assert(std::is_same<decltype(std::declval<const DynamicArray<int>&>()[0]), const int&>::value,
              "Const array access must return a reference");
static_assert(std::is_same<decltype(std::declval<const LinkedList<int>&>()[0]), const int&>::value,
              "Const list access must return a reference");
} // namespace

int RunCorrectnessTests() {
    struct Test { const char* name; void (*run)(); };
    const Test tests[] = {
        {"DynamicArray operations and iterators", SequenceOperations<DynamicArray<int>>},
        {"LinkedList operations and iterators", SequenceOperations<LinkedList<int>>},
        {"DynamicArray copy and move", CopyAndMove<DynamicArray<int>>},
        {"LinkedList copy and move", CopyAndMove<LinkedList<int>>},
        {"Array capacity, resize, aliasing and growth", ArrayCapacityAndAliasing},
        {"Resource release and nested containers", ResourcesAndNestedTypes},
        {"DynamicArray repeated mutations", RepeatedOperations<DynamicArray<int>>},
        {"LinkedList repeated mutations", RepeatedOperations<LinkedList<int>>},
        {"Long list traversal and destruction", LongListAndOwnership},
        {"Existing smart pointers", PointerSmokeTests},
        {"DynamicArray failed copies", FailedCopies<DynamicArray<Resource>>},
        {"LinkedList failed copies", FailedCopies<LinkedList<Resource>>}
    };
    int failed = 0;
    for (const auto& test : tests) {
        try { test.run(); std::cout << "[PASS] " << test.name << '\n'; }
        catch (const std::exception& error) {
            ++failed;
            std::cerr << "[FAIL] " << test.name << ": " << error.what() << '\n';
        }
    }
    std::cout << "Tests: " << (sizeof(tests) / sizeof(tests[0])) << ", failures: " << failed << '\n';
    return failed == 0 ? 0 : 1;
}
