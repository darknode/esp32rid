#include <Arduino.h>
#include "squid_tools.h"
#include "squid_instance.h"
#include "squid_network.h"

static Squid_Network network;
static Squid_Instance drone;
static Squid_Instance drone2;
static Squid_Tools tool;

void setup()
{
  Serial.begin(115200);
  tool.setupTime();
  network.begin(SD_NETWORK_MODE_BT); 

  delay(2000);

  drone.begin(&network);
  drone.setType(ODID_UATYPE_AEROPLANE);
  drone.setDescription("Military");
  drone.setName("MILMCP-CP #7883A");
  drone.setRemoteIdAsSerial("LAC05454");
  drone.setCenterLatLon(33.538016, -117.663136);
  drone.setSpeed(150);
  drone.update();

  drone2.begin(&network);
  drone2.setType(ODID_UATYPE_HELICOPTER_OR_MULTIROTOR);
  drone2.setDescription("Private");
  drone2.setName("DJI Mavic Model 3");
  drone2.setRemoteIdAsSerial("DJI");
  drone2.setCenterLatLon(33.538016, -117.663136);
  drone2.setSpeed(40);
  drone2.setDiffuser(1000);
  drone2.update();

  Squid_Path follow[] = {
    {SD_PATH_TYPE_GOTO, 0, 100},
    {SD_PATH_TYPE_GOTO, 90, 100},
    {SD_PATH_TYPE_GOTO, 180, 100},
    {SD_PATH_TYPE_GOTO, 270, 100},
  };

  drone.beginPath();
  drone.followPath(follow, 4);

  drone2.beginPath();
  drone2.randomPath();
  return;
}

void loop()
{
  drone.loop();
  drone2.loop();
  network.loop();
  return;
}
