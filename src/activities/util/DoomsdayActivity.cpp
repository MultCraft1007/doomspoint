#include "DoomsdayActivity.h"

#include <Arduino.h>
#include <GfxRenderer.h>
#include <I18n.h>
#include <esp_system.h>

#include <cstdio>

#include "MappedInputManager.h"
#include "components/UITheme.h"
#include "fontIds.h"

namespace {
constexpr uint16_t FIRST_YEAR = 1600;
constexpr uint16_t LAST_YEAR = 2099;
constexpr StrId WEEKDAYS[] = {StrId::STR_DOOMSDAY_SUNDAY,   StrId::STR_DOOMSDAY_MONDAY,
                               StrId::STR_DOOMSDAY_TUESDAY,  StrId::STR_DOOMSDAY_WEDNESDAY,
                               StrId::STR_DOOMSDAY_THURSDAY, StrId::STR_DOOMSDAY_FRIDAY,
                               StrId::STR_DOOMSDAY_SATURDAY};
}  // namespace

void DoomsdayActivity::nextDate() {
  const uint16_t year = FIRST_YEAR + esp_random() % (LAST_YEAR - FIRST_YEAR + 1);
  date_ = doomsday::dateFromDay(year, esp_random() % doomsday::daysInYear(year));
  startedAt_.store(0);
  elapsedMs_ = 0;
  revealed_ = false;
  rated_ = false;
  requestUpdate();
}

void DoomsdayActivity::onEnter() {
  Activity::onEnter();
  nextDate();
}

void DoomsdayActivity::rate(const bool correct) {
  if (!revealed_ || rated_) return;
  rated_ = true;
  if (correct) {
    ++correct_;
    totalCorrectMs_ += elapsedMs_;
  } else {
    ++incorrect_;
  }
  requestUpdate();
}

void DoomsdayActivity::loop() {
  if (mappedInput.wasReleased(MappedInputManager::Button::Back)) {
    activityManager.goHome(HomeMenuItem::DOOMSDAY);
    return;
  }
  if (mappedInput.wasReleased(MappedInputManager::Button::Confirm)) {
    if (!revealed_) {
      const uint32_t startedAt = startedAt_.load();
      if (startedAt == 0) return;
      elapsedMs_ = millis() - startedAt;
      revealed_ = true;
      requestUpdate();
    } else if (rated_) {
      nextDate();
    }
    return;
  }
  if (revealed_ && !rated_) {
    if (mappedInput.wasReleased(MappedInputManager::Button::NavPrevious)) rate(false);
    if (mappedInput.wasReleased(MappedInputManager::Button::NavNext)) rate(true);
  }
  int x = 0;
  int y = 0;
  if (!mappedInput.wasScreenTapped(x, y)) return;
  if (!revealed_) {
    const uint32_t startedAt = startedAt_.load();
    if (startedAt == 0) return;
    elapsedMs_ = millis() - startedAt;
    revealed_ = true;
    requestUpdate();
  } else if (!rated_) {
    rate(x >= renderer.getScreenWidth() / 2);
  } else {
    nextDate();
  }
}

void DoomsdayActivity::render(RenderLock&&) {
  renderer.clearScreen();
  const auto& metrics = UITheme::getInstance().getMetrics();
  const int width = renderer.getScreenWidth();
  const int height = renderer.getScreenHeight();
  GUI.drawHeader(renderer, Rect{0, metrics.topPadding, width, metrics.headerHeight}, tr(STR_DOOMSDAY_TITLE));

  char dateText[24];
  snprintf(dateText, sizeof(dateText), "%02u/%02u/%04u", static_cast<unsigned>(date_.day),
           static_cast<unsigned>(date_.month), static_cast<unsigned>(date_.year));
  renderer.drawCenteredText(UI_12_FONT_ID, height / 3, dateText, true, EpdFontFamily::BOLD);

  if (revealed_) {
    renderer.drawCenteredText(UI_12_FONT_ID, height / 2, I18N.get(WEEKDAYS[doomsday::weekday(date_)]));
    char anchor[64];
    snprintf(anchor, sizeof(anchor), "%s: %s", tr(STR_DOOMSDAY_ANCHOR),
             I18N.get(WEEKDAYS[doomsday::weekday({date_.year, 4, 4})]));
    renderer.drawCenteredText(UI_10_FONT_ID, height / 2 + 38, anchor);
    char timeText[32];
    snprintf(timeText, sizeof(timeText), "%s: %lu.%02lus", tr(STR_DOOMSDAY_TIME),
             static_cast<unsigned long>(elapsedMs_ / 1000), static_cast<unsigned long>((elapsedMs_ % 1000) / 10));
    renderer.drawCenteredText(UI_10_FONT_ID, height / 2 + 70, timeText);
    if (!rated_) renderer.drawCenteredText(UI_10_FONT_ID, height / 2 + 105, tr(STR_DOOMSDAY_RATE_HINT));
  } else {
    renderer.drawCenteredText(UI_10_FONT_ID, height / 2, tr(STR_DOOMSDAY_QUESTION));
  }

  if (correct_ + incorrect_ > 0) {
    char stats[48];
    snprintf(stats, sizeof(stats), "%s: %u   %s: %u", tr(STR_DOOMSDAY_CORRECT), static_cast<unsigned>(correct_),
             tr(STR_DOOMSDAY_WRONG), static_cast<unsigned>(incorrect_));
    renderer.drawCenteredText(UI_10_FONT_ID, height - metrics.buttonHintsHeight - 75, stats);
    if (correct_ > 0) {
      const uint32_t average = static_cast<uint32_t>(totalCorrectMs_ / correct_);
      snprintf(stats, sizeof(stats), "%s: %lu.%02lus", tr(STR_DOOMSDAY_AVERAGE),
               static_cast<unsigned long>(average / 1000), static_cast<unsigned long>((average % 1000) / 10));
      renderer.drawCenteredText(UI_10_FONT_ID, height - metrics.buttonHintsHeight - 45, stats);
    }
  }

  const char* confirm = !revealed_ ? tr(STR_DOOMSDAY_REVEAL) : rated_ ? tr(STR_DOOMSDAY_NEXT) : "";
  const char* up = revealed_ && !rated_ ? tr(STR_DOOMSDAY_WRONG) : "";
  const char* down = revealed_ && !rated_ ? tr(STR_DOOMSDAY_CORRECT) : "";
  const auto labels = mappedInput.mapLabels(tr(STR_BACK), confirm, up, down);
  GUI.drawButtonHints(renderer, labels.btn1, labels.btn2, labels.btn3, labels.btn4);
  renderer.displayBuffer();
  if (!revealed_ && startedAt_.load() == 0) startedAt_.store(millis());
}
