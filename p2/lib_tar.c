#include "lib_tar.h"
#include <string.h>


/**
 * Calculates the simple sum of all bytes in a header block.
 * The chksum field is threated as if it were filled with spaces
 * 
 * @param pointer A pointer to a header block.
 * 
 * @return the simple sum of all bytes in the header block,
 *         if all fields except chcksum are null-filled, returns 256.
 */
int calculate_chksum(tar_header_t* pointer) {
    int ans = 256;
    char* head = (char*) pointer;
    for(int i=0; i< 148; i++){ans += *(head+i);}
    for(int i=156; i<512; i++){ans += *(head+i);}

    return ans;
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
    tar_header_t buff; 
    int total_headers = 0;
    int start= 0; 

    while(1){
        if(pread(tar_fd, &buff, sizeof(tar_header_t), start*BLOCK) < 0){
            return -4;
        }
        int total = (int) calculate_chksum(&buff);
        if(total == 256){
            return total_headers; 
        }
        if( strcmp(TMAGIC, buff.magic)!= 0){
            return -1;
        }
        if(buff.version[0]!= TVERSION[0] || buff.version[1] != TVERSION[1]){
            return -2;
        }
        if(TAR_INT(buff.chksum) != total){
            return -3;
        }

        total_headers++;
        start += (1+ TAR_INT(buff.size)/BLOCK);
        if(TAR_INT(buff.size) % BLOCK != 0) {
            start += 1;
        }   
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
    tar_header_t buff; 
    int start = 0;
    while(1){
        pread(tar_fd, &buff, sizeof(tar_header_t), start*BLOCK);
        if(calculate_chksum(&buff) == 256){
            return 0; //end of file
        }
        if(strcmp(buff.name, path)==0){
            return start+1;  //OUBLIE PAS DE METTRE -1 POUR ACCEDER AU HEADER
        }
        //passer au prochain file
        start += (1+ TAR_INT(buff.size)/BLOCK);
        if(TAR_INT(buff.size) % BLOCK != 0) {
            start += 1;
        }   
    }

}


/**
 * Checks whether an entry corresponds to the specified type
 * 
 * @param tar_fd A file descriptor pointing to the start of a valid tar archive file.
 * @param path A path to a valid entry in the archive.
 * @param type A char corresponding to a type of entry.
 *
 * @return zero if the entry is not of the specified type, the location of the archive otherwise.
 */
int check_type(int tar_fd, char *path, char type){
    tar_header_t buff;
    int result = exists(tar_fd, path);
    if(result == 0){
        return 0;
    }
    pread(tar_fd, &buff, sizeof(tar_header_t), (result-1)*BLOCK);
    if(buff.typeflag != type){
        return 0;
    }
    return result;
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
    return check_type(tar_fd, path, DIRTYPE);
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
    return check_type(tar_fd, path, REGTYPE) + check_type(tar_fd, path, AREGTYPE);
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
    return check_type(tar_fd, path, SYMTYPE);
}

int is_hardlink(int tar_fd, char *path) {
    return check_type(tar_fd, path, LNKTYPE);
}

int link_to(int tar_fd, char *path, char *ret_path) {
    tar_header_t buff;
    int loc = exists(tar_fd, path);
    pread(tar_fd, &buff, sizeof(tar_header_t), (loc-1)*BLOCK);

    if (buff.typeflag == SYMTYPE) {
        // on cherche la ou est situe buff
        int pre_path_len = strlen(buff.name);
        char *cur = buff.name + pre_path_len-1;
        while ((pre_path_len > 0) && (*cur != '/')) {
            cur--;
            pre_path_len--;
        }
        char newpath[512];
        strcpy(newpath, buff.name);
        strcpy(newpath + pre_path_len, buff.linkname);
        if (!exists(tar_fd, newpath)) strcat(newpath, "/");
        return link_to(tar_fd, newpath, ret_path);
    }

    if (buff.typeflag == LNKTYPE) {
        return link_to(tar_fd, buff.linkname, ret_path);
    }
    strcpy(ret_path, buff.name);
    return loc;
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
int list(int tar_fd, char *path, char **entries, size_t *no_entries) { // on a des erreurs en soumettant mais on a pas de feedback
    tar_header_t buff;
    char def_path[512];
    link_to(tar_fd, path, def_path); // on se dirige vers le dossier vers lequel pointe le potentiel symlink
    if (is_dir(tar_fd, def_path)) {
        int n_listed = 0;
        int len_path = strlen(def_path);
        int start = 0;
        while (n_listed < *no_entries) { // faire un while(n_listed < no_entries) mais attention checker que on dépasse pas la fin de l'archive
            if(calculate_chksum(&buff) == 256){
                // end of the archive
                *no_entries = n_listed;
                return 2;
            }
            // OK parcourir les entrees du dir
            // OK ne considerer que les fichiers dont le nom commence par (*def_path)
            // OK et qui ne contiennent aucun '/' suplémentaire ou seulement un a la toute fin
            pread(tar_fd, &buff, sizeof(tar_header_t), start*BLOCK);
            int cur_name_len = strlen(buff.name);
            if ((cur_name_len > len_path) && (strncmp(def_path, buff.name, len_path) == 0)) {
                //current is in def_path or in one of its sub-directories
                char cur_name[cur_name_len];
                strcpy(cur_name, buff.name);
                cur_name[cur_name_len - 1] = '\0'; //on enleve le dernier char du nom de l'entree pour que si c'est un dir ca prenne pas le '/' final
                if (strstr(cur_name+len_path, "/") == NULL) { // if there isn't a '/' after def_path (it's not in a sub-directory)
                    //current is directly in def_path
                    char pathname[512];
                    char linkto[512];
                    strcpy(pathname, buff.name);
                    link_to(tar_fd, buff.name, linkto);
                    if (is_symlink(tar_fd, buff.name) && is_dir(tar_fd, linkto)) {
                        strcpy(pathname+cur_name_len, "/");
                    }
                    strcpy(entries[n_listed], pathname);
                    n_listed++;
                }
            }

            //passer au suivant
            start += (1+ TAR_INT(buff.size)/BLOCK);
            if(TAR_INT(buff.size) % BLOCK != 0) {
                start += 1;
            }
        }
        *no_entries = n_listed;
        return 1;
    }
    *no_entries = 0;
    return 0; // if not is_dir()
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
ssize_t read_file(int tar_fd, char *path, size_t offset, uint8_t *dest, size_t *len) { // on a des erreurs en soumettant: len et return_value are wrong sometimes (peut-etre a cause de offset ?)
    tar_header_t buff;
    char def_path[512];
    link_to(tar_fd, path, def_path); // on se dirige vers le dossier vers lequel pointe le potentiel symlink

    int position = is_file(tar_fd, def_path);
    if (position) {
        // lire le fichier etc...
        pread(tar_fd, &buff, sizeof(tar_header_t), (position-1)*BLOCK);

        size_t cur_size = TAR_INT(buff.size);
        // check if offset > buff.size
        if (offset > cur_size) {
            *len = 0;
            return -2;
        }


        if (cur_size - offset <= *len) {
            *len = cur_size - offset;
            pread(tar_fd, dest, *len, position*BLOCK + offset);
            return 0;
        }
        pread(tar_fd, dest, *len, position*BLOCK + offset);
        return TAR_INT(buff.size) - *len - offset;
    }

    *len = 0;
    return -1; // file doesn't exists or isn't a file
}