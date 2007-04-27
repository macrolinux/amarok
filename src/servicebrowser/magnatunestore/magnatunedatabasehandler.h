/***************************************************************************
 *   Copyright (c) 2006, 2007                                              *
 *        Nikolaj Hald Nielsen <nhnFreespirit@gmail.com>                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Steet, Fifth Floor, Boston, MA  02111-1307, USA.          *
 ***************************************************************************/ 

#ifndef MAGNATUNEDATABASEHANDLER_H
#define MAGNATUNEDATABASEHANDLER_H

#include "collectiondb.h"
#include "databasehandlerbase.h"
#include "magnatunetypes.h"

#include <QStringList>
#include <QMap>

typedef  QMap<QString, int> MagnatuneMoodMap;


/**
* This class wraps the database operations needed by the MagnatuneBrowser
* Uses the singleton pattern
*
* @author Nikolaj Hald Nielsen <nhnFreespirit@gmail.com>
*/
class MagnatuneDatabaseHandler : public DatabaseHandlerBase{
public:
    

    MagnatuneDatabaseHandler();
    ~MagnatuneDatabaseHandler();

    /**
     * Creates the tables needed to store Magnatune info
     */
    void createDatabase();

    /**
     * Destroys Magnatune tables
     */
    void destroyDatabase();

    /**
     * Inserts a new track into the Magnatune database
     * @param track pointer to the track to insert 
     * @param albumId id of the album containing the track
     * @param artistId id of the artist performing the track
     * @return the database id of the newly inserted track
     */
    int insertTrack(MagnatuneTrack *track, int albumId, int artistId);

    /**
     * inserts a new album into the Magnatune database
     * @param album pointer to the album to insert
     * @param artistId id of the artist performing the album
     * @return the database id of the newly inserted album
     */
    int insertAlbum(MagnatuneAlbum *album, int artistId);
   
    /**
     * inserts a new artist into the Magnatune database
     * @param artist pointer to the artist to insert
     * @return the database id of the newly inserted artist
     */
    int insertArtist(MagnatuneArtist *artist);

    //get id, or -1 if artist does not exist
    /**
     * Retrieves the id of a named artist
     * @param name artist name to retrieve
     * @return id of artist. -1 if no artist is found
     */
    int getArtistIdByExactName(const QString &name);


    /**
     * Retrieves the id of an album based on its unique album code.
     * @param albumcode The album code.
     * @return The id of the album, -1 if not foud.
     */
    int getAlbumIdByAlbumCode( const QString &albumcode );

    /**
     * Returns all artist that has albums in a given genre. If an artist has both a Rock
     * and a Techno album, he will be included when searching for either
     * @param genre the genre
     * @return  A list of artist in the genre
     */
    SimpleServiceArtistList getArtistsByGenre(const QString &genre);

    /**
     * Returns the artist with a given id
     * @param id The id of the artist to look for
     * @return The artist with the given id. Returns an empty artist if not found.
     */
    SimpleServiceArtist * getArtistById(int id);

    /**
     * Returns the album with a given id
     * @param id The id of the album to look for
     * @return The album with the given id. Returns an empty album if not found.
     */
    SimpleServiceAlbum * getAlbumById(int id);


     /**
     * Returns the track with a given id
     * @param id The id of the track to look for
     * @return The track with the given id. Returns an empty album if not found.
     */
    SimpleServiceTrack * getTrackById(int id);

    /**
     * Retrieves all albums by a single artist from the database
     * @param id The id of the artist
     * @param genre Limits the albums to a specific genre. Use "All" to get all albums
     * @return List of albums. empty if none are found
     */
    SimpleServiceAlbumList getAlbumsByArtistId(int id, const QString &genre);

    /**
     * Retrieves all tracks on a given album
     * @param id The id of the album
     * @return A list of tracks. Empty if album is not found or has no tracks
     */
    SimpleServiceTrackList getTracksByAlbumId(int id);

    /**
     * Retrieves all tracks by given artist
     * @param id The id of the artist
     * @return A list of tracks. Empty if artist is not found, artist has no albums or albums have no tracks
     */
    SimpleServiceTrackList getTracksByArtistId(int id); 

    /**
     * Retrieves a list of all genres present in the databse
     * @return A list of genres
     */
    QStringList getAlbumGenres();

    /**
     * Geet all moods for a track
     * @param id the track id
     * @return A list of moods
     */
    QStringList getMoodsForTrack( int id );

    /**
     * Returns a map of all defined moods. The key of the map is the moods itself (sorted
     * alfabetically ) and the value is the number of occurrences of the mood.
     * This is useful for displaying items in a cloud view
     * @param threshold Only include moods with more occurrences than treshold
     * @return A map of all defined moods and their frequency
     */
    MagnatuneMoodMap getMoodMap(int threshold = 0);

    /**
     * Returns all tracks that are tagged with a specific mood
     * @param mood The mood to search for
     * @return List of tracks
     */
    SimpleServiceTrackList getTracksByMood( QString mood );

    /**
     * Begins a database transaction. Must be followed by a later call to commit()
     */
    void begin();

    /**
     * Completes (executes) a database transaction. Must be preceded by a call to begin()
     */
    void commit();

};

#endif
