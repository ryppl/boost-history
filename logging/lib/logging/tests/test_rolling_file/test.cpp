/* 
    Tests rolling file
*/

#include <boost/logging/logging.hpp>

int main() {
    write_to_clean_rolling_file();
    write_to_existing_rolling_file();
    test_contents();
    write_to_too_full_rolling_file();
    test_contents_after_writing_to_full_rolling_file();
}
