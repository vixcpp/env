function(vix_env_add_test test_name test_source)
  add_executable(${test_name}
    ${test_source}
  )

  target_compile_features(${test_name}
    PRIVATE
      cxx_std_20
  )

  target_link_libraries(${test_name}
    PRIVATE
      vix::env
  )

  if (VIX_ENABLE_SANITIZERS AND NOT MSVC)
    target_compile_options(${test_name}
      PRIVATE
        -fno-omit-frame-pointer
        -fsanitize=address,undefined
    )

    target_link_options(${test_name}
      PRIVATE
        -fsanitize=address,undefined
    )
  endif()

  add_test(
    NAME ${test_name}
    COMMAND ${test_name}
  )
endfunction()

vix_env_add_test(vix_env_get_test get_test.cpp)
vix_env_add_test(vix_env_get_bool_test get_bool_test.cpp)
vix_env_add_test(vix_env_get_int_test get_int_test.cpp)
vix_env_add_test(vix_env_get_uint_test get_uint_test.cpp)
vix_env_add_test(vix_env_get_double_test get_double_test.cpp)
vix_env_add_test(vix_env_has_test has_test.cpp)
vix_env_add_test(vix_env_set_test set_test.cpp)
vix_env_add_test(vix_env_unset_test unset_test.cpp)
vix_env_add_test(vix_env_parse_line_test parse_line_test.cpp)
vix_env_add_test(vix_env_parse_content_test parse_content_test.cpp)
vix_env_add_test(vix_env_load_file_test load_file_test.cpp)
vix_env_add_test(vix_env_load_layered_test load_layered_test.cpp)
vix_env_add_test(vix_env_resolve_env_file_test resolve_env_file_test.cpp)
