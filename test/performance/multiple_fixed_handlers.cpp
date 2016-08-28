
#include <hayai.hpp>

#include <functional>
#include <wink/signal.hpp>

template <int i>
struct MultipleFixedHandlers {
  volatile int tmp{i};
  void handleEvent(int const& x) {
    int y = tmp + x;
    int z = static_cast<int>(std::sqrt(y) * y + x);
    tmp += z;
  }
};

class MultipleFixedHandlersFixture : public hayai::Fixture {
 public:
  std::vector<int> numbersToSend;

  void SetUp() override {
    numbersToSend.resize(10000);
    for (std::vector<int>::iterator i = numbersToSend.begin();
         i != numbersToSend.end(); ++i) {
      *i = rand();
    }
  }

  void TearDown() override {}
};

BENCHMARK_F(MultipleFixedHandlersFixture, DirectCall, 20, 100) {
  MultipleFixedHandlers<1> handler1;
  MultipleFixedHandlers<2> handler2;
  MultipleFixedHandlers<3> handler3;
  MultipleFixedHandlers<4> handler4;
  MultipleFixedHandlers<5> handler5;

  for (std::vector<int>::iterator i = numbersToSend.begin();
       i != numbersToSend.end(); ++i) {
    handler1.handleEvent(*i);
    handler2.handleEvent(*i);
    handler3.handleEvent(*i);
    handler5.handleEvent(*i);
    handler5.handleEvent(*i);
  }
}

BENCHMARK_F(MultipleFixedHandlersFixture, StdFunctionWithBind, 20, 100) {
  MultipleFixedHandlers<1> handler1;
  MultipleFixedHandlers<2> handler2;
  MultipleFixedHandlers<3> handler3;
  MultipleFixedHandlers<4> handler4;
  MultipleFixedHandlers<5> handler5;

  std::vector<std::function<void(int const&)>> funs;
  funs.push_back(std::bind(&MultipleFixedHandlers<1>::handleEvent, &handler1,
                           std::placeholders::_1));
  funs.push_back(std::bind(&MultipleFixedHandlers<2>::handleEvent, &handler2,
                           std::placeholders::_1));
  funs.push_back(std::bind(&MultipleFixedHandlers<3>::handleEvent, &handler3,
                           std::placeholders::_1));
  funs.push_back(std::bind(&MultipleFixedHandlers<4>::handleEvent, &handler4,
                           std::placeholders::_1));
  funs.push_back(std::bind(&MultipleFixedHandlers<5>::handleEvent, &handler5,
                           std::placeholders::_1));

  for (std::vector<int>::iterator i = numbersToSend.begin();
       i != numbersToSend.end(); ++i) {
    for (auto& fun : funs) fun(*i);
  }
}

BENCHMARK_F(MultipleFixedHandlersFixture, StdFunctionWithLambda, 20, 100) {
  MultipleFixedHandlers<1> handler1;
  MultipleFixedHandlers<2> handler2;
  MultipleFixedHandlers<3> handler3;
  MultipleFixedHandlers<4> handler4;
  MultipleFixedHandlers<5> handler5;

  std::vector<std::function<void(int const&)>> funs;
  funs.push_back([&handler1](int i) { handler1.handleEvent(i); });
  funs.push_back([&handler2](int i) { handler2.handleEvent(i); });
  funs.push_back([&handler3](int i) { handler3.handleEvent(i); });
  funs.push_back([&handler4](int i) { handler4.handleEvent(i); });
  funs.push_back([&handler5](int i) { handler5.handleEvent(i); });

  for (std::vector<int>::iterator i = numbersToSend.begin();
       i != numbersToSend.end(); ++i) {
    for (auto& fun : funs) fun(*i);
  }
}

BENCHMARK_F(MultipleFixedHandlersFixture, WinkSignal, 20, 100) {
  typedef wink::signal<wink::slot<void(int const&)>> signal;
  signal sender;

  MultipleFixedHandlers<1> handler1;
  MultipleFixedHandlers<2> handler2;
  MultipleFixedHandlers<3> handler3;
  MultipleFixedHandlers<4> handler4;
  MultipleFixedHandlers<5> handler5;

  sender.connect(&handler1, &MultipleFixedHandlers<1>::handleEvent);
  sender.connect(&handler2, &MultipleFixedHandlers<2>::handleEvent);
  sender.connect(&handler3, &MultipleFixedHandlers<3>::handleEvent);
  sender.connect(&handler4, &MultipleFixedHandlers<4>::handleEvent);
  sender.connect(&handler5, &MultipleFixedHandlers<5>::handleEvent);

  for (std::vector<int>::iterator i = numbersToSend.begin();
       i != numbersToSend.end(); ++i) {
    // emit the event
    sender(*i);
  }
}
