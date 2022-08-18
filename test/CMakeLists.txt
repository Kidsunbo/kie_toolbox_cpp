if(WITH_CONTEXT)
    add_subdirectory(context)
endif()

if(WITH_SERDE)
    add_subdirectory(serde)
endif()

if(WITH_SERDE_JSON)
    add_subdirectory(serde_json)
endif()

if(WITH_CONTAINER)
    add_subdirectory(container)
endif()