/* 
 * File:   unitTests.cpp
 * Author: massimo
 *
 * Created on October 04, 2017, 10:43 AM
 */

#include "../utilities.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <experimental/string_view>

using namespace ::testing;
using namespace std::experimental;
using namespace std::string_literals;

// BEGIN: ignore the warnings listed below when compiled with clang from here
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-member-function"
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#pragma clang diagnostic ignored "-Wglobal-constructors"
////////////////////////////////////////////////////////////////////////////////

TEST(helpersForConcurrentLogging, test_pclog)
{
  std::string s {"this is a test"};

  {
    std::clog << "PCLOG<start>" << std::endl;
    utilities::pclog{} << s << "\n";
    std::clog << "pclog" << std::endl;
  }
  std::clog << "PCLOG<end>" << std::endl;
}

TEST(helpersForConcurrentLogging, test_pcout)
{
  std::string s {"this is a test"};

  {
    std::cout << "PCOUT<start>" << std::endl;
    utilities::pcout{} << s << "\n";
    std::cout << "cout" << std::endl;
  }
  std::cout << "PCOUT<end>" << std::endl;
}

TEST(helpersForConcurrentLogging, test_pcerr)
{
  std::string s {"this is a test"};

  {
    std::cerr << "PCERR<start>" << std::endl;
    utilities::pcerr{} << s << "\n";
    std::cerr << "cerr" << std::endl;
  }
  std::cerr << "PCERR<end>" << std::endl;
}

TEST(helpersForConcurrentLogging, test_pcout_multithreaded)
{
  std::function<void(int)> print_cout = [] (int id)
  {
    std::cout << "cout hello from " << id << " " << std::endl;
  };

  std::function<void(int)> print_pcout = [] (int id)
  {
    utilities::pcout{} << "pcout hello from " << id << '\n';
  };

  constexpr size_t MAX_LOOP_INDEX {1000};
  std::vector<std::thread> v;

  for (size_t i {1}; i < MAX_LOOP_INDEX; ++i)
  {
    v.emplace_back(print_cout, i);
  }
  for (auto&& t : v)
  {
    t.join();
  }

  std::cout << "\n\n=====================\n";
  v.clear();

  for (size_t i {1}; i <= MAX_LOOP_INDEX; ++i)
  {
    v.emplace_back(print_pcout, i);
  }
  for (auto&& t : v)
  {
    t.join();
  }
}

TEST(withinHelper, test_within)
{
  ASSERT_EQ(false, utilities::within(0, 10, -1, -2, -3, 11, 12, 34));
  ASSERT_EQ(true, utilities::within(0, 10, 1, 2, 3, 10, 0, 4));
  ASSERT_EQ(false, utilities::within(0, 10, 1, 2, 3, 11, 10, 0, 4));
  ASSERT_EQ(true, utilities::within(0.0, 1.0, 0.00001, 0.9999999));
  ASSERT_EQ(true, utilities::within(0.0000001, 1.0, 0.00001, 0.9999999));

  std::string aaa {"aaa"};
  std::string bcd {"bcd"};
  std::string def {"def"};
  std::string zzz {"zzz"};
  ASSERT_EQ(true, utilities::within(aaa, zzz, bcd, def));
  ASSERT_EQ(false, utilities::within(aaa, def, bcd, zzz));

  ASSERT_EQ(true, utilities::within(static_cast<string_view>("aa"),
                                    static_cast<string_view>("ee"),
                                    static_cast<string_view>("cc"),
                                    static_cast<string_view>("bb"),
                                    static_cast<string_view>("dd")));

  ASSERT_EQ(false, utilities::within(static_cast<string_view>("aa"),
                                     static_cast<string_view>("ee"),
                                     static_cast<string_view>("cc"),
                                     static_cast<string_view>("bb"),
                                     "ff"s,
                                     static_cast<string_view>("dd")));

}

TEST(insertAllHelper, test_insertAll)
{
  std::vector<int> v {};
  
  ASSERT_EQ(0, v.size());
  utilities::insertAll(v);
  ASSERT_EQ(0, v.size());

  utilities::insertAll(v, 1, 2, 3, 4, 5);
  ASSERT_EQ(5, v.size());
}

