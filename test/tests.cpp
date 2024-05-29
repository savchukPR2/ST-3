// Copyright 2024 Savchuk Anton

#include "TimedDoor.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

class MockTimerClient : public TimerClient {
 public:
    MOCK_METHOD(void, Timeout, (), (override));
};

class TimedDoorTest : public ::testing::Test {
 protected:
    TimedDoorTest() : door(5) {}

    void SetUp() override {
        door.lock();
    }

    void TearDown() override {
        door.lock();
    }

    TimedDoor door;
};

TEST_F(TimedDoorTest, DoorStartsClosed) {
    ASSERT_FALSE(door.isDoorOpened());
}

TEST_F(TimedDoorTest, DoorUnlocks) {
    door.unlock();
    ASSERT_TRUE(door.isDoorOpened());
}

TEST_F(TimedDoorTest, DoorLocks) {
    door.unlock();
    door.lock();
    ASSERT_FALSE(door.isDoorOpened());
}

TEST_F(TimedDoorTest, TimerThrowsExceptionWithDoorStillOpen) {
    DoorTimerAdapter adapter(door);
    Timer timer;

    door.unlock();

    ASSERT_THROW(timer.tregister(door.getTimeOut(), &adapter), const char*);
}

TEST_F(TimedDoorTest, TimerDoesNotThrowExceptionWhenDoorIsLocked) {
    DoorTimerAdapter adapter(door);
    Timer timer;

    ASSERT_NO_THROW(timer.tregister(door.getTimeOut(), &adapter));
}

TEST_F(TimedDoorTest, TimerDoesNotThrowExceptionWhenDoorIsAlreadyClosed) {
    DoorTimerAdapter adapter(door);
    Timer timer;

    ASSERT_NO_THROW(timer.tregister(door.getTimeOut(), &adapter));
}

TEST_F(TimedDoorTest, TimerDoesNotThrowExceptionWhenDoorIsClosedBeforeTimeout) {
    DoorTimerAdapter adapter(door);
    Timer timer;

    door.unlock();
    door.lock();

    ASSERT_NO_THROW(timer.tregister(door.getTimeOut(), &adapter));
}

TEST_F(TimedDoorTest, TimerThrowsExceptionWhenDoorIsStillOpenAfterTimeout) {
    DoorTimerAdapter adapter(door);
    Timer timer;

    door.unlock();

    sleep(door.getTimeOut() + 1);
    ASSERT_THROW(adapter.Timeout(), const char*);
}

TEST_F(TimedDoorTest,
TimerDoesNotThrowExceptionWhenDoorIsClosedBeforeTimeoutButStillUnlocked) {
    DoorTimerAdapter adapter(door);
    Timer timer;

    door.unlock();
    door.lock();

    sleep(door.getTimeOut() + 1);
    ASSERT_NO_THROW(adapter.Timeout());
}

TEST_F(TimedDoorTest, TimerDoesNotThrowExceptionWhenDoorIsLockedBeforeTimeout) {
    DoorTimerAdapter adapter(door);
    Timer timer;

    door.lock();

    ASSERT_NO_THROW(timer.tregister(door.getTimeOut(), &adapter));
}
