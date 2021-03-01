

set(${PACK_NAME}_INCLUDE_DIR "${PACK_ROOT}/include")
set(${PACK_NAME}_HEADERS "${PACK_ROOT}/include")
set(${PACK_NAME}_CONFIG_DIR "${PACK_ROOT}/share/${PACK_NAME}/cmake")

if(NOT TARGET glm)
	find_dependency(glm CONFIG)
endif()

if(NOT TARGET ez::glcompat)
	find_dependency(ez-glcompat CONFIG)
endif()