TEST(quickRemoveHelper, test_quickRemove_from_unsorted_vector)
{
  {
    std::vector<int> v {123, 456, 789, 100, 200, 75, 325};

    utilities::quickRemoveAt(v, 2);

    ASSERT_EQ(6, v.size());

    utilities::printVectorElements(v);

    utilities::quickRemoveAt(v, std::find(std::begin(v), std::end(v), 123));

    ASSERT_EQ(5, v.size());

    utilities::printVectorElements(v);

    utilities::quickRemove(v, 325);

    ASSERT_EQ(4, v.size());

    utilities::printVectorElements(v);
  }
  {
    std::vector<int> w;

    utilities::quickRemoveAt(w, 10);
    utilities::quickRemoveAt(w, std::find(std::begin(w), std::end(w), 123));
    utilities::quickRemove(w, 1233);

    ASSERT_EQ(0, w.size());
  }
  {
    std::vector<int> y {2, 3, 4, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5};

    utilities::quickRemove(y, 5);
    utilities::quickRemove(y, 4);

    ASSERT_EQ(11, y.size());

    utilities::printVectorElements(y);
  }
  {
    std::vector<int> q {1};

    utilities::quickRemove(q, 1);

    ASSERT_EQ(0, q.size());
  }
}

TEST(removeAllItemsHelper, test_removeAllItems_1)
{
  std::vector<int> v {2, 3, 4, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5};

  utilities::removeAllItems(v, 4);
  utilities::removeAllItems(v, 100);

  ASSERT_EQ(9, v.size());

  utilities::printVectorElements(v);
}

TEST(remove_allItemsHelper, test_removeAllItems_2)
{
  std::vector<int> v;

  utilities::removeAllItems(v, 4);

  ASSERT_EQ(0, v.size());
}

TEST(insertSortedHelper, test_insertSorted)
{
  std::vector<int> v {10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 0, -1, -2, -3, -3, -4, -10, -15, -25, -35, 45};

  ASSERT_EQ(false, std::is_sorted(std::begin(v), std::end(v)));

  std::sort(std::begin(v), std::end(v));
  ASSERT_EQ(true, std::is_sorted(std::begin(v), std::end(v)));

  utilities::printVectorElements(v);

  utilities::insertSorted(v, 4);
  utilities::insertSorted(v, -20);
  utilities::insertSorted(v, 67);
  utilities::insertSorted(v, 23);
  ASSERT_EQ(true, std::is_sorted(std::begin(v), std::end(v)));
  utilities::printVectorElements(v);
  utilities::printContainerElements(v);
}

TEST(insertInMapHelper, test_insertInMap)
{
  std::map<std::string, int> m;
  ASSERT_EQ(0, m.size());

  // trying to get an item from a map creates a new element if the item does not exist yet
  int item = m["A"];
  ASSERT_EQ(0, item);
  ASSERT_EQ(1, m.size());
  utilities::printMapElements(m);

  auto [iterator, success] = utilities::insertInMap(m, static_cast<std::string>("B"), 123);
  ASSERT_EQ(true, success);
  ASSERT_EQ(2, m.size());
  item = m["B"];
  ASSERT_EQ(123, item);
  utilities::printMapElements(m);
}

TEST(valarrayProducts, test_valarrayProducts)
{
  std::valarray<double> a {1.0, 2.0, 3.0};
  std::valarray<double> b {4.0, 5.0, 6.0};

  // inner product
  // auto r { (a * b) };  // <-- DOES NOT COMPILE
  std::valarray<double> r { (a * b) };

  ASSERT_EQ(4.0,  r[0]);
  ASSERT_EQ(10.0, r[1]);
  ASSERT_EQ(18.0, r[2]);
  //dot product
  auto dotProd = std::accumulate(std::begin(r), std::end(r), 0.0);

  // dot product
  ASSERT_EQ(dotProd, r.sum());
  ASSERT_EQ(dotProd, utilities::dotProduct(a, b));

  // the 3 lines below would not compile without the overloading of operator<<()
  // for std::valarrray's
  std::cout << a << std::endl;
  std::cout << b << std::endl;
  std::cout << r << std::endl;

  utilities::printValarrayElements(a);
  utilities::printValarrayElements(b);
  utilities::printValarrayElements(r);

  std::cout << dotProd << std::endl;
  std::cout << r.sum() << std::endl;
  std::cout << utilities::dotProduct(a, b) << std::endl;
}

TEST(pairExtractionStreamer, test_pairExtractionStreamer)
{
  std::pair<int, int> p1 {1, 2};
  std::pair<double, std::string> p2 {11.45, "11.45"};
  std::pair<std::string, std::pair<int, double>> p3 {"A", {1, 23.78}};

  std::cout << p1 << std::endl;  
  std::cout << p2 << std::endl;  
  std::cout << p3 << std::endl;  
}

TEST(removeMultipleOccurrences, test_removeMultipleOccurrences)
{
  std::string s {"abccddeeffffffgh  i   "};

  utilities::removeMultipleOccurrences(s);

  std::cout << s << std::endl;
  ASSERT_EQ("abcdefgh i ", s);
}

