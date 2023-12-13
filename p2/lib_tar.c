#include "lib_tar.h"

#include <string.h>



int calculate_chksum(tar_header_t pointer) {
    unsigned char ans = 0;
    ans += (unsigned char) pointer.name;
    ans += (unsigned char) pointer.mode;                 /* 100 */
    ans += (unsigned char) pointer.uid;                  /* 108 */
    ans += (unsigned char) pointer.gid;                  /* 116 */
    ans += (unsigned char) pointer.size;                /* 124 */
    ans += (unsigned char) pointer.mtime;               /* 136 */
    //ans += (unsigned char) pointer.chksum;   
    ans += (unsigned char) 32*sizeof(pointer.chksum);//not sure this is correct
    ans += (unsigned char) pointer.typeflag;                /* 156 */
    ans += (unsigned char) pointer.linkname;           /* 157 */
    ans += (unsigned char) pointer.magic;                /* 257 */
    ans += (unsigned char) pointer.version;              /* 263 */
    ans += (unsigned char) pointer.uname;               /* 265 */
    ans += (unsigned char) pointer.gname;               /* 297 */
    ans += (unsigned char) pointer.devmajor;             /* 329 */
    ans += (unsigned char) pointer.devminor;             /* 337 */
    ans += (unsigned char) pointer.prefix;             /* 345 */
    ans += (unsigned char) pointer.padding;  

    /*
    int ans = 0;
    for(int i=0; i<sizeof(pointer); i++){
        if(i< offsetof(tar_header_t, chksum) || i>= offsetof(tar_header_t, chksum)+sizeof(pointer.chksum)){
            ans += (unsigned char) pointer[i] //this is gonna be wrong how to iterate over a structure?
        }
    }
    */

}
/**
 * Checks whether the archive is valid.
 *
 * Each non-null header of a valid archive has:
 *  - a magic value of "ustar" and a null,
 *  - a version value of "00" and no null,
 *  - a correct checksum
 *
 * @param tar_fd A file descriptor pointing to the start of a file supposed to contain a tar archive.
 *
 * @return a zero or positive value if the archive is valid, representing the number of non-null headers in the archive,
 *         -1 if the archive contains a header with an invalid magic value,
 *         -2 if the archive contains a header with an invalid version value,
 *         -3 if the archive contains a header with an invalid checksum value
 */
int check_archive(int tar_fd) {

    //TO ADD
    //does magic contain a null? is it at end?
    // Does version contain a non null? at end?
    //Check checksum value --> need to calculate it 


    tar_header_t buff; //should i do a malloc?
    int curr = -1;
    curr = read(tar_fd, &buff, sizeof(tar_header_t));
    if(curr == -1){
        return 0;
    }
    int total = (int) calculate_chksum(buff);

    if( strcmp(TMAGIC, buff.magic)== 0 && buff.magic[5]=='\0'){
        if(strcmp(TVERSION, buff.version) == 0){
            if(TAR_INT(buff.chksum) == total){
                return  TAR_INT(buff.chksum);
            }
            else{
                return -3;
            }

        }
        else{//invalid version value
            return -2;
        }
    }
    else{ //invalid magic number 
        return -1;
    }

}

/**
 * Checks whether an entry exists in the archive.
 *
 * @param tar_fd A file descriptor pointing to the start of a valid tar archive file.
 * @param path A path to an entry in the archive.
 *
 * @return zero if no entry at the given path exists in the archive,
 *         any other value otherwise.
 */
int exists(int tar_fd, char *path) {
    return 0;
}

/**
 * Checks whether an entry exists in the archive and is a directory.
 *
 * @param tar_fd A file descriptor pointing to the start of a valid tar archive file.
 * @param path A path to an entry in the archive.
 *
 * @return zero if no entry at the given path exists in the archive or the entry is not a directory,
 *         any other value otherwise.
 */
int is_dir(int tar_fd, char *path) {
    return 0;
}

/**
 * Checks whether an entry exists in the archive and is a file.
 *
 * @param tar_fd A file descriptor pointing to the start of a valid tar archive file.
 * @param path A path to an entry in the archive.
 *
 * @return zero if no entry at the given path exists in the archive or the entry is not a file,
 *         any other value otherwise.
 */
int is_file(int tar_fd, char *path) {
    return 0;
}

/**
 * Checks whether an entry exists in the archive and is a symlink.
 *
 * @param tar_fd A file descriptor pointing to the start of a valid tar archive file.
 * @param path A path to an entry in the archive.
 * @return zero if no entry at the given path exists in the archive or the entry is not symlink,
 *         any other value otherwise.
 */
int is_symlink(int tar_fd, char *path) {
    return 0;
}


/**
 * Lists the entries at a given path in the archive.
 * list() does not recurse into the directories listed at the given path.
 *
 * Example:
 *  dir/          list(..., "dir/", ...) lists "dir/a", "dir/b", "dir/c/" and "dir/e/"
 *   ├── a
 *   ├── b
 *   ├── c/
 *   │   └── d
 *   └── e/
 *
 * @param tar_fd A file descriptor pointing to the start of a valid tar archive file.
 * @param path A path to an entry in the archive. If the entry is a symlink, it must be resolved to its linked-to entry.
 * @param entries An array of char arrays, each one is long enough to contain a tar entry path.
 * @param no_entries An in-out argument.
 *                   The caller set it to the number of entries in `entries`.
 *                   The callee set it to the number of entries listed.
 *
 * @return zero if no directory at the given path exists in the archive,
 *         any other value otherwise.
 */
int list(int tar_fd, char *path, char **entries, size_t *no_entries) {
    return 0;
}

/**
 * Reads a file at a given path in the archive.
 *
 * @param tar_fd A file descriptor pointing to the start of a valid tar archive file.
 * @param path A path to an entry in the archive to read from.  If the entry is a symlink, it must be resolved to its linked-to entry.
 * @param offset An offset in the file from which to start reading from, zero indicates the start of the file.
 * @param dest A destination buffer to read the given file into.
 * @param len An in-out argument.
 *            The caller set it to the size of dest.
 *            The callee set it to the number of bytes written to dest.
 *
 * @return -1 if no entry at the given path exists in the archive or the entry is not a file,
 *         -2 if the offset is outside the file total length,
 *         zero if the file was read in its entirety into the destination buffer,
 *         a positive value if the file was partially read, representing the remaining bytes left to be read to reach
 *         the end of the file.
 *
 */
ssize_t read_file(int tar_fd, char *path, size_t offset, uint8_t *dest, size_t *len) {
    return 0;
}