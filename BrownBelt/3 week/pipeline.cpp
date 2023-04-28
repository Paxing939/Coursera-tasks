#include "test_runner.h"
#include <functional>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>


struct Email {
  std::string from;
  std::string to;
  std::string body;
};


class Worker {
public:
  virtual ~Worker() = default;

  virtual void Process(std::unique_ptr<Email> email) = 0;

  virtual void Run() {
    throw std::logic_error("Unimplemented");
  }

protected:
  void PassOn(std::unique_ptr<Email> email) const {
    if (next_worker_) {
      next_worker_->Process(std::move(email));
    }
  }

public:
  void SetNext(std::unique_ptr<Worker> next) {
    if (next) {
      next_worker_ = std::move(next);
    }
  }

private:
  std::unique_ptr<Worker> next_worker_;
};


class Reader : public Worker {
public:

  Reader(std::istream &in) : input_(in) {}

  void Process(std::unique_ptr<Email> email) override {}

  void Run() override {
    std::string from;
    while (std::getline(input_, from)) {
      auto email = std::make_unique<Email>(Email{from, "", ""});
      std::getline(input_, email->to);
      std::getline(input_, email->body);
      PassOn(std::move(email));
    }
  }

private:
  std::istream &input_;
};


class Filter : public Worker {
public:
  using Function = std::function<bool(const Email &)>;

  Filter(Function function) : function_(std::move(function)) {}

  void Process(std::unique_ptr<Email> email) override {
    if (email && function_(*email)) {
      PassOn(std::move(email));
    }
  }

private:
  Function function_;
};


class Copier : public Worker {
public:

  Copier(std::string recipient) : recipient_(std::move(recipient)) {}

  void Process(std::unique_ptr<Email> email) override {
    if (email->to != recipient_) {
      auto copy = std::make_unique<Email>(Email{email->from, recipient_, email->body});
      PassOn(std::move(email));
      PassOn(std::move(copy));
    } else {
      PassOn(std::move(email));
    }
  }

private:
  std::string recipient_;
};


class Sender : public Worker {
public:

  Sender(std::ostream &out) : out_(out) {}

  void Process(std::unique_ptr<Email> email) override {
    out_ << email->from << '\n' << email->to << '\n' << email->body << '\n';
    PassOn(std::move(email));
  }

private:
  std::ostream &out_;
};


class PipelineBuilder {
public:
  // добавляет в качестве первого обработчика Reader
  explicit PipelineBuilder(std::istream &in) {
    worker_ = std::make_unique<Reader>(in);
    last_worker_ = worker_.get();
  }

  // добавляет новый обработчик Filter
  PipelineBuilder &FilterBy(Filter::Function filter) {
    InsertWorker(std::make_unique<Filter>(filter));
    return *this;
  }

  // добавляет новый обработчик Copier
  PipelineBuilder &CopyTo(std::string recipient) {
    InsertWorker(std::make_unique<Copier>(recipient));
    return *this;
  }

  // добавляет новый обработчик Sender
  PipelineBuilder &Send(std::ostream &out) {
    InsertWorker(std::make_unique<Sender>(out));
    return *this;
  }

  // возвращает готовую цепочку обработчиков
  std::unique_ptr<Worker> Build() {
    return std::move(worker_);
  }


private:

  void InsertWorker(std::unique_ptr<Worker> new_worker) {
    Worker *raw_ptr = new_worker.get();
    last_worker_->SetNext(std::move(new_worker));
    last_worker_ = raw_ptr;
  }

  std::unique_ptr<Worker> worker_;
  Worker *last_worker_;
};


void TestSanity() {
  std::string input = (
      "erich@example.com\n"
      "richard@example.com\n"
      "Hello there\n"

      "erich@example.com\n"
      "ralph@example.com\n"
      "Are you sure you pressed the right button?\n"

      "ralph@example.com\n"
      "erich@example.com\n"
      "I do not make mistakes of that kind\n"
  );
  std::istringstream inStream(input);
  std::ostringstream outStream;

  PipelineBuilder builder(inStream);
  builder.FilterBy([](const Email &email) {
    return email.from == "erich@example.com";
  });
  builder.CopyTo("richard@example.com");
  builder.Send(outStream);
  auto pipeline = builder.Build();

  pipeline->Run();

  std::string expectedOutput = (
      "erich@example.com\n"
      "richard@example.com\n"
      "Hello there\n"

      "erich@example.com\n"
      "ralph@example.com\n"
      "Are you sure you pressed the right button?\n"

      "erich@example.com\n"
      "richard@example.com\n"
      "Are you sure you pressed the right button?\n"
  );

  ASSERT_EQUAL(expectedOutput, outStream.str());
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestSanity);
  return 0;
}
