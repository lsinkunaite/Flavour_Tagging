# Executables
add_executable(tuple tuple.cpp)
add_executable(histograms histograms.cpp)
add_executable(tuple3 tuple3.cpp)
add_executable(mass mass.cpp)
add_executable(time time.cpp)
add_executable(general general.cpp)
add_library(background background.cpp background.h)
add_executable(roo_fit roo_fit.cpp)
add_executable(generaltime generaltime.cpp)
add_library(etatime etatime.cpp etatime.h)
add_executable(generaltimeb generaltimeb.cpp)
add_library(etatimeb etatimeb.cpp etatimeb.h)
add_executable(etacattime etacattime.cpp)
add_executable(etareftime etareftime.cpp)
add_executable(etareftimeroo etareftimeroo.cpp)
add_executable(etareftime_nsw etareftime_nsw.cpp)
add_library(etaroof etaroof.cpp etaroof.h)
add_executable(etareftimef etareftimef.cpp)

# Aliases for module libs

# Linker information
target_link_libraries(tuple ${ROOT_LIBRARIES})
target_link_libraries(histograms ${ALL_LIBRARIES})
target_link_libraries(tuple3 ${ALL_LIBRARIES})
target_link_libraries(mass ${ALL_LIBRARIES})
target_link_libraries(time ${ALL_LIBRARIES})
target_link_libraries(general background ${ALL_LIBRARIES})
target_link_libraries(roo_fit ${ALL_LIBRARIES})
target_link_libraries(generaltime etatime ${ALL_LIBRARIES})
target_link_libraries(generaltimeb etatimeb ${ALL_LIBRARIES})
target_link_libraries(etacattime ${ALL_LIBRARIES})
target_link_libraries(etareftime ${ALL_LIBRARIES})
target_link_libraries(etareftime_nsw ${ALL_LIBRARIES})
target_link_libraries(etareftimeroo ${ALL_LIBRARIES})
target_link_libraries(etareftimef etaroof ${ALL_LIBRARIES})