TEST(printTuples, test_printTuples)
{
  utilities::printArgs(std::cout, 1, 2, "foo", 3, "bar\n");

  std::tuple<std::string, int, double, double> t {"A", 1, 3.14, 6.28};

  // ptintArgs() works with tuples as well
  utilities::printArgs(std::cout, t);
  std::cout << std::endl;
  // use of overloaded operator<<() for tuples
  std::cout << t << std::endl;
}

TEST(convertArrayToTuple, test_convertArrayToTuple)
{
  std::array<int, 4> a;
  std::iota(a.begin(), a.end(), 1);
  // a = {{1,2,3,4}}

  auto t = utilities::arrayToTuple(a);
  ASSERT_EQ(true, (std::is_same<decltype(t), std::tuple<int, int, int, int>>::value));
  ASSERT_EQ(a[0], get<0>(t));
  ASSERT_EQ(a[1], get<1>(t));
  ASSERT_EQ(a[2], get<2>(t));
  ASSERT_EQ(a[3], get<3>(t));
  std::cout << t << '\n';

  auto w = utilities::arrayToTuple<int, 3>({{3, 1, 4}});
  ASSERT_EQ(true, (std::is_same<decltype(w), std::tuple<int, int, int>>::value));
  ASSERT_EQ(3, get<0>(w));
  ASSERT_EQ(1, get<1>(w));
  ASSERT_EQ(4, get<2>(w));
  std::cout << w << '\n';
}

TEST(perfTimer, test_perfTimer_1)
{
  using namespace std::chrono_literals;
  using Granularity = std::chrono::seconds;

  constexpr auto SLEEP_TIME_S = 3s;
  auto func = [SLEEP_TIME_S] ()
  {
    std::this_thread::sleep_for(Granularity(SLEEP_TIME_S));
  };
  auto t = utilities::perftimer<Granularity>::duration(func).count();
  ASSERT_GE(t, SLEEP_TIME_S.count());
  std::cout << t << std::endl;
}

TEST(perfTimer, test_perfTimer_2)
{
  using namespace std::chrono_literals;
  using Granularity = std::chrono::milliseconds;

  constexpr auto SLEEP_TIME_MS = 587ms;
  auto func = [SLEEP_TIME_MS] ()
  {
    std::this_thread::sleep_for(Granularity(SLEEP_TIME_MS));
  };
  auto t = utilities::perftimer<Granularity>::duration(func).count();
  ASSERT_GE(t, SLEEP_TIME_MS.count());
  std::cout << t << std::endl;
}

TEST(perfTimer, test_perfTimer_3)
{
  using namespace std::chrono_literals;
  using Granularity = std::chrono::microseconds;

  constexpr auto SLEEP_TIME_US = 550689us;
  auto func = [SLEEP_TIME_US] ()
  {
    std::this_thread::sleep_for(Granularity(SLEEP_TIME_US));
  };
  auto t = utilities::perftimer<Granularity>::duration(func).count();
  ASSERT_GE(t, SLEEP_TIME_US.count());
  std::cout << t << std::endl;
}

TEST(perfTimer, test_perfTimer_4)
{
  using namespace std::chrono_literals;
  using Granularity = std::chrono::nanoseconds;

  constexpr auto SLEEP_TIME_NS = 1700500300ns;
  auto func = [SLEEP_TIME_NS] ()
  {
    std::this_thread::sleep_for(Granularity(SLEEP_TIME_NS));
  };
  auto t = utilities::perftimer<>::duration(func).count();
  ASSERT_GT(t, SLEEP_TIME_NS.count());
  std::cout << t << std::endl;
}

