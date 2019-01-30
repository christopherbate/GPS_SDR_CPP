#include "QuickTestCPP.h"

#include "Log.hpp"

int main() {
    TestRunner::GetRunner()->AddTest("Log", "Can log log messages.", []() {
        Log::MSG("Test 1");
        Log::ERR("Test - not really an error.");
        return 1;
    });

    TestRunner::GetRunner()->Run();

    TestRunner::GetRunner()->PrintSummary();

    return TestRunner::GetRunner()->GetRetCode();
}