#pragma once

#ifdef max
	#undef max
#endif
#ifdef min
	#undef min 
#endif

#include <algorithm>
#include <random>
#include <string>

namespace utils {

    class RandomGenerator {
    public:
        static RandomGenerator& GetInstance() {
            static RandomGenerator Generator;
            return Generator;
        }

        RandomGenerator(const RandomGenerator&) = delete;
        RandomGenerator(RandomGenerator&&) = delete;
        RandomGenerator operator=(const RandomGenerator&) = delete;
        RandomGenerator operator=(RandomGenerator&&) = delete;

        int GetInt(int Min = std::numeric_limits<int>::min(), int Max = std::numeric_limits<int>::max()) {
            std::uniform_int_distribution<int> Distribution(Min, Max);
            return Distribution(Engine);
        }

        double GetDouble(double Min = std::numeric_limits<double>::min(), double Max = std::numeric_limits<double>::max()) {
            std::uniform_real_distribution<double> Distribution(Min, Max);
            return Distribution(Engine);
        }

        std::string GetString(
            size_t Length,
            const std::string& Chars =
            "abcdefghijklmnopqrstuvwxyz"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "0123456789"
        ) {
            std::string Buffer(Length, '\0');
            std::generate(Buffer.begin(), Buffer.end(), [&] { return Chars[GetInt(0, Chars.size() - 1)]; });
            return Buffer;
        }

        std::vector<char> GetBytes(size_t Length) {
            std::vector<char> Buffer(Length);
            std::generate(Buffer.begin(), Buffer.end(), [&] { return GetInt(0, 255); });
            return Buffer;
        }

    private:
        RandomGenerator(unsigned int Seed = std::random_device{}()) : Engine(Seed) {}

        std::mt19937 Engine;
    };

} // namespace utils