TEST (objectFactory, test_1)
{
  // let's create a factory for objects of this class
  class A
  {
    mutable int _x{};
    mutable int _y{};
    mutable int _z{};

   public:
    explicit
    A() noexcept
    {
      std::cout << "default constructor A(): ";
      display_object();
    }

    explicit
    A(const int& x) noexcept
    :
    _x(x)
    {
      std::cout << "constructor-1 A(x): ";
      display_object();
    }

    explicit
    A(const int& x, const int& y) noexcept
    :
    _x(x),
    _y(y)
    {
      std::cout << "constructor-2 A(x, y): ";
      display_object();
    }

    explicit
    A(const int& x, const int& y, const int& z) noexcept
    :
    _x(x),
    _y(y),
    _z(z)
    {
      std::cout << "constructor-3 A(x, y, z): ";
      display_object();
    }

    int get_x() const noexcept
    {
      return _x;
    }
    int get_y() const noexcept
    {
      return _y;
    }
    int get_z() const noexcept
    {
      return _z;
    }

    void display_object (const std::string& prompt = "") const noexcept
    {
      std::cout << prompt
                << "(x, y, z): ("
                << get_x()
                << ", "
                << get_y()
                << ", "
                << get_z()
                << ")"
                << std::endl;
    }

    A(const A& rhs) = delete;
    A& operator=(const A& rhs) = delete;

    ~A()
    {
      std::cout << "Destroyed object: ";
      display_object();
    }
  };  // class A

  // The object's type returned by the factory function
  using Object = std::unique_ptr<A>;
  // The factory function's type
  using objectFactoryFunction = utilities::object_factory::objectFactoryFun<A>;

  // The factory function
  objectFactoryFunction objectFactoryFun {};

  // Create a factory function for our objects that call the default ctor
  objectFactoryFun = utilities::object_factory::createObjectFactoryFun<A>();
  // Create an object using the factory function
  Object o0 = objectFactoryFun();

  {
    // Create a factory function for our objects that call the ctor A(const int& x)
    objectFactoryFun = utilities::object_factory::createObjectFactoryFun<A, const int>(123);
  }
  // Create an object using the factory function
  Object o1 = objectFactoryFun();

  // Create a factory function for our objects that call the ctor A(const int& x)
  objectFactoryFun = utilities::object_factory::createObjectFactoryFun<A,const int>(456);
  // Create an object using the factory function
  Object o2 = objectFactoryFun();

  // Create a factory function for our objects that call the ctor A(const int& x, const int& y)
  objectFactoryFun = utilities::object_factory::createObjectFactoryFun<A,const int, const int>(11, 22);
  // Create an object using the factory function
  Object o3 = objectFactoryFun();

  // Create a factory function for our objects that call the ctor A(const int& x, const int& y, const int& z)
  objectFactoryFun = utilities::object_factory::createObjectFactoryFun<A,const int, const int, const int>(11, 22, 33);
  // Create an object using the factory function
  Object o4 = objectFactoryFun();

  // check the values in the objects created are right
  ASSERT_EQ(o0.get()->get_x(), 0);
  ASSERT_EQ(o0.get()->get_y(), 0);
  ASSERT_EQ(o0.get()->get_z(), 0);
  ASSERT_EQ(o1.get()->get_x(), 123);
  ASSERT_EQ(o1.get()->get_y(), 0);
  ASSERT_EQ(o1.get()->get_z(), 0);
  ASSERT_EQ(o2.get()->get_x(), 456);
  ASSERT_EQ(o2.get()->get_y(), 0);
  ASSERT_EQ(o2.get()->get_z(), 0);
  ASSERT_EQ(o3.get()->get_x(), 11);
  ASSERT_EQ(o3.get()->get_y(), 22);
  ASSERT_EQ(o3.get()->get_z(), 0);
  ASSERT_EQ(o4.get()->get_x(), 11);
  ASSERT_EQ(o4.get()->get_y(), 22);
  ASSERT_EQ(o4.get()->get_z(), 33);

  {
    // Create a factory function for our objects that call the ctor A(const int& x, const int& y, const int& z)
    objectFactoryFun = utilities::object_factory::createObjectFactoryFun<A,
                                                              const int,
                                                              const int,
                                                              const int>
                                                              (99, 88, 77);
    // define a vector for the objects created by the factory
    std::vector<Object> v {};

    // create 5 objects using the factory and store them in the vector
    for (int i = 1; i <= 5; ++i)
    {
      // create an object that can be used in this scope only
      Object o = objectFactoryFun();
      // store the object in the vector v using move semantics;
      // the lifetime of the object is extended over this scope
      v.push_back(std::move(o));
    }
    // all elements in the vector have the same attribute values
    for (auto&& item : v)
    {
      ASSERT_EQ(item.get()->get_x(), 99);
      ASSERT_EQ(item.get()->get_y(), 88);
      ASSERT_EQ(item.get()->get_z(), 77);
    }
  }  // the objects in the vector v are destroyed, going out of scope
}

