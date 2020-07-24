#include <iostream>
#include <string>
#include <utility>

using namespace std;

void SendSms(const string &number, const string &message) {
    cout << "Send '" << message << "' to number " << number << endl;
}

void SendEmail(const string &email, const string &message) {
    cout << "Send '" << message << "' to e-mail " << email << endl;
}

/*
 Реализуйте здесь классы INotifier, SmsNotifier, EmailNotifier
 */
class INotifier {
public:
    virtual void Notify(const string &message) = 0;
};

class SmsNotifier : public INotifier {
public:

    explicit SmsNotifier(string phoneNumber) : phone_number_(std::move(phoneNumber)) {}

    void Notify(const string &message) override {
        SendSms(phone_number_, message);
    }

private:
    string phone_number_;
};

class EmailNotifier : public INotifier {
public:

    explicit EmailNotifier(string emailAddress) : email_address_(std::move(emailAddress)) {}

    void Notify(const string &message) override {
        SendEmail(email_address_, message);
    }

private:
    string email_address_;
};

void Notify(INotifier &notifier, const string &message) {
    notifier.Notify(message);
}

int main() {
    SmsNotifier sms{"+7-495-777-77-77"};
    EmailNotifier email{"na-derevnyu@dedushke.ru"};

    Notify(sms, "I have White belt in C++");
    Notify(email, "And want a Yellow one");
    return 0;
}
