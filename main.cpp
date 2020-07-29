#include <iostream>
#include <string>

#include <boost/outcome/outcome.hpp>
#include <boost/algorithm/hex.hpp>

#include <windows.h>

// https://www.boost.org/doc/libs/1_73_0/libs/outcome/doc/html/tutorial/essential.html
//
// Boost example:
//
//outcome::result<void> print_half(const std::string& text)
//{
//	if (outcome::result<int> r = convert(text))     // #1
//	{
//		std::cout << (r.value() / 2) << std::endl;    // #2
//	}
//	else
//	{
//		if (r.error() == ConversionErrc::TooLong)     // #3
//		{
//			BOOST_OUTCOME_TRY(i, BigInt::fromString(text));  // #4
//			std::cout << i.half() << std::endl;
//		}
//		else
//		{
//			return r.as_failure();                      // #5
//		}
//	}
//	return outcome::success();                      // #6
//}


template<typename T>
using Result = boost::outcome_v2::std_result<T>;

HRESULT FoobarHRESULT(bool askFailure, std::string& msg)
{
	if (askFailure)
	{
		return HRESULT_FROM_WIN32(ERROR_ACCESS_DENIED);
	}

	msg = "Foobar message";
	return 0;
}

Result<std::string> Foobar(bool askFailure)
{
	std::string msg;
	auto hr = FoobarHRESULT(askFailure, msg);
	if (FAILED(hr))
	{
		return { hr, std::system_category() };
	}

	return msg;
}

void Test(bool askFailure)
{
	auto msg = Foobar(askFailure);
	std::cout << "Foobar - askFailure: " << askFailure;
	if (!msg)
	{
		std::cout << ", error: '" << msg.error().message() << "'" << std::endl;
	}
	else
	{
		std::cout << ", value: '" << msg.value() << "'" << std::endl;
	}
}

int main(int argc, wchar_t* argv[])
{
	Test(true);
	Test(false);

	// warning 'nodiscard'
	//Foobar(true);

	// ok
	//auto msg = Foobar(true);

	// abort: default policy throw exception but custom policy is possible
	// auto msg = Foobar(true);
	// std::cout << msg.value() << std::endl;

	return 0;
}

// output:
// Foobar - askFailure: 1, error : 'Access is denied.'
// Foobar - askFailure : 0, value : 'Foobar message'
