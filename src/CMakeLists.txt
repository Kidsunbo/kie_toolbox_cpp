# with_context is True
if(WITH_CONTEXT)
    find_package(Boost REQUIRED)
    target_link_libraries(kie_toolbox INTERFACE boost::boost)
    message(STATUS "WITH_CONTEXT is enabled, context will be installed")
    install(DIRECTORY include/context DESTINATION include/kie) # Copy folder context to include/kie
endif()


# with_serde is True
if(WITH_SERDE)
    find_package(Boost REQUIRED)
    target_link_libraries(kie_toolbox INTERFACE boost::boost)
    message(STATUS "WITH_SERDE is enabled, serde will be installed")
    install(DIRECTORY include/serde DESTINATION include/kie) # Copy folder serde to include/kie
endif()


# with_serde_json is True
if(WITH_SERDE_JSON)
    find_package(Boost REQUIRED)
    find_package(nlohmann_json REQUIRED)
    target_link_libraries(kie_toolbox INTERFACE boost::boost nlohmann_json::nlohmann_json)
    message(STATUS "WITH_SERDE_JSON is enabled, serde_json will be installed")
    install(DIRECTORY include/serde_json DESTINATION include/kie) # Copy folder serde_json to include/kie
endif()