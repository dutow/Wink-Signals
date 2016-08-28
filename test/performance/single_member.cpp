
#include <hayai.hpp>

#include <functional>
#include <wink/signal.hpp>

struct SingleMember {
  volatile int tmp{};
  void handleEvent(int const& x) {
    int y = tmp + x;
    int z = static_cast<int>(std::sqrt(y) * y + x);
    tmp += z;
  }
};

class SingleMemberFixture : public hayai::Fixture {
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

BENCHMARK_F(SingleMemberFixture, DirectCall, 20, 100) {
  SingleMember handler;

  for (std::vector<int>::iterator i = numbersToSend.begin();
       i != numbersToSend.end(); ++i) {
    handler.handleEvent(*i);
  }
}

BENCHMARK_F(SingleMemberFixture, StdFunctionWithBind, 20, 100) {
  SingleMember handler;
  std::function<void(int const&)> fun =
      std::bind(&SingleMember::handleEvent, &handler, std::placeholders::_1);

  for (std::vector<int>::iterator i = numbersToSend.begin();
       i != numbersToSend.end(); ++i) {
    fun(*i);
  }
}

BENCHMARK_F(SingleMemberFixture, StdFunctionWithLambda, 20, 100) {
  SingleMember handler;
  std::function<void(int const&)> fun = [&handler](int i) {
    handler.handleEvent(i);
  };

  for (std::vector<int>::iterator i = numbersToSend.begin();
       i != numbersToSend.end(); ++i) {
    fun(*i);
  }
}

BENCHMARK_F(SingleMemberFixture, WinkSignal, 20, 100) {
  typedef wink::signal<wink::slot<void(int const&)>> signal;
  SingleMember handler;
  signal sender;

  sender.connect(&handler, &SingleMember::handleEvent);

  for (std::vector<int>::iterator i = numbersToSend.begin();
       i != numbersToSend.end(); ++i) {
    // emit the event
    sender(*i);
  }
}
