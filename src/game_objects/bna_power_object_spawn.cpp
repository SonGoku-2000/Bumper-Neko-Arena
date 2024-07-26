#include "bna_power_object_spawn.hpp"

#include "bna_time.hpp"

#include "bna_car_powers_id.hpp"


bna::PowerObjectSpawn::PowerObjectSpawn(bn::fixed_point position) {
    _position = bna::Indicator(position, true);
    _remainingTime = _random.get_int(bna::time::seconds_to_frames(5),bna::time::seconds_to_frames(10));
}

void bna::PowerObjectSpawn::update(){
    if(_remainingTime){
        _remainingTime--;
        return;
    }

    _remainingTime = _random.get_int(bna::time::seconds_to_frames(5),bna::time::seconds_to_frames(10));
    _powerObject = bna::PowerObject(_position,_generateCarPowerId());
}


bna::car_powers_id bna::PowerObjectSpawn::_generateCarPowerId(){
    return bna::car_powers_id(_random.get_int(1,int(bna::car_powers_id::MAX_VALUE)));
}
