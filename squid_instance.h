#ifndef SQUID_INSTANCE_H
#define SQUID_INSTANCE_H

// CONFIGURATION ----------------------------------------------------------------------
#define USE_WIFI_NAN 0
#define USE_WIFI_BEACON 0
#define USE_WIFI 0 // set to 0 if any of above enabled
#define USE_BT 1   // ASTM F3411-19 / ASD-STAN 4709-002.  .. good  for testing - gets picked up quickly
#define USE_BEACON_FUNC 0
#define USE_NATIVE_WIFI 0

#define SATS_LEVEL_1 4
#define SATS_LEVEL_2 7
#define SATS_LEVEL_3 10

#define DEFAULT_ALT 137.0
#define DEFAULT_SPEED 60
#define DEFAULT_NAME "UAS_NO_NAME"
#define DEFAULT_REMOTE_ID ""
#define DEFAULT_DESCRIPTION "Recreational"

// INTERNAL ---------------------------------------------------------------------------
#define WIFI_CHANNEL 6 // Be careful changing this.
#define BEACON_FRAME_SIZE 512
#define BEACON_INTERVAL 0 // ms, defaults to 500. Android apps would prefer 100ms.
#define AUTH_DATUM 1546300800LU
#define PARAM_SIZE 24
#define PATH_SIZE 50 // 50 points
#define M_MPH_MS 0.44704

// INCLUDES ---------------------------------------------------------------------------
#include "opendroneid.h"
#include "squid_tools.h"
#include "squid_network.h"

// ENUM ----------------------------------------------------------------------------
typedef enum Squid_Mode
{
    SD_MODE_IDLE = 0,
    SD_MODE_PATH = 1,
} Squid_Mode_t;

typedef enum Side_Path_Mode
{
    SD_PATH_MODE_IDLE = 0,
    SD_PATH_MODE_FOLLOW = 1,
    SD_PATH_MODE_RANDOM = 2,
} Squid_Path_Mode_t;

typedef enum Side_Path_Type
{
    SD_PATH_TYPE_GOTO = 0,   // uses heading and distance
    SD_PATH_TYPE_TRAVEL = 1, // uses lat/lon in whatever heading
    SD_PATH_TYPE_SET = 2,    // sets lat/lon
} Side_Path_Type_t;

// STRUCTS ----------------------------------------------------------------------------
struct Squid_Path
{
    Side_Path_Type_t type;
    double param1; // can be heading or lat
    double param2; // can be distance (in m) or lon
};

struct Squid_Params
{
    char uas_operator[PARAM_SIZE];
    char uas_description[PARAM_SIZE];
    char uas_id[PARAM_SIZE];
    ODID_uatype_t uas_type = ODID_UATYPE_NONE;
    ODID_idtype_t id_type = ODID_IDTYPE_NONE;
    char flight_desc[PARAM_SIZE];
    uint8_t
        region,
        spare1,
        eu_category,
        eu_class,
        id_type2,
        spare3;
    char id[PARAM_SIZE * 2];
    char secret[4];
    uint8_t spare[28];
};

struct Squid_Data
{
    int years;
    int months;
    int days;
    int hours;
    int minutes;
    int seconds;
    int csecs;
    double latitude_d;
    double longitude_d;
    float alt_msl_m;
    float alt_agl_m;
    int speed;
    int heading;
    char *hdop_s;
    char *vdop_s;
    int satellites;
    double base_latitude;
    double base_longitude;
    float base_alt_m;
    int base_valid;
    int vel_N_cm;
    int vel_E_cm;
    int vel_D_cm;
};

void construct2(void);
void init2(char *, int, uint8_t *, uint8_t);
int transmit_wifi2(uint8_t *, int);
int transmit_ble2(uint8_t *, int);

class Squid_Instance
{

public:
    Squid_Instance();
    void update();
    void update(struct Squid_Params *);
    void setAuth(char *);
    void setAuth(uint8_t *, short int, uint8_t);
    int transmit();
    int transmit(struct Squid_Data *);

    void begin(Squid_Network *n);
    void begin(Squid_Network *n, Squid_Params params);
    void loop(void);
    void setCenterLatLon(double lat, double lon);
    void setName(char *input);
    void setType(ODID_uatype_t type);
    void setRemoteId(char *input, ODID_idtype_t type);
    void setSpeed(int speed);
    void setRemoteIdAsSerial(char *input);
    void setRemoteIdAsFAARegistration(char *input);
    void clearRemoteId();
    void setDescription(char *input);

    void setMode(Squid_Mode_t m);

    void setDiffuser(uint32_t diff);

    void beginPath();
    void endPath();
    void randomPath();
    void followPath(Squid_Path *, int size);

private:
    void continueRandomPath();
    void continueFollowPath();

    Squid_Tools tools;
    Squid_Params params;
    Squid_Data data;
    Squid_Path path[PATH_SIZE];
    Stream *Debug_Serial = NULL;
    Squid_Mode_t mode = SD_MODE_IDLE;
    Squid_Path_Mode_t pathMode = SD_PATH_MODE_IDLE;
    Squid_Network *network;

    LatLon_t
        path_origin,
        path_target;

    float
        x = 0.0,
        y = 0.0,
        z = 100.0,
        speed_m_x,
        max_dir_change = 75.0;
    double
        deg2rad = 0.0,
        m_deg_lat = 0.0,
        m_deg_long = 0.0;
    int
        phase = 0,
        path_mode = 0,
        path_size = 0,
        path_index = 0,
        auth_page = 0,
        auth_page_count = 0,
        key_length = 0,
        iv_length = 0;
    char
        wifi_ssid[32],
        *uas_operator;

    uint32_t
        last_update,
        last_ble,
        last_msecs = 2000,
        path_ms_t = 0;

    uint16_t
        alt = DEFAULT_ALT,
        wifi_interval = 0,
        ble_interval = 0;

    uint8_t
        wifi_mac[6],
        wifi_channel = WIFI_CHANNEL,
        *auth_key = NULL,
        *auth_iv = NULL,
        msg_counter[16],
        use_bt = 0,
        use_wifi = 0;

    size_t
        wifi_ssid_length = 0;

    int transmit_wifi(struct Squid_Data *, int);
    int transmit_ble(uint8_t *, int);

#if USE_WIFI
    uint16_t sequence = 1, beacon_interval = 0x200;
#if USE_WIFI_BEACON
    int beacon_offset = 0, beacon_max_packed = 30;
    uint8_t beacon_frame[BEACON_FRAME_SIZE],
#if USE_BEACON_FUNC
        beacon_counter = 0;
#else
        *beacon_payload, *beacon_timestamp, *beacon_counter, *beacon_length, *beacon_seq;
#endif
#endif
#endif

#if USE_WIFI_BEACON && (!USE_BEACON_FUNC)
    void init_beacon();
#endif

#if USE_BT
    uint8_t ble_message[36], counter = 0;
#endif

    ODID_UAS_Data UAS_data;
    ODID_BasicID_data *basicID_data;
    ODID_Location_data *location_data;
    ODID_Auth_data *auth_data[ODID_AUTH_MAX_PAGES];
    ODID_SelfID_data *selfID_data;
    ODID_System_data *system_data;
    ODID_OperatorID_data *operatorID_data;

    ODID_BasicID_encoded basicID_enc[2];
    ODID_Location_encoded location_enc;
    ODID_Auth_encoded auth_enc;
    ODID_SelfID_encoded selfID_enc;
    ODID_System_encoded system_enc;
    ODID_OperatorID_encoded operatorID_enc;
};

#endif
