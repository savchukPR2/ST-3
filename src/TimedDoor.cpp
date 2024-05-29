// Copyright 2024 Savchuk Anton

#include "TimedDoor.h"

DoorTimerAdapter::DoorTimerAdapter(TimedDoor& door) : door(door) {}

void DoorTimerAdapter::Timeout() {
    door.throwState();
}

TimedDoor::TimedDoor(int timeout) : iTimeout(timeout),
isOpened(false), adapter(nullptr) {}

bool TimedDoor::isDoorOpened() {
    return isOpened;
}

void TimedDoor::unlock() {
    isOpened = true;
    if (adapter) {
        Timer timer;
        timer.tregister(iTimeout, adapter);
    }
}

void TimedDoor::lock() {
    isOpened = false;
}

int TimedDoor::getTimeOut() const {
    return iTimeout;
}

void TimedDoor::throwState() {
    if (isOpened) {
        throw "Door still opened!";
    }
}

void Timer::sleep(int seconds) {}

void Timer::tregister(int seconds, TimerClient* client) {
    this->client = client;
    sleep(seconds);
    client->Timeout();
}
