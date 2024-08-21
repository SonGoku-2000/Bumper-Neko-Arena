#pragma once
#include "bn_vector.h"
#include "bn_random.h"
namespace bna {

    template<typename Type, int MaxSize>
    class vector :public bn::vector<Type, MaxSize> {
        public:
        vector() :bn::vector<Type, MaxSize>() {}
        ~vector() = default;
        void pop(int index) {
            this->erase(this->begin() + index);
        }

        void pop_pointer(int index) {
            delete (*this)[index];
            this->erase(this->begin() + index);
        }

        void shuffle() {
            static bn::random rand;
            for (int i = this->size() - 1; i > 0; --i) {
                int j = rand.get_int(0, i);
                bn::swap((*this)[i], (*this)[j]);
            }
        }
    };
} // namespace loz