TEST(uniquePtr2sharedPtr, test_1)
{
  class A
  {
    mutable int _x{};
    mutable int _y{};
    mutable int _z{};

   public:
    explicit
    A() noexcept
    {
      std::cout << "default constructor A(): ";
      display_object();
    }

    explicit
    A(const int& x) noexcept
    :
    _x(x)
    {
      std::cout << "constructor-1 A(x): ";
      display_object();
    }

    explicit
    A(const int& x, const int& y) noexcept
    :
    _x(x),
    _y(y)
    {
      std::cout << "constructor-2 A(x, y): ";
      display_object();
    }

    explicit
    A(const int& x, const int& y, const int& z) noexcept
    :
    _x(x),
    _y(y),
    _z(z)
    {
      std::cout << "constructor-3 A(x, y, z): ";
      display_object();
    }

    int get_x() const noexcept
    {
      return _x;
    }
    int get_y() const noexcept
    {
      return _y;
    }
    int get_z() const noexcept
    {
      return _z;
    }

    void display_object (const std::string& prompt = "") const noexcept
    {
      std::cout << prompt
                << "(x, y, z): ("
                << get_x()
                << ", "
                << get_y()
                << ", "
                << get_z()
                << ")"
                << std::endl;
    }

    A(const A& rhs) = delete;
    A& operator=(const A& rhs) = delete;

    ~A()
    {
      std::cout << "Destroyed object: ";
      display_object();
    }
  };  // class A

  // create a unique_ptr that points to an A's object created using the object factory
  std::unique_ptr<A> uptr = utilities::object_factory::createUniquePtr<A>(11, 22, 33);
  uptr.get()->display_object("Object pointed by uptr: ");
  ASSERT_NE(nullptr, uptr.get());
  ASSERT_EQ(true, static_cast<bool>((uptr)));
  ASSERT_EQ(true, uptr.operator bool());
  ASSERT_EQ(11, uptr.get()->get_x());
  ASSERT_EQ(22, uptr.get()->get_y());
  ASSERT_EQ(33, uptr.get()->get_z());

  // create a shared pointer to the object: don't try to access the unique_ptr from now on
  std::shared_ptr<A> shptr {utilities::object_factory::uniquePtr2sharedPtr(uptr)};
  // display the object pointed by the shared pointer
  shptr.get()->display_object("Object pointed by shptr: ");

  // now uptr.get() returns nullptr
  ASSERT_EQ(nullptr, uptr.get());
  ASSERT_EQ(false, static_cast<bool>((uptr)));
  ASSERT_EQ(false, uptr.operator bool());
  // the shared pointer must contain the moved data
  ASSERT_EQ(11, shptr.get()->get_x());
  ASSERT_EQ(22, shptr.get()->get_y());
  ASSERT_EQ(33, shptr.get()->get_z());
  ASSERT_EQ(1, shptr.use_count());
  ASSERT_EQ(true, shptr.unique());
  
  // create a shared pointer from an invalid unique_ptr
  std::shared_ptr<A> shptrBad {utilities::object_factory::uniquePtr2sharedPtr(uptr)};
  // the shared pointer must be nullptr
  ASSERT_EQ(nullptr, shptrBad.get());
  ASSERT_EQ(false, static_cast<bool>((shptrBad)));
  ASSERT_EQ(false, shptrBad.operator bool());
}

TEST(memVarTest, test_1)
{
  EXPECT_THROW(utilities::memvar<int> mv(0,0), std::invalid_argument);
  EXPECT_THROW(utilities::memvar<int> mv(0,-10), std::invalid_argument);
  EXPECT_NO_THROW(utilities::memvar<int> mv {});
  EXPECT_NO_THROW(utilities::memvar<int> mv(11, 20));

  EXPECT_THROW(utilities::memvar<double> mv(0.0,0), std::invalid_argument);
  EXPECT_THROW(utilities::memvar<double> mv(0.0,-10), std::invalid_argument);
  EXPECT_NO_THROW(utilities::memvar<double> mv {});
  EXPECT_NO_THROW(utilities::memvar<double> mv(11.2345, 20));

  EXPECT_THROW(utilities::memvar<std::string> mv("Hello World!",0), std::invalid_argument);
  EXPECT_THROW(utilities::memvar<std::string> mv("Hello World!",-10), std::invalid_argument);
  EXPECT_NO_THROW(utilities::memvar<std::string> mv {});
  EXPECT_NO_THROW(utilities::memvar<std::string> mv("Hello World!", 20));
}

TEST(memVarTest, test_2)
{
  utilities::memvar<int> mv {};
  ASSERT_EQ(10, mv.getHistoryCapacity());
  mv.printHistoryData();
  ASSERT_EQ(int {}, mv());

  int v = mv();
  ASSERT_EQ(int {}, v);
  ASSERT_EQ(0, v);

  v = 123;
  ASSERT_NE(int {}, v);
  ASSERT_NE(v, mv());

  int w = mv();
  ASSERT_EQ(int {}, w);
  ASSERT_EQ(0, w);
}

TEST(memVarTest, test_3)
{
  utilities::memvar<int> mv {55};
  mv.printHistoryData();
  ASSERT_EQ(55, mv());

  int v = mv();
  ASSERT_EQ(55, v);

  v = 123;
  ASSERT_NE(55, v);
  ASSERT_NE(v, mv());

  int w = mv();
  ASSERT_EQ(55, w);
}

