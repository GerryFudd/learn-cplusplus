#ifndef APPENDABLE_TYPE
#define APPENDABLE_TYPE
#include <string>
#include <exception_utils/enriched_exception.hpp>

using namespace std;

namespace dexenjaeger {
    namespace array_utils {
        class NullArrayException: public exception_utils::enriched_exception {
        public:
            NullArrayException(const char * message);
            NullArrayException(string message);
        };

        template <class itemType>
        class appendable {
            itemType * items;
            unsigned short count;
        public:
            appendable(): items{nullptr}, count{0} {};
            ~appendable() {
                if (count > 0) {
                    delete[] items;
                }
            };
            unsigned short length();
            void append(itemType);
            itemType get(int);
        };
        
        template <class itemType>
        unsigned short appendable<itemType>::length() {
            return count;
        }

        template <class itemType>
        void appendable<itemType>::append(itemType item) {
            itemType * updated_items = new itemType[count + 1];
            for (int i = 0; i < count; i++) {
                updated_items[i] = items[i];
            }
            updated_items[count] = item;

            if (count > 0) {
                delete[] items;
            }
            items = updated_items;
            count++;
        }

        template <class itemType>
        itemType appendable<itemType>::get(int i) {
            if (count == 0) {
                throw NullArrayException(string("Unable to get items from an empty array."));
            }
            return items[i % count];
        }
    }
}
#endif
