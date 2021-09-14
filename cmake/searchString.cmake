
CPMAddPackage(
    NAME SentenceValidation
    GITHUB_REPOSITORY lidotcircle/SVMSentenceValidation
    GIT_TAG dev
    DOWNLOAD_ONLY ON
)

if(SentenceValidation_ADDED)
    add_subdirectory(
        ${SentenceValidation_SOURCE_DIR}
        ${SentenceValidation_BINARY_DIR}
    )
endif()