TEST(memVarTest, test_4)
{
  utilities::memvar<int> mv {33,9};
  ASSERT_EQ(9, mv.getHistoryCapacity());

  ASSERT_EQ(33, mv());

  mv = 78;
  mv.printHistoryData();

  ASSERT_EQ(78, mv());

  mv = 45;
  mv.printHistoryData();

  ASSERT_EQ(45, mv());

  ++mv;
  mv.printHistoryData();

  ASSERT_EQ(46, mv());

  mv = ++mv + mv();
  mv.printHistoryData();

  ASSERT_EQ(94, mv());

  mv++;
  mv.printHistoryData();

  ASSERT_EQ(95, mv());

  mv = ++mv + mv() + mv++;
  mv.printHistoryData();

  ASSERT_EQ(289, mv());

  mv--;
  mv.printHistoryData();

  ASSERT_EQ(288, mv());

  mv = --mv - mv() - mv--;

  ASSERT_EQ(-286, mv());

  // ways of printing the history data in the memvar
  mv.printHistoryData();
  std::cout << mv.getMemVarHistory() << std::endl;
  utilities::printDequeElements(mv.getMemVarHistory());
  utilities::printContainerElements(mv.getMemVarHistory());
  auto mvh {mv.getMemVarHistory()};
  std::cout << mvh << std::endl;
  utilities::printDequeElements(mvh);
  utilities::printContainerElements(mvh);
}

