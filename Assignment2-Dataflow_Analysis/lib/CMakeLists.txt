add_library(DFA SHARED DFA.cpp
                       1-AvailExprs.cpp
                       2-Liveness.cpp
                       3-SCCP.cpp
                       4-LCM/1-AnticipatedExprs.cpp
                       4-LCM/2-WBAvailExprs.cpp
                       4-LCM/3-EarliestPlacement.cpp
                       4-LCM/4-PostponableExprs.cpp
                       4-LCM/5-LatestPlacement.cpp
                       4-LCM/6-UsedExprs.cpp
                       DFA/Domain/Expression.cpp
                       DFA/Domain/Variable.cpp)
