#pragma once

#include <atomic>
#include <cstdint>

#include "DoomsdayDate.h"
#include "activities/Activity.h"

class DoomsdayActivity final : public Activity {
  doomsday::Date date_{1600, 1, 1};
  std::atomic<uint32_t> startedAt_{0};
  uint32_t elapsedMs_ = 0;
  uint64_t totalCorrectMs_ = 0;
  uint16_t correct_ = 0;
  uint16_t incorrect_ = 0;
  bool revealed_ = false;
  bool rated_ = false;

  void nextDate();
  void rate(bool correct);

 public:
  DoomsdayActivity(GfxRenderer& renderer, MappedInputManager& mappedInput)
      : Activity("Doomsday", renderer, mappedInput) {}
  void onEnter() override;
  void loop() override;
  void render(RenderLock&&) override;
};
