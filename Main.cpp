#define _CRT_SECURE_NO_WARNINGS
#define nameof(x)	#x
#define run_timed_test(func, ...) run_timed_test_t(nameof(func), func, __VA_ARGS__)

#include <chrono>		// timers
#include <sstream>		// stringstream
#include <conio.h>		// getch
#include <charconv>		// from_chars

/* Define the amount of iterations here. */
constexpr size_t CNT = 1000000;
/* The result is saved outside of the functions to level the playing field. */
static float result;

/* ASCII digits can easily be converted to their numerical parts by subtracting 48 (ASCII 0) from them. */
inline constexpr uint32_t digit(char c)
{
	return c - '0';
}

/* A digit can only be in the range [0, 9]. */
inline constexpr bool is_digit(char c)
{
	return digit(c) < 10u;
}

/* Anything other than a digit is larger or equal to 10, or underflows the unsigned integer resulting in a large number. */
inline constexpr bool is_not_digit(char c)
{
	return digit(c) >= 10u;
}

bool try_string_to_float(const char * str, float &output)
{
	/* Define result parts. */
	float mantissa = 0.0f, exponent = 0.0f;

	/* Define current specifiers. */
	char sign = '+';

	/* Define reading parameters. */
	int read = 0;
	bool endNotReached = false;

	/* Update sign is needed and check for non digit chars for fast early out. */
	if (*str == '+' || *str == '-')
	{
		sign = *str;
		++str;
	}
	else if (is_not_digit(*str) && *str != '.') return false;

	/* Read integer part. */
	for (endNotReached = *str != '\0'; endNotReached && is_digit(*str); ++str, ++read, endNotReached = *str != '\0')
	{
		mantissa *= 10.0f;
		mantissa += digit(*str);
	}

	/* Read decimal part, string might start with a decimal point (or comma). */
	if (endNotReached && (*str == '.' || *str == ','))
	{
		++str;
		read = 1;

		for (endNotReached = str != '\0'; endNotReached && is_digit(*str); ++str, ++read, endNotReached = str != '\0')
		{
			/* Avoid using math pow, improves precision. */
			constexpr float pow_lut[] = { 1.0f, 0.1f, 0.01f, 0.001f, 0.0001f, 0.00001f, 0.000001f, 0.0000001f };
			constexpr size_t lut_entries = sizeof(pow_lut) / sizeof(float);

			mantissa += digit(*str) * (read < lut_entries ? pow_lut[read] : powf(10.0f, -static_cast<float>(read)));
		}
	}
	else if (read < 1) return false;

	/* Read exponent part. */
	if (endNotReached && (*str == 'e' || *str == 'E'))
	{
		char expSign = '+';

		++str;
		read = 0;
		endNotReached = str != '\0';

		/* Update sign if needed. */
		if (endNotReached && (*str == '+' || *str == '-'))
		{
			expSign = *str;
			++str;
		}
		else if (is_not_digit(*str)) return false;

		for (; endNotReached && is_digit(*str); ++str, ++read, endNotReached = str != '\0')
		{
			exponent *= 10.0f;
			exponent += digit(*str);
		}

		/* Apply exponent sign and check if anything has been read at all. */
		if (read == 0) return false;
		exponent *= (expSign == '+' ? 1.0f : -1.0f);
	}

	/* Assemble result. */
	output = (sign == '+' ? 1.0f : -1.0f) * (exponent ? ldexpf(mantissa * powf(5.0f, exponent), static_cast<int>(exponent)) : mantissa);
	return true;
}

inline float _try_string_to_float(const char *str)
{
	try_string_to_float(str, result);
	return result;
}

inline float _sscanf(const char *str)
{
	sscanf(str, "%f", &result);
	return result;
}

inline float _sscanf_s(const char *str)
{
	sscanf_s(str, "%f", &result);
	return result;
}

inline float _stringstream(const char *str)
{
	std::stringstream stream{ str };
	stream >> result;
	return result;
}

inline float _from_chars(const char *str)
{
	std::from_chars(str, nullptr, result);
	return result;
}

template <typename func_t, typename ... args_t>
void run_timed_test_t(const char *name, func_t function, args_t ... args)
{
	result = -1.0f;
	const std::chrono::time_point start = std::chrono::high_resolution_clock::now();
	
	for (size_t i = 0; i < CNT; i++) result = static_cast<float>(function(args...));

	const std::chrono::time_point end = std::chrono::high_resolution_clock::now();
	const int64_t time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	const int64_t avrg = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / CNT;

	printf("%-20s took %03lldms to complete (%f) convertion (%03lldns average).\n", name, time, result, avrg);
}

int main()
{
	const char *strs[] = 
	{ 
		"",								// Empty string
		"0",							// Single integer
		"0.1",							// Small float
		"0.00000000000000000000111",	// Large float
		"+3.1417e+2",					// Epsilon
		"invalid",						// Should not parse to float
		"-.2",							// No integer part
		"1.797693e+308",				// Floating point overflow
		"-iNF",							// Negative infinity
		"true",							// Different type (could be converted)
		"(1)",							// Encapsulation
		"-+1",							// Double sign specifier
		"0E0",							// Zero with some extra
		"6e777777",						// Same binary value as special value (infinity)
		"1,234",						// Locale change from decimal dot to decimal comma
		"infinity",						// Special value (Infinity)
		"12.34.56",						// Multiple decimal points
		"+1e1^5",						// 10, but in a weird way
		"NaN",							// Special value (Not a Number)
		"1,234.567",					// Digit grouping (APA style)
		"1 234.567",					// Digit grouping (SI style)
		"123,4.567",					// Digit grouping (Chinese)
		"1'234,567",					// Digit grouping (Swiss)
		"1,23,45,678.9"					// Digit grouping (Indian Crore)
	};

	printf("Running performance tests at %zu iterations.\n", CNT);
	puts("A result of '-1' indicates that the function didn't change the output!");

	for (const char *cur : strs)
	{
		printf("-------------------- %s --------------------\n", cur);

		run_timed_test(strtof, cur, nullptr);										// C's string to float with end read position
		run_timed_test(std::strtof, cur, nullptr);									// C++ string to float
		run_timed_test(atof, cur);													// C's default string to double
		run_timed_test_t(nameof(std::from_chars), _from_chars, cur);				// C++17
		run_timed_test_t(nameof(std::stringstream), _stringstream, cur);			// C++ string streams
		run_timed_test_t(nameof(sscanf), _sscanf, cur);								// C's scan string function
		run_timed_test_t(nameof(sscanf_s), _sscanf_s, cur);							// MSVC scan string function
		run_timed_test_t(nameof(try_string_to_float), _try_string_to_float, cur);	// Optimized (based on tryParseDouble from tinyobjloader, syoyo)

		puts("");
	}

	puts("Finished running performance tests, press any key to continue...");
	_getch();
	return 0;
}