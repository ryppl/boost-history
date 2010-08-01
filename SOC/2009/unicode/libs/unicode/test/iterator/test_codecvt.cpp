//[ test_codecvt
/*`
This test/example shows how to use a codecvt facet that transcodes from
wide chars (UTF-16 or UTF-32) to UTF-8 on the way out, and that
does the opposite on the way in, but normalizes the string as well.
*/
#define BOOST_TEST_MODULE Codecvt
#include <boost/test/included/unit_test.hpp>

#include <boost/unicode/codecvt.hpp>

#include <fstream>
#include <boost/range/algorithm.hpp>
#include <boost/range/as_literal.hpp>


BOOST_AUTO_TEST_CASE( codecvt )
{
    // e\u0301 is \u00E9
    // \U0002FA1D is \U0002A600
    const wchar_t data_[] = L"hello e\u0301 \U0002FA1D world";
    boost::iterator_range<const wchar_t*> data = boost::as_literal(data_);
    
    const wchar_t data_normalized_[] = L"hello \u00E9 \U0002A600 world";
    boost::iterator_range<const wchar_t*> data_normalized = data;//boost::as_literal(data_normalized_);

    std::locale old_locale;
    std::locale utf8_locale(old_locale, new boost::unicode::utf_u8_codecvt());

    // Set a new global locale
    //std::locale::global(utf8_locale);

    // Send the UTF-X data out, converting to UTF-8
    {
        std::wofstream ofs("data.ucd");
        ofs.imbue(utf8_locale);
        boost::copy(data, std::ostream_iterator<wchar_t, wchar_t>(ofs));
    }

    // Read the UTF-8 data back in, converting to UTF-X and normalizing on the way in
    {
        std::wifstream ifs("data.ucd");
        ifs.imbue(utf8_locale);
        wchar_t item = 0;
        size_t i = 0;
        while (ifs >> std::noskipws >> item)
        {
            BOOST_CHECK_EQUAL(data_normalized[i], item);
            i++;
        }
        BOOST_CHECK_EQUAL(i, (size_t)boost::size(data_normalized));
    }
}
//]
