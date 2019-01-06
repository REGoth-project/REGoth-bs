
find_package(Doxygen) 
find_package(Sphinx)

if (DOXYGEN_FOUND AND SPHINX_FOUND)

    # Configure Doxygen paths
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    # Build Doxygen XML, which can be read by Sphinx using Breathe
    add_custom_target( doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM )

    # Configure Sphinx
    set(SPHINX_WARNINGS_AS_ERRORS OFF)

    if(NOT DEFINED SPHINX_THEME)
        set(SPHINX_THEME sphinx_rtd_theme)
    endif()
    
    if(NOT DEFINED SPHINX_THEME_DIR)
        set(SPHINX_THEME_DIR)
    endif()
    
    # configured documentation tools and intermediate build results
    set(BINARY_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/_build")
    
    # Sphinx cache with pickled ReST documents
    set(SPHINX_CACHE_DIR "${CMAKE_CURRENT_BINARY_DIR}/_doctrees")
    
    # HTML output directory
    set(SPHINX_HTML_DIR "${CMAKE_CURRENT_BINARY_DIR}/html")

    # Make sure Breathe can find the generated XML
    set(DOXYGEN_XML_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/doc-doxygen/xml")
    
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/conf.py.in"
        "${BINARY_BUILD_DIR}/conf.py"
        @ONLY)
    
    add_custom_target(docs
        ${SPHINX_EXECUTABLE}
            -q -b html
            -c "${BINARY_BUILD_DIR}"
            -d "${SPHINX_CACHE_DIR}"
            "${CMAKE_CURRENT_SOURCE_DIR}"
            "${SPHINX_HTML_DIR}"
        COMMENT "Building HTML documentation with Sphinx"
        DEPENDS doc_doxygen)

endif()