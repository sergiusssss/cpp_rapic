include(GoogleTest)

add_subdirectory(utils)

add_subdirectory(component)
add_subdirectory(unit)

configure_file(resources/certificate.crt resources/certificate.crt COPYONLY)
configure_file(resources/privatekey.key resources/privatekey.key COPYONLY)