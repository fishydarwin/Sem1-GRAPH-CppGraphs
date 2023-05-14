//
// Created by Rares Bozga on 14.05.2023.
//

#include "activity.h"

Activity::Activity() {
    this->activityId = -1;
    this->requiredActivities = vector<int>();
    this->startTime = tm {};
    this->durationMinutes = 0;
}

string Activity::toString() { // golly what a function
    string output = "Activity #" + to_string(activityId) + "; Requires: ";
    for (const int &i : requiredActivities)
        output += to_string(i) + " ";
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%d.%m.%Y-%H:%M:%S", &startTime);
    std::string timeString(buffer);
    output += "; Start: " + timeString + "; Duration: " + to_string(durationMinutes) + " minutes.";
    return output;
}
