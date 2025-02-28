#include <apollo.h>
#include <dbglogger.h>
#define LOG dbglogger_log

#define APOLLO_PATH				"ms0:/APOLLO/"
#define APOLLO_APP_PATH			"ms0:/PSP/GAME/APOLLO/"
#define APOLLO_USER_PATH		APOLLO_PATH "FILES/"
#define APOLLO_DATA_PATH		APOLLO_APP_PATH "DATA/"
#define APOLLO_LOCAL_CACHE		APOLLO_APP_PATH "CACHE/"
#define APOLLO_UPDATE_URL		"https://api.github.com/repos/bucanero/apollo-psp/releases/latest"

#define MAX_USB_DEVICES         5
#define MC0_PATH                "mc0:/"
#define MC1_PATH                "mc1:/"
#define USB_PATH                "mass:/"
#define USER_PATH_USB           "PS2/SAVEDATA/"

#define PS2_SAVES_PATH_USB      "PS3/EXPORT/PS2SD/"
#define PSP_SAVES_PATH_USB      "APOLLO/SAVEDATA/"

#define PS1_SAVES_PATH_HDD      APOLLO_PATH "PS1/"
#define PSP_SAVES_PATH_HDD      USB_PATH USER_PATH_USB

#define PS1_IMP_PATH_USB        "PS1/SAVEDATA/"

#define EXPORT_PATH             "APOLLO/EXPORT/"
#define EXPORT_ZRIF_PATH        APOLLO_PATH "zrif/"

#define IMP_PS2VMC_PATH_USB     USB_PATH "PS2/VMC/"
#define IMPORT_RAP_PATH_USB     USB_PATH PS3_LICENSE_PATH

#define ONLINE_URL              "https://bucanero.github.io/apollo-saves/"
#define ONLINE_PATCH_URL        "https://bucanero.github.io/apollo-patches/PSP/"
#define ONLINE_CACHE_TIMEOUT    24*3600     // 1-day local cache

#define OWNER_XML_FILE          "owners.xml"

enum storage_enum
{
    STORAGE_MC0,
    STORAGE_MC1,
    STORAGE_MASS,
    STORAGE_HOST,
};

enum save_sort_enum
{
    SORT_DISABLED,
    SORT_BY_NAME,
    SORT_BY_TITLE_ID,
};

enum cmd_code_enum
{
    CMD_CODE_NULL,

// Save commands
    CMD_RESIGN_SAVE,
    CMD_DOWNLOAD_USB,
    CMD_DOWNLOAD_HDD,
    CMD_COPY_SAVE_USB,
    CMD_COPY_SAVE_HDD,
    CMD_EXPORT_ZIP_USB,
    CMD_VIEW_DETAILS,
    CMD_VIEW_RAW_PATCH,
    CMD_RESIGN_VMP,
    CMD_EXP_FINGERPRINT,
    CMD_CONVERT_TO_PSV,
    CMD_HEX_EDIT_FILE,
    CMD_EXPORT_DATA_FILE,
    CMD_IMPORT_DATA_FILE,

// Bulk commands
    CMD_RESIGN_SAVES,
    CMD_RESIGN_ALL_SAVES,
    CMD_COPY_SAVES_USB,
    CMD_COPY_ALL_SAVES_USB,
    CMD_COPY_SAVES_HDD,
    CMD_COPY_ALL_SAVES_HDD,
    CMD_DUMP_FINGERPRINTS,
    CMD_SAVE_WEBSERVER,

// Export commands
    CMD_EXP_KEYSTONE,
    CMD_SETUP_PLUGIN,
    CMD_EXP_VMP2MCR,
    CMD_EXP_PS2SAVE,
    CMD_DUMP_PSPKEY,

// Import commands
    CMD_IMP_SAVE_MC,
    CMD_IMP_MCR2VMP0,
    CMD_IMP_MCR2VMP1,
    CMD_EXTRACT_ARCHIVE,
    CMD_URL_DOWNLOAD,
    CMD_NET_WEBSERVER,

// SFO patches
    SFO_UNLOCK_COPY,
    SFO_CHANGE_ACCOUNT_ID,
    SFO_REMOVE_PSID,
    SFO_CHANGE_TITLE_ID,
};

// Save flags
#define SAVE_FLAG_MEMCARD       1
#define SAVE_FLAG_SELECTED      2
#define SAVE_FLAG_ZIP           4
#define SAVE_FLAG_PS1           8
#define SAVE_FLAG_PS2           16
#define SAVE_FLAG_PSP           32
#define SAVE_FLAG_ISO           64
#define SAVE_FLAG_LOCKED        128
#define SAVE_FLAG_ONLINE        256

enum save_type_enum
{
    FILE_TYPE_NULL,
    FILE_TYPE_PSV,
    FILE_TYPE_MENU,
    FILE_TYPE_PS2,

    // PS1 File Types
    FILE_TYPE_MCS,
    FILE_TYPE_PSX,
    FILE_TYPE_ZIP,

