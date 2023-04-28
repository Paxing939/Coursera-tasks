#include <algorithm>
#include <utility>
#include <queue>
#include <future>

#include "Common.h"

using namespace std;

class LruCache : public ICache {
public:
  LruCache(
      shared_ptr<IBooksUnpacker> books_unpacker,
      const Settings &settings
  ) : books_unpacker_(std::move(books_unpacker)), settings_(settings), capacity_(0) {
    // реализуйте метод
  }

  BookPtr GetBook(const string &book_name) override {
    // реализуйте метод
    std::lock_guard<std::mutex> lock(m_);
    auto found_book = std::find_if(cache_.begin(), cache_.end(), [&book_name](const BookPtr &book) {
      return book->GetName() == book_name;
    });
    if (found_book != cache_.end()) {
      // в кэше есть книга двигаем приоритет и берем ее
      BookPtr tmp = *found_book;
      cache_.erase(found_book);
      cache_.push_front(tmp);
      return tmp;
    } else {
      // в кэше нету книги добавляем в кэш
      auto unique_book_ptr = books_unpacker_->UnpackBook(book_name);
      if (unique_book_ptr->GetContent().size() > settings_.max_memory) {
        cache_.clear();
        capacity_ = 0;
        return BookPtr(unique_book_ptr.release());
      }
      int new_book_size = unique_book_ptr->GetContent().size();
      if (new_book_size > static_cast<int>(settings_.max_memory) - static_cast<int>(capacity_)) {
        BookPtr tmp;
        while (new_book_size > static_cast<int>(settings_.max_memory) - static_cast<int>(capacity_)) {
          tmp = cache_.back();
          capacity_ -= static_cast<int>(tmp->GetContent().size());
          cache_.pop_back();
        }
      }
      capacity_ += static_cast<int>(unique_book_ptr->GetContent().size());
      cache_.push_front(BookPtr(unique_book_ptr.release()));
      return cache_.front();
    }
  }

private:
  shared_ptr<IBooksUnpacker> books_unpacker_;
  Settings settings_;
  int capacity_;
  std::deque<BookPtr> cache_;
  std::mutex m_;
};


unique_ptr<ICache> MakeCache(
    shared_ptr<IBooksUnpacker> books_unpacker,
    const ICache::Settings &settings
) {
  return std::make_unique<LruCache>(std::move(books_unpacker), settings);
}
