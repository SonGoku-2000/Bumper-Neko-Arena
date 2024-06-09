#include "bna_test_map.hpp"

bna::TestMap::TestMap(){
    _size = bn::size(300,300);
}

bn::size bna::TestMap::getSize(){
return _size;
}