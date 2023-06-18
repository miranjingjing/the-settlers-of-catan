#include "observer.h"
#include <unordered_set>

std::unordered_set<Observer*> Subject::get_observers() const {
    return observers;
}

void Subject::notify_observers() { 
    for(auto x : observers) x->notify(*this);
}

void Subject::attach( Observer *o) {
    observers.insert(o);
}

void Subject::detach( Observer *o) {
    observers.erase(o);
}