    // License Files
    FILE_TYPE_MAX,
    FILE_TYPE_CBS,
    FILE_TYPE_XPS,
    FILE_TYPE_PSU,

    // ISO Files
    FILE_TYPE_ISO,
    FILE_TYPE_NET,
};

enum char_flag_enum
{
    CHAR_TAG_NULL,
    CHAR_TAG_PS1,
    CHAR_TAG_PS2,
    CHAR_TAG_UNUSED1,
    CHAR_TAG_PSP,
    CHAR_TAG_UNUSED2,
    CHAR_TAG_APPLY,
    CHAR_TAG_OWNER,
    CHAR_TAG_LOCKED,
    CHAR_TAG_NET,
    CHAR_RES_LF,
    CHAR_TAG_TRANSFER,
    CHAR_TAG_ZIP,
    CHAR_RES_CR,
    CHAR_TAG_PCE,
    CHAR_TAG_WARNING,
    CHAR_BTN_X,
    CHAR_BTN_S,
    CHAR_BTN_T,
    CHAR_BTN_O,
    CHAR_TRP_BRONZE,
    CHAR_TRP_SILVER,
    CHAR_TRP_GOLD,
    CHAR_TRP_PLATINUM,
    CHAR_TRP_SYNC,
};

enum code_type_enum
{
    PATCH_NULL,
    PATCH_GAMEGENIE = APOLLO_CODE_GAMEGENIE,
    PATCH_BSD = APOLLO_CODE_BSD,
    PATCH_COMMAND,
    PATCH_SFO,
    PATCH_TROP_UNLOCK,
    PATCH_TROP_LOCK,
};

typedef struct save_entry
{
    char * name;
    char * title_id;
    char * path;
    char * dir_name;
    char * icon;
    uint16_t flags;
    uint16_t type;
    list_t * codes;
} save_entry_t;

typedef struct
{
    list_t * list;
    char path[128];
    char* title;
    uint8_t icon_id;
    void (*UpdatePath)(char *);
    int (*ReadCodes)(save_entry_t *);
    list_t* (*ReadList)(const char*);
} save_list_t;

list_t * ReadUsbList(const char* userPath);
list_t * ReadUserList(const char* userPath);
list_t * ReadOnlineList(const char* urlPath);
list_t * ReadBackupList(const char* userPath);
list_t * ReadTrophyList(const char* userPath);
void UnloadGameList(list_t * list);
char * readTextFile(const char * path, long* size);
int sortSaveList_Compare(const void* A, const void* B);
int sortSaveList_Compare_TitleID(const void* A, const void* B);
int sortCodeList_Compare(const void* A, const void* B);
int ReadCodes(save_entry_t * save);
int ReadTrophies(save_entry_t * game);
int ReadOnlineSaves(save_entry_t * game);
int ReadBackupCodes(save_entry_t * bup);

int network_up(void);
int http_init(void);
void http_end(void);
int http_download(const char* url, const char* filename, const char* local_dst, int show_progress);

int extract_7zip(const char* zip_file, const char* dest_path);
int extract_rar(const char* rar_file, const char* dest_path);
int extract_zip(const char* zip_file, const char* dest_path);
int zip_directory(const char* basedir, const char* inputdir, const char* output_zipfile);
int zip_append_directory(const char* basedir, const char* inputdir, const char* output_filename);

int show_dialog(int dialog_type, const char * format, ...);
int show_multi_dialog(const char** options, const char * msg);
int osk_dialog_get_text(const char* title, char* text, uint32_t size);
void init_progress_bar(const char* msg);
void update_progress_bar(uint64_t progress, const uint64_t total_size, const char* msg);
void end_progress_bar(void);
#define show_message(...)	show_dialog(DIALOG_TYPE_OK, __VA_ARGS__)

int init_loading_screen(const char* msg);
void stop_loading_screen(void);

void execCodeCommand(code_entry_t* code, const char* codecmd);

int create_savegame_folder(const char* folder);
int get_save_details(const save_entry_t *save, char** details);

int importCBS(const char *save, const char *mc_path);
int importPSU(const char *save, const char *mc_path);
int importXPS(const char *save, const char *mc_path);
int importMAX(const char *save, const char *mc_path);
int importPSV(const char *save, const char *mc_path);

int exportPSU(const char *mc_save, const char* out_path);
int exportPSV(const char *mc_save, const char* out_path);
int exportCBS(const char *mc_save, const char* out_path, const char* title);

int psp_DecryptSavedata(const char* fpath, const char* fname, uint8_t* key);
int psp_EncryptSavedata(const char* fpath, const char* fname, uint8_t* key);

int sjis2ascii(uint8_t* bData);
int vmp_resign(const char *src_vmp);
int ps1_mcr2vmp(const char* mcrfile, const char* vmp_path);
int ps1_vmp2mcr(const char* vmpfile, const char* mcr_path);
