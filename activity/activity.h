//
// Created by Rares Bozga on 14.05.2023.
//

#pragma once

#include <vector>
#include <string>
#include <chrono>

using namespace std;

class Activity {
public:
    int activityId;
    vector<int> requiredActivities;
    int duration;

    Activity();
    string toString();
};
