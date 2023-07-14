#include <tgbot/tgbot.h>

#include <cstdlib>
#include <exception>
#include <iostream>
#include <random>

class ClownToss {
    static constexpr float clownProbability = 0.1;

public:
    ClownToss()
    {
        auto rd = std::random_device{};
        _engine = std::mt19937{rd()};
    }

    bool operator()()
    {
        return _dist(_engine) <= clownProbability;
    }

private:
    std::mt19937 _engine;
    std::uniform_real_distribution<float> _dist;
};

int main() {
    const char* token = std::getenv("TOKEN");
    if (!token) {
        std::cerr << "Fill TOKEN environment variable\n";
        return EXIT_FAILURE;
    }

    auto toss = ClownToss{};

    auto bot = TgBot::Bot{token};

    bot.getEvents().onCommand("start", [&bot] (TgBot::Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Я робот-клоунщик!");
    });
    bot.getEvents().onAnyMessage([&bot, &toss] (TgBot::Message::Ptr message) {
        if (toss()) {
            std::cout << "toss pass\n";
            bot.getApi().sendMessage(message->chat->id, "\xF0\x9F\xA4\xA1", false, message->messageId);
        } else {
            std::cout << "toss fail\n";
        }
    });

    try {
        auto longPoll = TgBot::TgLongPoll{bot};
        for (;;) {
            std::cout << "polling\n";
            longPoll.start();
        }
    } catch (const std::exception& e) {
        std::cerr << e.what();
    }
}
