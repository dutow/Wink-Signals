
#include <catch.hpp>

#include <wink/signal.hpp>

void void_global_func_a(int& i, int& j) { i++; }
void void_global_func_b(int& i, int& j) { j++; }

SCENARIO("signals can be called") {
  typedef wink::signal<wink::slot<void(int&, int&)>> signal;
  signal sender;

  GIVEN("a global function") {
    sender.connect(&void_global_func_a);
    int i = 0;
    int j = 0;

    WHEN("the signal is called once") {
      sender(i, j);
      THEN("the effect of it's connected function is observed") {
        REQUIRE(i == 1);
        REQUIRE(j == 0);
      }
    }

    WHEN("the signal is called twice") {
      sender(i, j);
      sender(i, j);
      THEN("the effect of it's connected function is observed twice") {
        REQUIRE(i == 2);
      }
    }
  }

  GIVEN("the same global function twice") {
    sender.connect(&void_global_func_a);
    sender.connect(&void_global_func_a);
    int i = 0;
    int j = 0;

    WHEN("the signal is called once") {
      sender(i, j);
      THEN("the effect of it's connected function is observed twice") {
        REQUIRE(i == 2);
      }
    }

    WHEN("the signal is called twice") {
      sender(i, j);
      sender(i, j);
      THEN("the effect of it's connected function is observed four times") {
        REQUIRE(i == 4);
      }
    }
  }

  GIVEN("two different global functions") {
    sender.connect(&void_global_func_a);
    sender.connect(&void_global_func_b);
    int i = 0;
    int j = 0;

    WHEN("the signal is called once") {
      sender(i, j);
      THEN("the effect of it's connected functions is observed") {
        REQUIRE(i == 1);
        REQUIRE(j == 1);
      }
    }

    WHEN("the signal is called twice") {
      sender(i, j);
      sender(i, j);
      THEN("the effect of it's connected functions is observed twice") {
        REQUIRE(i == 2);
        REQUIRE(j == 2);
      }
    }
  }
}
