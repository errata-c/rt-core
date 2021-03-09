
if(NOT TARGET glm)
	find_dependency(glm CONFIG)
endif()

if(NOT TARGET rt::loader)
	find_dependency(rt-loader CONFIG)
endif()

if(NOT TARGET ez::bitflags)
	find_dependency(ez-bitflags CONFIG)
endif()