add_executable(horizon_test
        main_case.cpp)

target_link_libraries(horizon_test PUBLIC kernel_core cpu_core Catch2::Catch2WithMain)
add_test(horizon_test horizon_test --durations yes)

