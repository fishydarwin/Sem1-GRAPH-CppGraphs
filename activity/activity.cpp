//
// Created by Rares Bozga on 14.05.2023.
//

#include "activity.h"

Activity::Activity() {
    this->activityId = -1;
    this->requiredActivities = vector<int>();
    this->duration = 0;
}

string Activity::toString() { // golly what a function
    string output = "Activity " + string(1, char('A' + activityId)) + "; ";
    if (!requiredActivities.empty()) {
        output += "Requires: ";
        for (const int &i : requiredActivities)
            output += to_string(i) + " ";
        output += "; ";
    } else {
        output += "No requirements; ";
    }
    output += "Duration: " + to_string(duration);
    return output;
}
