/*
 * Mount tool fuse functions
 *
 * Copyright (C) 2011-2026, Omar Choudary <choudary.omar@gmail.com>,
 *                          Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#if !defined( _MOUNT_FUSE_H )
#define _MOUNT_FUSE_H

#include <common.h>
#include <types.h>

#if defined( HAVE_LIBFUSE ) || defined( HAVE_LIBFUSE3 ) || defined( HAVE_LIBOSXFUSE )

#if !defined( FUSE_USE_VERSION ) && !defined( CYGFUSE )

/* Ensure FUSE_USE_VERSION is defined before including fuse.h
 */
#if defined( HAVE_LIBFUSE3 )
#define FUSE_USE_VERSION	30
#else
#define FUSE_USE_VERSION	26
#endif

#endif /* !defined( FUSE_USE_VERSION ) && !defined( CYGFUSE ) */

#if defined( HAVE_LIBFUSE )
#include <fuse/fuse.h>
#elif defined( HAVE_LIBFUSE3 )
#include <fuse3/fuse.h>
#elif defined( HAVE_LIBOSXFUSE )
#include <osxfuse/fuse.h>
#endif

#endif /* defined( HAVE_LIBFUSE ) || defined( HAVE_LIBFUSE3 ) || defined( HAVE_LIBOSXFUSE ) */

#include "fvdetools_libcerror.h"
#include "fvdetools_libfvde.h"
#include "mount_file_entry.h"
#include "mount_handle.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( __APPLE__ ) && defined( FUSE_DARWIN_ENABLE_EXTENSIONS ) && FUSE_DARWIN_ENABLE_EXTENSIONS == 1
typedef struct fuse_darwin_attr mount_fuse_stat_t;
#elif defined( __CYGWIN__ ) && defined( FUSE_MAJOR_VERSION ) && FUSE_MAJOR_VERSION >= 3
typedef struct fuse_stat mount_fuse_stat_t;
#else
typedef struct stat mount_fuse_stat_t;
#endif

#if defined( __APPLE__ ) && defined( FUSE_DARWIN_ENABLE_EXTENSIONS ) && FUSE_DARWIN_ENABLE_EXTENSIONS == 1
#define mount_fuse_fill_dir_t fuse_darwin_fill_dir_t
#else
#define mount_fuse_fill_dir_t fuse_fill_dir_t
#endif

#if defined( HAVE_LIBFUSE ) || defined( HAVE_LIBFUSE3 ) || defined( HAVE_LIBOSXFUSE )

int mount_fuse_set_stat_info(
     mount_fuse_stat_t *stat_info,
     size64_t size,
     uint16_t file_mode,
     int64_t access_time,
     int64_t inode_change_time,
     int64_t modification_time,
     libcerror_error_t **error );

int mount_fuse_filldir(
     void *buffer,
     mount_fuse_fill_dir_t filler,
     const char *name,
     mount_fuse_stat_t *stat_info,
     mount_file_entry_t *file_entry,
     libcerror_error_t **error );

int mount_fuse_open(
     const char *path,
     struct fuse_file_info *file_info );

int mount_fuse_read(
     const char *path,
     char *buffer,
     size_t size,
     off_t offset,
     struct fuse_file_info *file_info );

int mount_fuse_release(
     const char *path,
     struct fuse_file_info *file_info );

int mount_fuse_opendir(
     const char *path,
     struct fuse_file_info *file_info );

#if defined( HAVE_LIBFUSE3 )
int mount_fuse_readdir(
     const char *path,
     void *buffer,
     mount_fuse_fill_dir_t filler,
     off_t offset,
     struct fuse_file_info *file_info,
     enum fuse_readdir_flags flags );
#else
int mount_fuse_readdir(
     const char *path,
     void *buffer,
     mount_fuse_fill_dir_t filler,
     off_t offset,
     struct fuse_file_info *file_info );
#endif

int mount_fuse_releasedir(
     const char *path,
     struct fuse_file_info *file_info );

#if defined( HAVE_LIBFUSE3 )
int mount_fuse_getattr(
     const char *path,
     mount_fuse_stat_t *stat_info,
     struct fuse_file_info *file_info );
#else
int mount_fuse_getattr(
     const char *path,
     mount_fuse_stat_t *stat_info );
#endif

void mount_fuse_destroy(
      void *private_data );

#endif /* defined( HAVE_LIBFUSE ) || defined( HAVE_LIBFUSE3 ) || defined( HAVE_LIBOSXFUSE ) */

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _MOUNT_FUSE_H ) */

