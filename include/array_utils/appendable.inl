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

        /*
            This class holds a dynamically sized array of the parameterized type. It will persist a new
            array in memory if more items are appended than the current array can hold. Each of these
            arrays are cleaned up when either a new items array is created or the class instance is
            destructed.
        */
        template <class itemType>
        class appendable {
            itemType * items;
            unsigned short count;
            unsigned short array_size;
        public:
            /*
                This constructor allocates some amount of memory to the array so appending won't incur
                the overhead of creating and deleting dynamic memory for the initial appends.
            */
            appendable(unsigned short size): items{new itemType[size]}, count{0}, array_size{size} {};
            appendable(): appendable{0} {};
            ~appendable() {
                delete[] items;
            };
            unsigned short length();
            unsigned short size();
            void append(itemType);
            itemType get(int);
        };
        
        template <class itemType>
        unsigned short appendable<itemType>::length() {
            return count;
        }
        template <class itemType>
        unsigned short appendable<itemType>::size() {
            return array_size;
        }

        template <class itemType>
        void appendable<itemType>::append(itemType item) {
            if (count >= array_size) {
                array_size = count + 1;
                itemType * updated_items = new itemType[array_size];
                for (int i = 0; i < count; i++) {
                    updated_items[i] = items[i];
                }

                delete[] items;
                items = updated_items;
            }
            items[count] = item;
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
