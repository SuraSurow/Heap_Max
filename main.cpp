#include <iostream>
#include <random>
#include <sstream>

int randValue(int min, int max) {
    std::random_device rd;
    std::seed_seq seed{rd(), rd(), rd(), rd()};
    std::mt19937 random_engine(seed);
    std::uniform_int_distribution<int> distribution(min, max);

    return distribution(random_engine);
}

struct Obj {
    int num;

    Obj ( ) : num(0) {};

    Obj ( int n ) : num(n) {}

    static int compare ( Obj *o1, Obj *o2 ) {
        if ( o1->num > o2->num ) {
            return 1;
        } else if ( o1->num < o2->num ) {
            return -1;
        } else {
            return 0;
        }
    }

    int operator->() {
        return num;
    }
    };

    typedef int(*compareObj) ( Obj *, Obj * );


    template<typename Data_type>
    struct Heap {
        class Dynamic_Array {
        public:
            Data_type *array;
            unsigned int currentSize;
            unsigned int maxSize;
            float growthRate;

            Dynamic_Array ( ) : currentSize(0), maxSize(1), growthRate(2.0) {
                array = new Data_type[maxSize];
            }

            ~Dynamic_Array ( ) {
                delete array;
            }

            void addObj ( Data_type *obj ) {
                if ( currentSize == maxSize ) {
                    maxSize *= growthRate;
                    Data_type *newArray = new Data_type[maxSize];
                    for ( unsigned int i = 0 ; i < currentSize ; i++ ) {
                        newArray[ i ] = array[ i ];
                    }
                    delete[] array;
                    array = newArray;
                }
                array[ currentSize ] = *obj;
                currentSize++;
            }

            Data_type getObj ( unsigned int indeks ) const {
                if ( indeks > currentSize ) {
                    std::cerr << "\n'getObj(" << indeks << ") indeks wiekszy od tablicy!!\n";
                    return nullptr;
                }
                return array[ indeks ];
            }

            bool clear ( ) {
                while ( currentSize > 0 ) {
                    Data_type currentToDelete = array[ currentSize - 1 ];
                    delete currentToDelete;
                    currentSize--;
                }
                maxSize = 1;
                return true;
            }

            void removeObj(unsigned int indeks) {
                if (indeks < currentSize) {
                    for (unsigned int i = indeks; i < currentSize - 1; i++) {
                        array[i] = array[i + 1];
                    }
                    currentSize--;
                } else {
                    std::cerr << "Nieprawidłowy indeks." << std::endl;
                }
            }

            void switchObj ( unsigned int indexOne, unsigned int indexTwo ) {
                if ( indexOne < currentSize && indexTwo < currentSize ) {
                    Data_type temp = array[ indexOne ];
                    array[ indexOne ] = array[ indexTwo ];
                    array[ indexTwo ] = temp;
                } else {
                    std::cerr << "Nieprawidłowy indeks." << std::endl;
                }
            }

        };

        Dynamic_Array array;
    private:


    public:
        void Insert ( Data_type data ) {
            array.addObj(&data);
            unsigned int currentIndex = array.currentSize - 1;
            while ( currentIndex > 0 ) {
                unsigned int parentIndex = ( currentIndex - 1 ) / 2;
                compareObj cmp = Obj::compare;
                if (cmp(data, array.getObj(parentIndex)) >= 0) {
                    // Jeśli nowy element jest większy lub równy rodzicowi, zamień je
                    array.switchObj(currentIndex, parentIndex);
                    currentIndex = parentIndex;
                } else {
                    break;
                }
            }
        }

        std::string toString() const {
                std::stringstream result;
                result << "Heap: ";

                for (unsigned int i = 0; i < array.currentSize; ++i) {
                    result << array.getObj(i)->num << " ";
                }

                return result.str();
            }

        Data_type PopMax ( ) {
            if ( array.currentSize == 0 ) {
                return nullptr;
            } else if ( array.currentSize == 1 ) {
                array.currentSize = 0 ;
                Data_type MaxObj = array.getObj(0);
                array.removeObj(0);
                return MaxObj;
            }
            else{
                Data_type MaxObj = array.getObj(0);
                unsigned int lastObj = array.currentSize-1 ;
                array.switchObj(0,lastObj);
                array.currentSize--;
                HeapifyDown(0);
                return MaxObj;
            }
        }

        void Clear ( ) {
            array.clear();
        }

        void HeapifyUp(unsigned int index) {
            while (index > 0) {
                unsigned int parentIndex = (index - 1) / 2;

                if (Obj::compare(array.getObj(index), array.getObj(parentIndex)) == 1) {
                    array.switchObj(index, parentIndex);
                    index = parentIndex;
                } else {
                    break;
                }
            }
        }

        void HeapifyDown(unsigned int index) {
            bool change = true;
            while (change) {
                unsigned int left = 2 * index + 1;
                unsigned int right = 2 * index + 2;
                unsigned int heapify = index;

                if (left < array.currentSize && Obj::compare(array.getObj(left), array.getObj(heapify)) == 1) {
                    heapify = left;
                }

                if (right < array.currentSize && Obj::compare(array.getObj(right), array.getObj(heapify)) == 1) {
                    heapify = right;
                }

                if (heapify != index) {
                    array.switchObj(index, heapify);
                    index = heapify;
                } else {
                    change = false;
                }
            }
        }


    };


int main() {
    Heap<Obj *> *heap = new Heap<Obj *>();

    clock_t startInsert = clock();

    for (int i = 0; i < 1000000; i++) {
        Obj *obj = new Obj(randValue(0, 1000000));
        heap->Insert(obj);
    }

    clock_t stopInsert = clock();
    double durationInsert = (double)(stopInsert - startInsert) / CLOCKS_PER_SEC * 1000.0;

    clock_t startPop = clock();

    while (heap->array.currentSize > 1) {
        Obj *obiekt = heap->PopMax();
        delete obiekt;
    }

    clock_t stopPop = clock();
    double durationPop = (double)(stopPop - startPop) / CLOCKS_PER_SEC * 1000.0;

    std::cout << "Insert Time: " << durationInsert << " ms\t"
              << "Pop Time: " << durationPop << " ms\t" << std::endl;

    delete heap;

    return 0;
}