TEST(memVarTest, test_5)
{
  using varType = int64_t;
  // you need enough memory to run this test
  // if not, swap will be used if swap is on
  constexpr utilities::memvar<varType>::capacityType historyCapacity {1'000'000'000};
  utilities::memvar<varType> mv {0,historyCapacity};
  ASSERT_EQ(historyCapacity, mv.getHistoryCapacity());

  auto func = [&mv] ()
  {
    varType c {0};
    while ( false == mv.isHistoryFull() )
    {
      mv = ++c;
    }
  };
  auto t = utilities::perftimer<>::duration(func).count();
  std::cout << "loop took: " << t << " nsec" << std::endl;

  ASSERT_EQ(mv.getHistoryCapacity(), mv.getHistorySize());
  ASSERT_EQ(historyCapacity - 1, mv());

  auto clearHistory = [&mv] ()
  {
    mv.clearHistory();
  };
  t = utilities::perftimer<>::duration(clearHistory).count();
  std::cout << "history clearing took: " << t << " nsec" << std::endl;
  ASSERT_EQ(historyCapacity - 1, mv());
  ASSERT_EQ(historyCapacity, mv.getHistoryCapacity());
  ASSERT_EQ(1, mv.getHistorySize());
  mv.printHistoryData();
}

TEST(memVarTest, test_6)
{
  using varType = int64_t;
  constexpr utilities::memvar<varType>::capacityType historyCapacity {100};
  constexpr varType maxValue {1'000'000'000};
  utilities::memvar<varType> mv {0,historyCapacity};
  ASSERT_EQ(historyCapacity, mv.getHistoryCapacity());

  auto func = [&mv, maxValue] ()
  {
    varType c {0};
    while ( c < maxValue )
    {
      mv = ++c;
    }
  };
  auto t = utilities::perftimer<>::duration(func).count();
  std::cout << "loop took: " << t << " nsec" << std::endl;

  ASSERT_EQ(maxValue, mv());
  ASSERT_EQ(historyCapacity, mv.getHistorySize());
  //mv.printHistoryData();

  auto clearHistory = [&mv] ()
  {
    mv.clearHistory();
  };
  t = utilities::perftimer<>::duration(clearHistory).count();
  std::cout << "history clearing took: " << t << " nsec" << std::endl;
  ASSERT_EQ(historyCapacity, mv.getHistoryCapacity());
  ASSERT_EQ(1, mv.getHistorySize());
  ASSERT_EQ(maxValue, mv());
  mv.printHistoryData();
}

TEST(memVarTest, test_7)
{
  utilities::memvar<std::string> mvs {"A"};
  std::string s {"B"};
  mvs = s;
  mvs = mvs() + s;
  mvs.printHistoryData();

  ASSERT_EQ("BB", mvs());
  ASSERT_EQ(3, mvs.getHistorySize());
}

TEST(memVarTest, test_8)
{
  using varType = char;
  utilities::memvar<varType> mvc {'A'};
  varType c = mvc() + 1;
  mvc = c;  // B
  mvc++;  // C
  mvc++;  // D
  mvc--;  // C
  // [ C D C B A  ]
  mvc.printHistoryData();

  ASSERT_EQ('C', mvc());
  ASSERT_EQ(5, mvc.getHistorySize());

  utilities::memvar<varType>::historyValue hv {};

  hv = mvc.getHistoryValue(1);
  ASSERT_EQ('D', std::get<varType>(hv));
  ASSERT_EQ(false, std::get<bool>(hv));

  hv = mvc.getHistoryValue(2);
  ASSERT_EQ('C', std::get<varType>(hv));
  ASSERT_EQ(false, std::get<bool>(hv));

  varType value {};
  bool error {};

  std::tie(value, error) = mvc.getHistoryValue(3);
  ASSERT_EQ('B', value);
  ASSERT_EQ(false, error);

  std::tie(value, error) = mvc.getHistoryValue(4);
  ASSERT_EQ('A', value);
  ASSERT_EQ(false, error);

  // trying to access the history out of bound
  std::tie(value, error) = mvc.getHistoryValue(5);
  ASSERT_EQ(char {}, value);
  ASSERT_EQ(true, error);

  // trying to access the history out of bound
  auto [v, e] = mvc.getHistoryValue(100000);
  ASSERT_EQ(varType {}, v);
  ASSERT_EQ(true, e);
}

////////////////////////////////////////////////////////////////////////////////
static inline
void testIOColor(const enum iocolor::color fg,
                 const enum iocolor::color bg) noexcept
{
  std::cout << iocolor::color::reset
            << iocolor::foreground(fg)
            << iocolor::background(bg)
            << " REGULAR "
            << iocolor::color::reset
            << iocolor::foreground(fg)
            << iocolor::background(bg)
            << iocolor::effects(iocolor::effect::bold)
            << " BOLD "
            << iocolor::color::reset
            << iocolor::foreground(fg)
            << iocolor::background(bg)
            << iocolor::effects(iocolor::effect::underline)
            << " UNDERLINE "
            << iocolor::color::reset
            << iocolor::foreground(fg)
            << iocolor::background(bg)
            << iocolor::effects(iocolor::effect::reverse)
            << " REVERSE "
            << iocolor::color::reset
            << iocolor::foreground(fg)
            << iocolor::background(bg)
            << iocolor::effects(iocolor::effect::strike)
            << " STRIKE "
            << iocolor::color::reset
            << std::endl;
}

TEST(ioColor, test_full_1)
{
  const std::vector<iocolor::color> foreGroundVector
  {
    iocolor::color::black,
    iocolor::color::red,
    iocolor::color::green,
    iocolor::color::yellow,
    iocolor::color::blue,
    iocolor::color::magenta,
    iocolor::color::cyan,
    iocolor::color::white,
  };
  const std::vector<iocolor::color> backGroundVector
  {
    iocolor::color::black,
    iocolor::color::red,
    iocolor::color::green,
    iocolor::color::yellow,
    iocolor::color::blue,
    iocolor::color::magenta,
    iocolor::color::cyan,
    iocolor::color::white,
  };
  const std::vector<iocolor::effect> effectVector
  {
    iocolor::effect::none,
    iocolor::effect::bold,
    iocolor::effect::underline,
    iocolor::effect::reverse,
    iocolor::effect::strike,
  };

  auto f1 = [&backGroundVector, &effectVector]
            (const iocolor::color& foreGroundColor) noexcept
            {
              auto f2 = [&effectVector, &foreGroundColor]
                        (const iocolor::color& backGroundColor) noexcept
                        {
                          auto f3 = [&foreGroundColor, &backGroundColor]
                                    (const iocolor::effect& effect) noexcept
                                    {
                                      std::cout << iocolor::foreground(foreGroundColor)
                                                << iocolor::background(backGroundColor)
                                                << iocolor::effects(effect)
                                                << " ABCDEFGHabcdefgh012345 "
                                                << iocolor::color::reset;
                                    };
                                    std::for_each(std::begin(effectVector),
                                                  std::end(effectVector),
                                                  f3);
                                    std::cout << std::endl;
                        };
              std::for_each(std::begin(backGroundVector), std::end(backGroundVector), f2);
            };
  std::for_each(std::begin(foreGroundVector), std::end(foreGroundVector), f1);

// the code above does the same of the following commented code
//  for (auto&& fgc : foreGroundVector)
//  {
//    for (auto&& bgc : backGroundVector)
//    {
//      for (auto&& eff: effectVector)
//      {
//        std::cout << iocolor::foreground(fgc)
//                  << iocolor::background(bgc)
//                  << iocolor::effects(eff)
//                  << " ABCDEFGHabcdefgh012345 "
//                  << iocolor::color::reset;
//      }
//      std::cout << std::endl;
//    }
//  }
}

TEST(ioColor, test_full_2)
{
  const std::vector<iocolor::color> foreGroundColorVector
  {
    iocolor::color::black,
    iocolor::color::red,
    iocolor::color::green,
    iocolor::color::yellow,
    iocolor::color::blue,
    iocolor::color::magenta,
    iocolor::color::cyan,
    iocolor::color::white,
  };
  const std::vector<iocolor::color> backGroundColorVector
  {
    iocolor::color::black,
    iocolor::color::red,
    iocolor::color::green,
    iocolor::color::yellow,
    iocolor::color::blue,
    iocolor::color::magenta,
    iocolor::color::cyan,
    iocolor::color::white,
  };
  
  auto f1 = [&backGroundColorVector]
            (const iocolor::color& foreGroundColor) noexcept
            {
              auto f2 = [&foreGroundColor]
                        (const iocolor::color& backGroundColor) noexcept
                        {
                          testIOColor(foreGroundColor,backGroundColor);  
                        };
              std::for_each(std::begin(backGroundColorVector),
                            std::end(backGroundColorVector),
                            f2);
            };
  std::for_each(std::begin(foreGroundColorVector), std::end(foreGroundColorVector), f1);

// the code above does the same of the following commented code
//  for (auto&& foreGroundColor : foreGroundColorVector)
//  {
//    for (auto&& backGroundColor : backGroundColorVector)
//    {
//      testIOColor(foreGroundColor, backGroundColor);
//    }
//  }
}

TEST(ioColor, test_full_3)
{
  std::cout << iocolor::color::reset
            << iocolor::foreground(iocolor::color::black)
            << iocolor::background(iocolor::color::black)
            << "          "
            << iocolor::foreground(iocolor::color::red)
            << iocolor::background(iocolor::color::red)
            << "          "
            << iocolor::foreground(iocolor::color::green)
            << iocolor::background(iocolor::color::green)
            << "          "
            << iocolor::foreground(iocolor::color::yellow)
            << iocolor::background(iocolor::color::yellow)
            << "          "
            << iocolor::foreground(iocolor::color::blue)
            << iocolor::background(iocolor::color::blue)
            << "          "
            << iocolor::foreground(iocolor::color::magenta)
            << iocolor::background(iocolor::color::magenta)
            << "          "
            << iocolor::foreground(iocolor::color::cyan)
            << iocolor::background(iocolor::color::cyan)
            << "          "
            << iocolor::foreground(iocolor::color::white)
            << iocolor::background(iocolor::color::white)
            << "          "
            << iocolor::color::reset
            << std::endl;
}

static
const std::string s = " ABCDEFGHIJKLMNOPQRSTUVWXYZ-0123456789-_\\|!\"£$%&/()=?^{}[]@#,.;: "s;

TEST(ioColor, testMakeColor_1)
{
  short color {};

  for (color = 0; color <= 255; ++color)
  {
    std::cout << iocolor::make_color(color)
              << "fg "
              << color
              << s
              << iocolor::color::reset
              << std::endl;
  }
}

TEST(ioColor, testMakeColor_2)
{
  short foreGroundColor {};

  for (foreGroundColor = 0; foreGroundColor <= 255; ++foreGroundColor)
  {
    std::cout << iocolor::foreground(foreGroundColor)
              << "fg "
              << foreGroundColor
              << s
              << iocolor::color::reset
              << std::endl;
  }
}

TEST(ioColor, testMakeColor_3)
{
  short backGroundColor {};

  for (backGroundColor = 0; backGroundColor <= 255; ++backGroundColor)
  {
    std::cout << iocolor::background(backGroundColor)
              << "bg "
              << backGroundColor
              << s
              << iocolor::color::reset
              << std::endl;
  }
}

// very long output: commented out
// uncomment if wishing to run it
//TEST(ioColor, testMakeColor_4)
//{
//  short foreGroundColor {};
//  short backGroundColor {};
//
//  for (foreGroundColor = 0; foreGroundColor <= 255; ++foreGroundColor)
//  {
//    for (backGroundColor = 0; backGroundColor <= 255; ++backGroundColor)
//    {
//      std::cout << iocolor::foreground(foreGroundColor)
//                << iocolor::background(backGroundColor)
//                << "fg "
//                << foreGroundColor
//                << " bg "
//                << backGroundColor
//                << s
//                << iocolor::color::reset
//                << std::endl;
//    }
//  }
//}
////////////////////////////////////////////////////////////////////////////////
#pragma clang diagnostic pop
// END: ignore the warnings when compiled with clang up to here
