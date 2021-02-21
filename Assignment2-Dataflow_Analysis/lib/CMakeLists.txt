add_library(DFA SHARED AvailExpr.cpp Liveness.cpp
                       LCM/1-AntiExpr.cpp LCM/2-WBAvailExpr.cpp
                       LCM/3-EPlace.cpp)
