add_definitions(${MYSQL_CFLAGS})

########### mysqlserver ###############

set( amarok_storage-mysqlserverstorage_PART_SRCS
     ../mysql-shared/MySqlStorage.cpp
     MySqlServerStorage.cpp
     MySqlServerStorageFactory.cpp
)

kde4_add_plugin(amarok_storage-mysqlserverstorage ${amarok_storage-mysqlserverstorage_PART_SRCS})

STRING(REPLACE "-Wl,--fatal-warnings" "" CMAKE_SHARED_LINKER_FLAGS_NOFATALWARN "${CMAKE_SHARED_LINKER_FLAGS}")
SET(CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS_NOFATALWARN} )

STRING(REPLACE "-Wl,--fatal-warnings" "" CMAKE_MODULE_LINKER_FLAGS_NOFATALWARN "${CMAKE_MODULE_LINKER_FLAGS}")
SET(CMAKE_MODULE_LINKER_FLAGS ${CMAKE_MODULE_LINKER_FLAGS_NOFATALWARN} )

target_link_libraries(amarok_storage-mysqlserverstorage
    # amarok-sqlstorage
    amarokcore
    amaroklib
    ${KDE4_KDECORE_LIBS}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTGUI_LIBRARY}
    ${MYSQL_LIBRARIES}
    ${CMAKE_DL_LIBS}
    ${ZLIB_LIBRARIES}
)

if(NOT WIN32 AND NOT APPLE)
    target_link_libraries( amarok_storage-mysqlserverstorage crypt pthread )
endif(NOT WIN32 AND NOT APPLE)

if(APPLE)
	SET_TARGET_PROPERTIES(amarok_storage-mysqlserverstorage PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
endif(APPLE)

install(TARGETS amarok_storage-mysqlserverstorage DESTINATION ${PLUGIN_INSTALL_DIR} )

install(FILES amarok_storage-mysqlserverstorage.desktop DESTINATION ${SERVICES_INSTALL_DIR})
