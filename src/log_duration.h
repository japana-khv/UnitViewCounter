#pragma once

#include <chrono>
#include <iostream>
#include <string>

class LogDuration {
public:
	// заменим имя типа std::chrono::steady_clock
	// с помощью using для удобства
	using Clock = std::chrono::steady_clock;

	LogDuration(const std::string &name, std::ostream& os = std::cerr) :
			name_(name), os_(os) {
	}

	~LogDuration() {
		using namespace std::chrono;
		using namespace std::literals;

		const auto end_time = Clock::now();
		const auto dur = end_time - start_time_;
		std::cerr << name_ << ": "s << duration_cast<milliseconds>(dur).count()
				<< " ms"s << std::endl;
		std::cerr << name_ << ": "s << duration_cast<microseconds>(dur).count()
				<< " us"s << std::endl;
	}

private:
	std::string name_;
	std::ostream& os_;
	const Clock::time_point start_time_ = Clock::now();
};

#define PROFILE_CONCAT_INTERNAL(X, Y) X ## Y
#define PROFILE_CONCAT(X, Y) PROFILE_CONCAT_INTERNAL(X, Y)
#define UNIQUE_VAR_NAME_PROFILE PROFILE_CONCAT(profileGuard, __LINE__)
#define LOG_DURATION(x) LogDuration UNIQUE_VAR_NAME_PROFILE(x, std::cerr)
#define LOG_DURATION_STREAM(x, os) LogDuration UNIQUE_VAR_NAME_PROFILE(x, os)
