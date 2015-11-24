CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET (PROJECT_NAME "${EXAMPLE_NUMBER}")
PROJECT(PROJECT_NAME)

SET(MAIN_FILE "${EXAMPLE_NUMBER}") # the name of the source file with no extension
SET(EXEC_FILE "${EXAMPLE_NUMBER}") # the name of the executable file

femusMacroBuildApplication(${MAIN_FILE} ${EXEC_FILE})
