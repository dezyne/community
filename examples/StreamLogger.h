// tested on MSVC2013/2017 and GCC7.2

#include <array>
#include <functional>
#include <streambuf>

template <class Elem, class Tr = std::char_traits<Elem>,
	class Alloc = std::allocator<Elem>>
	class StreamLoggerBuf : public std::basic_streambuf<Elem, Tr>
{
	using LogFunction = std::function<void(const Elem*)>;
	using _int_type = typename std::basic_streambuf<Elem, Tr>::int_type;
	using _traits_type = typename std::basic_streambuf<Elem, Tr>::traits_type;
public:
	explicit StreamLoggerBuf(LogFunction f) :
		m_buf(),
		m_index(0),
		m_logFunction(f) {}

	StreamLoggerBuf(const StreamLoggerBuf&) = delete;

protected:
	int sync() override
	{
		if (m_index == 0)
			return 0;
		return internalSync();
	}

	_int_type overflow(_int_type c) override
	{
		if (c == _traits_type::eof())
			return 0;

		if (c == '\n')
		{
			internalSync();
		}
		else
		{
			m_buf[m_index] = _traits_type::to_char_type(c);
			m_index++;
			if (m_index == (m_bufferSize - 1))
			{
				internalSync();
			}
		}
		return c;
	}

private:
	int internalSync()
	{
		m_buf[m_index] = '\0';
		m_index = 0;
		m_logFunction(m_buf.data());
		return 0;
	}

	static const int m_bufferSize = 1024;
	std::array<Elem, m_bufferSize> m_buf;
	int m_index;

	LogFunction m_logFunction;
};

template <class Elem, class Tr = std::char_traits<Elem>>
class StreamLogger_stream : public std::basic_ostream<Elem, Tr>
{
	using LogFunction = std::function<void(const Elem*)>;

public:
	explicit StreamLogger_stream(LogFunction f) :
		std::basic_ostream<Elem, Tr>(&buf),
		buf(f) {}

private:
	StreamLoggerBuf<Elem, Tr> buf;
};

using StreamLogger = StreamLogger_stream<char>;
using WStreamLogger = StreamLogger_stream<wchar_t>;

#include <iostream>

int main()
{
	auto f = [](const char* message) {
		std::cout << message << "\n";
	};
	auto wf = [](const wchar_t* message) {
		std::wcout << message << L"\n";
	};

	StreamLogger log(f);
	log << "char * test1 ";
	log << "char * test2\n";

	WStreamLogger wlog(wf);
	wlog << L"wchar_t test1 ";
	wlog << L"wchar_t test2\n";
	return 0;
}
