#include "../../src/file/jsonReader.h"
#include <vector>
#include <cassert>
#include <filesystem>
using namespace std;

int main () {
    for (const auto & wrongFile : filesystem::directory_iterator(filesystem::current_path().string() + "/examples/tests/eventsJSON/wrong")) {
        try {
            CJSONReader reader(wrongFile.path());
            reader.readAllEvents();
            reader.close();
            assert(false);
        } catch (...) {
        }
    }
    for (const auto & correctFile : filesystem::directory_iterator(filesystem::current_path().string() + "/examples/tests/eventsJSON/correct")) {
        try {
            CJSONReader reader(correctFile.path());
            reader.readAllEvents();
            reader.close();
        } catch (...) {
            assert(false);
        }
    }
}